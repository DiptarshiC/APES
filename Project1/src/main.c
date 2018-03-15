/** @file main.c
*
* @description This module contains the Main thread which is launched from the
*              command line and which launches all other threads, checks for
*              occasional heartbeats from each thread, handles a SIGINT signal
*              (telling other threads to gracefully quit), and sends error
*              to the USR LEDs
*
* @date 13th March 2017
*
* @authors Diptarshi Chakraborty and Connor Shapiro
*/

#include <pthread.h>
#include <mqueue.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <stdint.h>
/* Un-comment as remote gets implemented */
// #include "../includes/remote.h"
#include "../includes/light.h"
#include "../includes/temp.h"
#include "../includes/logger.h"
#include "../includes/main.h"

#define SUCCESS             0
#define FAILURE             -1
#define EXPECTED_ARGC       2
#define START_OK_TIME_US    500000
#define HEARTBEAT_TIME_US   5000000
#define MAIN_MQ             "/tmp/mainmq"
#define LOG_MQ              "/tmp/logmq"
#define REMOTE_MQ           "/tmp/remotemq"
#define LIGHT_MQ            "/tmp/lightmq"
#define TEMPERATURE_MQ      "/tmp/temperaturemq"
#define MAIN_MAX_MSGS       20
#define PRIORITY_LOWEST     0
#define PRIORITY_ONE        1
#define PRIORITY_TWO        2
// #define N_THREADS           4
#define N_THREADS           1 /* Uncomment above and delete this line when all 
                               * threads are implemented with msg queues
                               */

static volatile bool b_sigexit;

static void sigint_handler(int signum)
{
    b_sigexit = true;
}

int main(int argc, char * argv[])
{
    /* Check Params */
    if (argc != EXPECTED_ARGC)
    {
        printf("Usage: %s \"path/to/logfile\"\n", argv[0]);
        return FAILURE;
    }
    if (strlen(argv[1]) > MAX_LOG_NAME)
    {
        printf("Path to logfile (incl. file name) is too long. Must be <= 15 characters.\n");
        return FAILURE;        
    }

    /* Set up capability for application graceful exit */
    struct sigaction act;
    act.sa_handler = sigint_handler;
    if (sigemptyset(&act.sa_mask))
    {
        perror("(Main) [ERROR]: Failed to set empty sigaction mask.\n");
        return FAILURE;
    }
    act.sa_flags = SA_RESETHAND;
    if (sigaction(SIGINT, &act, NULL))
    {
        perror("(Main) [ERROR]: Failed to set up custom SIGINT handler.\n");
        return FAILURE;
    }

    /* Create message queues */
    struct mq_attr main_mq_attr;
    main_mq_attr.mq_maxmsg = MAIN_MAX_MSGS;
    main_mq_attr.mq_msgsize = sizeof(main_msg_t);
    mqd_t main_mqd = mq_open(MAIN_MQ, O_CREAT | O_RDONLY | O_NONBLOCK, 0666, 
                &main_mq_attr); // Use nonblock only for Main MQ
    struct mq_attr log_mq_attr;
    log_mq_attr.mq_maxmsg = LOG_MAX_MSGS;
    log_mq_attr.mq_msgsize = sizeof(log_msg_t);
    mqd_t log_mqd = mq_open(LOG_MQ, O_CREAT | O_WRONLY, 0666, &log_mq_attr);
    /* Un-comment as queues get implemented into modules */
    // struct mq_attr remote_mq_attr;
    // log_mq_attr.mq_maxmsg = REMOTE_MAX_MSGS;
    // log_mq_attr.mq_msgsize = sizeof(remote_msg_t);
    // mqd_t remote_mqd = mq_open(LOG_MQ, O_CREAT | O_WRONLY, 0666,
    //                                                         &remote_mq_attr);
    // struct mq_attr light_mq_attr;
    // log_mq_attr.mq_maxmsg = LIGHT_MAX_MSGS;
    // log_mq_attr.mq_msgsize = sizeof(light_msg_t);
    // mqd_t light_mqd = mq_open(LOG_MQ, O_CREAT | O_WRONLY, 0666,
    //                                                         &light_mq_attr);
    // struct mq_attr temp_mq_attr;
    // log_mq_attr.mq_maxmsg = TEMP_MAX_MSGS;
    // log_mq_attr.mq_msgsize = sizeof(temp_msg_t);
    // mqd_t temp_mqd = mq_open(LOG_MQ, O_CREAT | O_WRONLY, 0666, &temp_mq_attr);

    /* Start up Logger thread */
    struct log_thread_info * p_log_args = (struct log_thread_info *) 
                                        malloc(sizeof(struct log_thread_info));
    if (p_log_args == NULL)
    {
        perror("(Main) [ERROR]: Failed to allocate dynamic memory for"
                                                        " log_thread args.\n");
        return FAILURE;
    }
    strcpy(p_log_args->log_mq_name, LOG_MQ);
    strcpy(p_log_args->main_mq_name, MAIN_MQ);
    strcpy(p_log_args->log_filename, argv[1]);

    pthread_attr_t log_tattr;
    pthread_attr_init(&log_tattr);  // Default pthread attr
    pthread_t * log_thread;
    if (pthread_create(log_thread, &log_tattr, logger, p_log_args))
    {
        perror("(Main) [ERROR]: Could not create Logger.\n");
        return FAILURE;
    }
    else
    {
        log_msg_t log_msg;
        time(log_msg.timestamp);
        log_msg.level = INFO;
        log_msg.source = MAIN;
        strcpy(log_msg.str, "Successfully created Logger.\n");
        mq_send(log_mqd, &log_msg, sizeof(log_msg_t), PRIORITY_LOWEST);
    }
    free(p_log_args);

    /* Copy/paste above for the other three once threading is implemented */
    
    /* Now that all four child threads have been started up, wait for START_OK.
     * If it doesn't come within 500ms, something is wrong
     */
    usleep(START_OK_TIME_US);
    uint8_t i_threads;
    main_msg_t msg_main;
    for (i_threads = 0; i_threads < N_THREADS; i_threads++)
    {
        if (mq_receive(main_mqd, &msg_main, sizeof(main_msg_t), NULL))
        {
            perror("(Main) [ERROR]: A thread did not start OK.\n");
            return FAILURE;
        }
    }

    /* Main loop */
    while (!b_sigexit)
    {
        /* Heartbeat */
        log_msg_t log_msg;
        log_msg.level = COMMAND;
        log_msg.source = MAIN;
        time(log_msg.timestamp);
        strcpy(log_msg.str, "heartbeat");
        mq_send(log_mqd, &log_msg, sizeof(log_msg_t), PRIORITY_TWO);
        // Repeat above for other modules
        usleep(HEARTBEAT_TIME_US);
        for (i_threads = 0; i_threads < N_THREADS; i_threads++)
        {
            if (mq_receive(main_mqd, &msg_main, sizeof(main_msg_t), NULL))
            {
                perror("(Main) [ERROR]: A thread did not heartbeat.\n");
                return FAILURE;
            }
        }

        /* USR LED functionality goes here */
    }

    /* Cleanup child threads */
    log_msg_t log_msg;
    log_msg.level = COMMAND;
    log_msg.source = MAIN;
    time(log_msg.timestamp);
    strcpy(log_msg.str, "exit");
    int8_t * log_ret;
    if (pthread_join(log_thread, &log_ret))
    {
        perror("(Main) [ERROR]: Could not join Logger.\n");
        return FAILURE;
    }
    if (!log_ret)
    {
        perror("(Main) [WARNING]: Logger experienced a runtime error.\n");
    }

    /* Repeat for other threads */

    /* Cleanup Main */
    if (mq_unlink(MAIN_MQ))
    {
        perror("(Main) [ERROR]: Failed to destory Main message queue.\n");
        return FAILURE;
    }
    return SUCCESS;
}