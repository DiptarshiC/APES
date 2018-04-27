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
#ifdef UART
    #define UART1               ("/dev/ttyS1")
    #define READ_ELEMENT(x)     (read(fd_serial, &x, sizeof(x)))
    #define WRITE_OUT_COMMAND() (write(fd_serial, &outgoing_cmd, sizeof(outgoing_cmd)))
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
    comm_packet_t * p_inc_packet;
    uint32_t i_pl;

    /* Check parameters */
    if (argc != EXPECETED_ARGC)
    {
        printf("Usage: %s \"path/to/dumpfile\"\n", argv[0]);
        return FAILURE;
    }

    state = WAITING_TO_START;
    p_inc_packet = malloc(COMMS_QUEUE_SIZE);
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
                        READ_ELEMENT(inc_checksum);
                        READ_ELEMENT(p_inc_packet->xDest);
                        READ_ELEMENT(p_inc_packet->xSource);
                        READ_ELEMENT(p_inc_packet->usPadding);
                        READ_ELEMENT(p_inc_packet->ulSize);
                        read(fd_serial, &p_inc_packet->ucPayload,
                                                        p_inc_packet->ulSize);
                        /* Evaluate checksum */
                        good_packet = packet_checksum_pass(inc_checksum,
                                                                    p_inc_packet);
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
                // Need to add a way to revert to beginning of game dump if bad cartridge checksum, this will require changes to TIVA fw
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
    free(p_inc_packet);
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
    *p_fd = open(UART1, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (*p_fd < SUCCESS)
    {
        return FAILURE;
    }
    
    /* Setup UART session with termios */
    struct termios session;
    tcgetattr(*p_fd, &session);
    session.c_iflag = IGNPAR;
    session.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
    tcflush(*p_fd, TCIOFLUSH);
    tcsetattr(*p_fd, TCSANOW, &session);

#elif defined(SPI)
#endif
}