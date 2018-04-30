/*
* main.c
* Beaglebone Main thread which launches comms and logger threads
* and handles SIGINT
*
* @author Connor Shapiro
*/

#include <signal.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>

#include "../inc/bb_comms.h"
#include "../inc/bb_logger.h"

static volatile bool b_sigexit;

static void sigint_handler(int signum)
{
    b_sigexit = true;
}

int main(int argc, char * argv[])
{
    const char * name = PIPE_NAME;
    int pipe_fd;
    
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

    /* Start up Logger task using thread below */
    uint8_t log_args;
    pthread_attr_t log_tattr;
    pthread_attr_init(&log_tattr);  // Default pthread attr
    pthread_t * log_thread;
    if (pthread_create(log_thread, &log_tattr, logger, &log_args))
    {
        perror("(Main) [ERROR]: Could not create Logger.\n");
        return FAILURE;
    }
    else
    {
        printf("(Main) [INFO]: Created Logger.\n");
    }

	/* Init Comms */
    uint8_t comms_args;
    pthread_attr_t comms_tattr;
    pthread_attr_init(&comms_tattr);
    pthread_t * comms_thread;
    if (pthread_create(comms_thread, &comms_tattr, comms, &comms_args))
    {
        perror("(Main) [ERROR]: Could not create Comms.\n");
        return FAILURE;
    }
    else
    {
        printf("(Main) [INFO]: Created Comms.\n");
    }

    /* Main loop */
    while (!b_sigexit);    


    /*Cleanup of logger thread*/
    log_msg_t * p_log_msg;
    p_log_msg->level = COMMAND;
    p_log_msg->source = MAIN;
    time(&p_log_msg->timestamp);
    strcpy(p_log_msg->str, "exit");
    int8_t * log_ret;
    if (pthread_join(*log_thread, (void **)&log_ret))
    {
        perror("(Main) [ERROR]: Could not join Logger.\n");
        return FAILURE;
    }
    if (!log_ret)
    {
        perror("(Main) [WARNING]: Logger experienced a runtime error.\n");
    }

    /*Cleanup of temp thread*/
    
    p_log_msg->level = COMMAND;
    p_log_msg->source = MAIN;
    time(&p_log_msg->timestamp);
    strcpy(p_log_msg->str, "exit");
    int8_t * temp_ret;
    if (pthread_join(*temp_thread, (void **)&temp_ret))
    {
        perror("(Main) [ERROR]: Could not join Temperature.\n");
        return FAILURE;
    }
    if (!temp_ret)
    {
        perror("(Main) [WARNING]: Temperature experienced a runtime error.\n");
    }


    /*Cleanup of light thread*/
    
    p_log_msg->level = COMMAND;
    p_log_msg->source = MAIN;
    time(&p_log_msg->timestamp);
    strcpy(p_log_msg->str, "exit");
    int8_t * light_ret;
    if (pthread_join(*light_thread, (void **)&light_ret))
    {
        perror("(Main) [ERROR]: Could not join light.\n");
        return FAILURE;
    }
    if (!light_ret)
    {
        perror("(Main) [WARNING]: light experienced a runtime error.\n");
    }


   /*Cleanup of remote thread*/

    p_log_msg->level = COMMAND;
    p_log_msg->source = MAIN;
    time(&p_log_msg->timestamp);
    strcpy(p_log_msg->str, "exit");
    int8_t * remote_ret;
    if (pthread_join(*remote_thread, (void **)&remote_ret))
    {
        perror("(Main) [ERROR]: Could not join remote.\n");
        return FAILURE;
    }
    if (!remote_ret)
    {
        perror("(Main) [WARNING]: remote experienced a runtime error.\n");
    }



    /* Cleanup Main */
    if (mq_unlink(MAIN_MQ))
    {
        perror("(Main) [ERROR]: Failed to destory Main message queue.\n");
        return FAILURE;
    }
    return SUCCESS;
}