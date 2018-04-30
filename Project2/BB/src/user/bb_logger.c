/*
* bb_logger.c
* BB remote logging server thread
*
* @author Connor Shapiro
*/

#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "../../../inc/API.h"
#include "../../inc/bb_logger.h"

#define PIPE_NAME           ("/tmp/project2_pipe")
#define LOG_NAME            ("/home/debian/lkm-development/BB/src/logfile")
#define INTERNAL_STR_SIZE   (128)
#define SUCCESS             (0)
#define FAILURE             (-1)
#define TIME_STR_SIZE       (9)

/*!
* @brief Internal function to log any timestamped string literal to logfile.
* @description  Appends a timestamp to the beginning of param string and a 
*               newline at the end, then logs that to the logfile.
* @param[in] string String to be logged
* @return int8_t
*/

static FILE * gp_log_file;

static int8_t log_str(uint8_t * string)
{
    time_t thetime;
    if (FAILURE == time(&thetime))
    {
        perror("Failed to get time.\n");  /* Use perror() instead of some sort
                                           * weird recursive logging.
                                           */
        return FAILURE;
    }
    uint8_t strtime[TIME_STR_SIZE];
    strftime(strtime, TIME_STR_SIZE, "%T", localtime(&thetime));
    if (fprintf(gp_log_file, "%s - %s\n", strtime, string) < SUCCESS)
    {
        perror("Failed to write to logfile.\n");
        return FAILURE;
    }
    return SUCCESS;
}

void * logger(void * arg)
{
    const char * name = PIPE_NAME;
    int pipe_fd;
    comm_packet_t * p_packet;
    bool graceful_exit;
    uint8_t string[INTERNAL_STR_SIZE] = "";
    uint8_t str_level[10] = "";
    uint8_t str_source[16] = "";
    uint8_t str_msg[100] = "";
    uint8_t message[32][4] = {0,};

    /* Open logfile */
    gp_log_file = fopen(LOG_NAME, "a");
    if (!gp_log_file)
    {
        perror("(Logger) [ERROR]: Failed to open log file.\n");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
    }

    /* Access named pipe */
    if (FAILURE == open(name, O_RDONLY))
    {
        if (mkfifo(name, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
                                                            | S_IWOTH))
        {
            log_str("(Logger) [ERROR]: Creation of named pipe failed.\n");
            int8_t retvalue = FAILURE;
            pthread_exit(&retvalue);
        }
    }

    /* Allocate memory for packet */
    p_packet = (comm_packet_t *)malloc(sizeof(comm_packet_t));
    if (!p_packet)
    {
        log_str("(Logger) [ERROR]: Dynamic allocation of memory for comms packet failed...\n");
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
    }

    /* Main loop */
    graceful_exit = false;
    while (!graceful_exit)
    {
        pipe_fd = open(name, O_RDONLY); // Blocking until comms opens to write
        if (FAILURE == pipe_fd)
        {
            log_str("(Logger) [ERROR]: Failed to open named pipe for read.\n");
            int8_t retvalue = FAILURE;
            pthread_exit(&retvalue);
        }

        /* Read in log packet */
        if (FAILURE == read(pipe_fd, p_packet, sizeof(comm_packet_t)))
        {
            log_str("(Logger) [ERROR]: Read from pipe failed.\n");
            int8_t retvalue = FAILURE;
            pthread_exit(&retvalue);
        }
        close(pipe_fd);

        /* Use BB_LOGGER xSource as a way to signal exit */
        if (BB_COMMS == p_packet->xSource)
        {
            graceful_exit = true;
        }

        else // Log
        {
            /* Prep level */
            strcpy(str_level, "[INFO]: ");

            /* Prep source */
            switch (p_packet->xSource)
            {
                case (TIVA_INIT):
                    str_source = "(TIVA) ";
                break;

                case (TIVA_CART):
                    str_source = "(TIVA_CART) ";
                break;

                case (TIVA_CONTROL0):
                    str_source = "(TIVA_CONTROL0) ";
                break;

                case (TIVA_CONTROL1):
                    str_source = "(TIVA_CONTROL1) ";
                break;

                case (TIVA_LOGGER):
                    str_source = "(TIVA_LOGGER) ";
                break;

                case (TIVA_XPORT):
                    str_source = "(TIVA_XPORT) ";
                break;

                case (TIVA_COMMS):
                    str_source = "(TIVA_COMMS) ";
                break;

                default:
                break;
            }

            /* Prep Log*/
            strcpy(string, str_source);
            strcat(string, str_level);
            message = p_packet->ucPayload;
            switch (message)
            {
                case ({'S','I','_','_'}):
                    str_msg = "System Init";
                break;

                case ({'C','O','_','I'}):
                    str_msg = "Controller Init";
                break;

                case ({'C','O','_','E'}):
                    str_msg = "Controller Exit";
                break;

                case ({'C','A','_','I'}):
                    str_msg = "Cartridge Init";
                break;

                case ({'C','A','_','E'}):
                    str_msg = "Cartridge Exit";
                break;

                case ({'T','R','_','I'}):
                    str_msg = "Transport Init";
                break;

                case {'T','R','_','E'}():
                    str_msg = "Transport Exit";
                break;

                case ({'L','O','_','I'}):
                    str_msg = "Logger Init";
                break;

                case ({'L','O','_','E'}):
                    str_msg = "Logger Exit";
                break;

                default:
                break;
            }
            strcat(string, str_msg);

            /* Log */
            log_str(string);
        }
    }
    /* Graceful Exit */
    int8_t retvalue = FAILURE;
    pthread_exit(&retvalue);
}