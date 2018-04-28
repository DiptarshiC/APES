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
#include <termios.h>
#include "../../inc/API.h"
#include "../inc/bb_comms.h"

#define EXPECETED_ARGC      (2)
#define FAILURE             (-1)
#define SUCCESS             (0)
#define FOUR_BYTES          (4)
#ifdef UART
    #define UART1               ("/dev/ttyS1")
    #define WRITE_OUT_COMMAND() (write(fd_serial, &outgoing_cmd, sizeof(outgoing_cmd)))
    #define VMAX                (255)
    #define HALF_SECOND         (5)
    #define FOURTEEN_BYTES      (14)
#elif defined(SPI)
    #define READ_ELEMENT(x)     ()
    #define WRITE_OUT_COMMAND() ()
#endif

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

static int8_t comms_init (int * fd);
static bool packet_checksum_pass (uint32_t checksum, comm_packet_t * p_packet);

/* Test program to dump cartridge ROM to a file */
int main(int argc, char *argv[])
{
    FILE * ROM_file;    // File to dump ROM into
#ifdef UART
    int fd_serial;      // File description for UART1
#elif defined(SPI)
#endif
    bool graceful_exit;
    bb_comms_state_t state;
    bool good_packet;
    bool good_gamefile;
    uint8_t outgoing_cmd;
    uint32_t inc_checksum;      // inc = incoming
    comm_packet_t inc_packet;
    uint8_t p_buf_packet[COMMS_QUEUE_SIZE + sizeof(inc_checksum)];
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
                if (FAILURE == comms_init(&fd_serial))
                {
                    return FAILURE;
                }
                state = DUMPING_GAME;
                outgoing_cmd = SEND_GAME;
                WRITE_OUT_COMMAND();
            break;

            case DUMPING_GAME:
                good_gamefile = false;
                while (!good_gamefile)                // Gamefile loop
                {
                    good_packet = false;
                    while (!good_packet)    // Packet loop
                    {
                        /* Receive packet */
                        read(fd_serial, p_buf_packet, COMMS_QUEUE_SIZE + sizeof(inc_checksum));

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
                perror("Somehow got to default state.");
                return FAILURE;
            break;
        }
    }

    /* Graceful Exit */
    
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
#endif
}