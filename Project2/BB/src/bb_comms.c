/*
 * bb_comms.c
 * Beaglebone UART/SPI Communications application
 *      for interfacing with the TIVA TM4C1294
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef UART
    #include <termios.h>
#elif defined(SPI)
    #include <sys/ioctl.h>
	#include <linux/types.h>
	#include <linux/spi/spidev.h>
#endif  // UART/SPI

#include "../../inc/API.h"
#include "../inc/bb_comms.h"

#define EXPECETED_ARGC      (2)
#define FAILURE             (-1)
#define SUCCESS             (0)
#define ONE_BYTE            (1)
#define BITS_EIGHT          (8)
#define FOUR_BYTES          (4)
#define UART1               ("/dev/ttyS1")
#define SPI0                ("/dev/spidev0.0")
#define VMAX                (255)
#define HALF_SECOND         (5)
#define FOURTEEN_BYTES      (14)
#define KHZ_500             (500000)
#define CS_ON               ("0")
#define CS_OFF              ("1")

#ifdef UART
    #define WRITE_OUT_COMMAND() (write(fd_comm, &outgoing_cmd, sizeof(outgoing_cmd)))
#elif defined(SPI)
    #define READ_ELEMENT(x)     ()
    #define WRITE_OUT_COMMAND() (send_byte_SPI(&fd_comm, outgoing_cmd))
#endif  // UART/SPI

typedef enum
{
    WAITING_TO_START,       // First state after bootup
    DUMPING_GAME,           // Second state, receiving game but not controller
    RECEIVING_CONTROLLER    // Third state, only receiving controller
                            /* Task can transition from third state to second state
                             * or out to exit. First state can only transition to 
                             * second state or exit, second to third or exit.
                             */
} bb_comms_state_t;

#ifdef SPI
static const char CS0[] = "/sys/class/gpio/gpio48/value";
    #ifdef TWO_CONTROLLERS
static const char CS1[] = "/sys/class/gpio/gpio12/value";
    #endif  // TWO_CONTROLLERS
#endif  // SPI

static bool packet_checksum_pass (uint32_t checksum, comm_packet_t * p_packet);
static int8_t comms_init (int * p_fd);
static int8_t send_byte_SPI (int * p_fd, uint8_t command);

/* Test program to dump cartridge ROM to a file */
int main(int argc, char *argv[])
{
    FILE * ROM_file;    // File to dump ROM into
    int fd_comm;
    bool graceful_exit;
    bb_comms_state_t state;
    uint32_t inc_checksum;      // inc = incoming
    uint8_t p_buf_packet[COMMS_QUEUE_SIZE + sizeof(inc_checksum)];
#ifdef SPI
//    FILE * CS0_fd, * CS1_fd;
    struct spi_ioc_transfer rx_params =
    {
        .tx_buf = (uint32_t)NULL,
        .rx_buf = (uint32_t)p_buf_packet,
        .len = COMMS_QUEUE_SIZE + sizeof(inc_checksum),
        .delay_usecs = 0,
        .speed_hz = KHZ_500,
        .bits_per_word = BITS_EIGHT,
        .cs_change = true
    };
#endif  // SPI
    bool good_packet;
    bool good_gamefile;
    uint8_t outgoing_cmd;

    comm_packet_t inc_packet;

    uint32_t i_pl;

    /* Check parameters */
    if (argc != EXPECETED_ARGC)
    {
        printf("Usage: %s \"path/to/dumpfile\"\n", argv[0]);
        return FAILURE;
    }

    state = WAITING_TO_START;

    graceful_exit = false;
    while (!graceful_exit)
    {
        switch (state)
        {
            case WAITING_TO_START:
                if (FAILURE == comms_init(&fd_comm))
                {
                    return FAILURE;
                }

// #ifdef SPI
//                 /* Open CS_n file handles */
//                 CS0_fd = fopen(CS0, "r+");
//     #ifdef TWO_CONTROLLERS
//                 CS1_fd = fopen(CS1, "r+");
//                 if (!(CS0_fd && CS1_fd))
//     #else
//                 if (!(CS0_fd))
//     #endif  // TWO_CONTROLLERS
//                 {
//                     perror("Failed to open CS file descriptor.\n");
//                     return FAILURE;
//                 }
// #endif  // SPI
                
                state = DUMPING_GAME;
                outgoing_cmd = SEND_GAME;
//                fwrite(CS_ON, sizeof(uint8_t), 1, CS0_fd); // Only ever send GAME command to first TIVA
//                fflush(CS0_fd);
                WRITE_OUT_COMMAND();
//                fwrite(CS_OFF, sizeof(uint8_t), 1, CS0_fd);
//                fflush(CS0_fd);
            break;

            case DUMPING_GAME:
                good_gamefile = false;
                while (!good_gamefile)                // Gamefile loop
                {
                    good_packet = false;
                    while (!good_packet)    // Packet loop
                    {
                        /* Receive packet */
#ifdef UART
                        read(fd_comm, p_buf_packet, COMMS_QUEUE_SIZE + sizeof(inc_checksum));
#elif defined(SPI)
                        ioctl(fd_comm, SPI_IOC_MESSAGE(1), &rx_params);
#endif  // UART/SPI
                        /* Split out packet items - start with checksum */
                        for (i_pl = 0; i_pl < FOUR_BYTES; i_pl++)
                        {
                            *((uint8_t *)&inc_checksum + i_pl) = *((uint8_t *)p_buf_packet + i_pl);
                        }

                        /* Then non-payload comm_packet_t */
                        for (i_pl = 0; i_pl < COMMS_QUEUE_OVERHEAD; i_pl++)
                        {
                            *((uint8_t *)&inc_packet + i_pl) = p_buf_packet[i_pl + sizeof(inc_checksum)];
                        }

                        /* Finally, payload */
                        for (i_pl = 0; i_pl < inc_packet.ulSize; i_pl++)
                        {
                            *((uint8_t *)&inc_packet + i_pl + COMMS_QUEUE_OVERHEAD) = p_buf_packet[i_pl + sizeof(inc_checksum) + COMMS_QUEUE_OVERHEAD];
                        }

                        /* Evaluate checksum */
                        good_packet = packet_checksum_pass(inc_checksum, &inc_packet);

                        /* If bad, request re-send of TIVA's buffered packet */
                        if (!good_packet)
                        {
                            outgoing_cmd = GAME_CHKSUM_BAD;
                            WRITE_OUT_COMMAND();
                        }
                    }
                    /* When a good packet comes across, TIVA shall dequeue and
                     * send next packet
                     */
                    outgoing_cmd = GAME_CHKSUM_GOOD;
                    WRITE_OUT_COMMAND();
                }
                /* Need to add a way to revert to beginning of game dump if bad
                 * cartridge checksum, this will require changes to TIVA fw
                 */
                state = RECEIVING_CONTROLLER;
                outgoing_cmd = SEND_CONTROL;
                WRITE_OUT_COMMAND();
            break;

            case RECEIVING_CONTROLLER:

            break;

            default:    // How did we get here??
                perror("Somehow got to default state.\n");
                return FAILURE;
            break;
        }
    }

    /* Graceful Exit */
    close(fd_comm);
#ifdef SPI
//    fclose(CS0_fd);
    #ifdef TWO_CONTROLLERS
    fclose(CS1_fd);
    #endif // TWO_CONTROLLERS
#endif  // SPI
    
}

static bool packet_checksum_pass(uint32_t checksum, comm_packet_t * p_packet)
{
    uint32_t i_chksum;
    uint32_t eval_chksum = 0;
    
    /* Calculate additive checksum */
    for (i_chksum = 0; i_chksum < (COMMS_QUEUE_OVERHEAD + 
                                                p_packet->ulSize); i_chksum++)
    {
        eval_chksum += *((uint8_t *)p_packet + i_chksum);
    }

    /* Return */
    if (eval_chksum == checksum)
    {
        return true;
    }
    return false;
}

static int8_t comms_init(int * p_fd)
{
#ifdef UART
    // Need to pass fd in by reference
    *p_fd = open(UART1, O_RDWR | O_NOCTTY);
    if (*p_fd < SUCCESS)
    {
        return FAILURE;
    }
    
    /* Setup UART session with termios */
    struct termios session;
    tcgetattr(*p_fd, &session);
    session.c_iflag = IGNBRK;
    session.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
    session.c_lflag = 0;         // Non-canonical blocking mode
    session.c_cc[VMIN] = 0;         // Can't read more than 255 bytes in one read()
    session.c_cc[VTIME] = 100; // Longest burst SHOULD only take 
                                       // 0.2 seconds, give it up to 0.5
    tcflush(*p_fd, TCIFLUSH);
    tcsetattr(*p_fd, TCSANOW, &session);

#elif defined(SPI)
    uint32_t spi_mode;
    uint32_t size;
    uint32_t speed;
    *p_fd = open(SPI0, O_RDWR);

    if (*p_fd < SUCCESS)
    {
        perror("Failed to open SPI0\n");
        return FAILURE;
    }
    spi_mode = 0;
    size = BITS_EIGHT;
    speed = KHZ_500;
    /* Modes */
    if (FAILURE == ioctl(*p_fd, SPI_IOC_WR_MODE, &spi_mode))
    {
        perror("Failed to set SPI output mode\n");
        return FAILURE;
    }
    if (FAILURE == ioctl(*p_fd, SPI_IOC_RD_MODE, &spi_mode))
    {
        perror("Failed to set SPI input mode\n");
        return FAILURE;
    }

    /* Transaction Size */
    if (FAILURE == ioctl(*p_fd, SPI_IOC_WR_BITS_PER_WORD, &size))
    {
        perror("Failed to set output transaction size\n");
        return FAILURE;
    }
    if (FAILURE == ioctl(*p_fd, SPI_IOC_RD_BITS_PER_WORD, &size))
    {
        perror("Failed to set input transactino size\n");
        return FAILURE;
    }

    /* Transaction Speed */
    if (FAILURE == ioctl(*p_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed))
    {
        perror("Failed to set output speed\n");
        return FAILURE;
    }
    if (FAILURE == ioctl(*p_fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed))
    {
        perror("Failed to set input speed\n");
        return FAILURE;
    }
}

static int8_t send_byte_SPI(int * p_fd, uint8_t command)
{
    uint32_t tx = (uint32_t)command;
    struct spi_ioc_transfer tx_params =
    {
        .tx_buf = (uint32_t)&tx,
        .rx_buf = (uint32_t)NULL,
        .len = ONE_BYTE,
        .delay_usecs = 0,
        .speed_hz = KHZ_500,
        .bits_per_word = BITS_EIGHT,
        .cs_change = true
    };

    if (ioctl(*p_fd, SPI_IOC_MESSAGE(1), &tx_params) < ONE_BYTE)
    {
        perror("SPI CMD send failed\n");
        return FAILURE;
    }
    else
    {
        return SUCCESS;
    }
}
#endif  // UART/SPI