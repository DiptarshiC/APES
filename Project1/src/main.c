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
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../includes/remote.h"
#include "../includes/light.h"
#include "../includes/temp.h"
#include "../includes/logger.h"
#include "../includes/main.h"




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
    mqd_t main_mqd = mq_open(MAIN_MQ, O_CREAT | O_RDONLY | O_NONBLOCK, 0666,&main_mq_attr); // Use nonblock only for Main MQ

    struct mq_attr log_mq_attr;
    log_mq_attr.mq_maxmsg = LOG_MAX_MSGS;
    log_mq_attr.mq_msgsize = sizeof(log_msg_t);
    mqd_t log_mqd = mq_open(LOG_MQ, O_CREAT | O_WRONLY, 0666, &log_mq_attr);

    struct mq_attr remote_mq_attr;
    log_mq_attr.mq_maxmsg = REMOTE_MAX_MSGS;
    log_mq_attr.mq_msgsize = sizeof(remote_msg_t);
    mqd_t remote_mqd = mq_open(REMOTE_MQ, O_CREAT | O_WRONLY, 0666, &remote_mq_attr);

    struct mq_attr light_mq_attr;
    log_mq_attr.mq_maxmsg = LIGHT_MAX_MSGS;
    log_mq_attr.mq_msgsize = sizeof(light_msg_t);
    mqd_t light_mqd = mq_open(LIGHT_MQ, O_CREAT | O_WRONLY, 0666, &light_mq_attr);

    struct mq_attr temp_mq_attr;
    log_mq_attr.mq_maxmsg = TEMP_MAX_MSGS;
    log_mq_attr.mq_msgsize = sizeof(temp_msg_t);
    mqd_t temp_mqd = mq_open(TEMPERATURE_MQ , O_CREAT | O_WRONLY, 0666, &temp_mq_attr);

    /* Start up Logger task using thread below */
    struct log_thread_info * p_log_args = (struct log_thread_info *)  malloc(sizeof(struct log_thread_info));
    if (p_log_args == NULL)
    {
    perror("(Main) [ERROR]: Failed to allocate dynamic memory for log_thread args.\n");
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
        log_msg_t * p_log_msg;
        time(&p_log_msg->timestamp);
        p_log_msg->level = INFO;
        p_log_msg->source = MAIN;
        strcpy(p_log_msg->str, "Successfully created Logger.\n");
        mq_send(log_mqd, (char *) p_log_msg, sizeof(log_msg_t), PRIORITY_LOWEST);
    }
    free(p_log_args);

    /* Creating temperature task using code below */
    
     struct temp_thread_info * p_temp_args = (struct temp_thread_info *)  malloc(sizeof(struct temp_thread_info));
    if (p_temp_args == NULL)
    {
    perror("(Main) [ERROR]: Failed to allocate dynamic memory for temp_thread args.\n");
        return FAILURE;
    }
    strcpy(p_temp_args->main_mq_name, MAIN_MQ);
    strcpy(p_temp_args->temp_mq_name, TEMPERATURE_MQ);
    strcpy(p_temp_args->remote_mq_name,REMOTE_MQ);
   strcpy(p_temp_args->log_mq_name,LOG_MQ);

    pthread_attr_t temp_tattr;
    pthread_attr_init(&temp_tattr);  // Default pthread attr
    pthread_t * temp_thread;
    if (pthread_create(temp_thread, &temp_tattr, temp, p_temp_args))
    {
        perror("(Main) [ERROR]: Could not create temperature task.\n");
        return FAILURE;
    }
    else
   {
        log_msg_t * p_log_msg;
        time(&p_log_msg->timestamp);
        p_log_msg->level = INFO;
        p_log_msg->source = MAIN;
        strcpy(p_log_msg->str, "Successfully created Temperature task.\n");
        mq_send(log_mqd, (char *) p_log_msg, sizeof(log_msg_t), PRIORITY_LOWEST);
    }
	 free(p_log_args);

	 /* Creating light task using code below */

   struct light_thread_info * p_light_args = (struct light_thread_info *)  malloc(sizeof(struct light_thread_info));
    if (p_light_args == NULL)
    {
    perror("(Main) [ERROR]: Failed to allocate dynamic memory for light_thread args.\n");
        return FAILURE;
    }
    strcpy(p_light_args->main_mq_name, MAIN_MQ);
    strcpy(p_light_args->light_mq_name, LIGHT_MQ);
    strcpy(p_light_args->remote_mq_name,REMOTE_MQ);
    strcpy(p_light_args->log_mq_name,LOG_MQ);

    pthread_attr_t light_tattr;
    pthread_attr_init(&light_tattr);  // Default pthread attr
    pthread_t * light_thread;
    if (pthread_create(light_thread, &light_tattr, light, p_light_args))
    {
        perror("(Main) [ERROR]: Could not create light task.\n");
        return FAILURE;
    }
    else
   {
        log_msg_t * p_log_msg;
        time(&p_log_msg->timestamp);
        p_log_msg->level = INFO;
        p_log_msg->source = MAIN;
  	strcpy(p_log_msg->str, "Successfully created light task.\n");
        mq_send(log_mqd, (char *) p_log_msg, sizeof(log_msg_t), PRIORITY_LOWEST);
    }
         free(p_log_args);


	 /* Creating remote task using code below */

 struct remote_thread_info * p_remote_args = (struct remote_thread_info *)  malloc(sizeof(struct remote_thread_info));
    if (p_remote_args == NULL)
    {
    perror("(Main) [ERROR]: Failed to allocate dynamic memory for remote_thread args.\n");
        return FAILURE;
    }
    strcpy(p_remote_args->main_mq_name, MAIN_MQ);
    strcpy(p_remote_args->light_mq_name, LIGHT_MQ);
    strcpy(p_remote_args->temp_mq_name, TEMPERATURE_MQ );
    strcpy(p_remote_args->remote_mq_name,REMOTE_MQ);
    pthread_attr_t remote _tattr;
    pthread_attr_init(&remote_tattr);  // Default pthread attr
    pthread_t * remote_thread;
    if (pthread_create(remote_thread, &remote_tattr, remote, p_remote_args))
    {
        perror("(Main) [ERROR]: Could not create remote task.\n");
        return FAILURE;
    }
    else
   {
        log_msg_t * p_log_msg;
        time(&p_log_msg->timestamp);
        p_log_msg->level = INFO;
        p_log_msg->source = MAIN;
	  strcpy(p_log_msg->str, "Successfully created remote task.\n");
        mq_send(log_mqd, (char *) p_log_msg, sizeof(log_msg_t), PRIORITY_LOWEST);
    }
         free(p_log_args);




    usleep(START_OK_TIME_US);
    uint8_t i_threads;
    main_msg_t * msg_main;
    for (i_threads = 0; i_threads < N_THREADS; i_threads++)
    {
        if (mq_receive(main_mqd, (char *) msg_main, sizeof(main_msg_t), NULL))
        {
            perror("(Main) [ERROR]: A thread did not start OK.\n");
            return FAILURE;
        }
    }

    /* Main loop */
    while (!b_sigexit)
    {
        /* Heartbeat to the logger*/
        log_msg_t * p_log_msg;
        p_log_msg->level = COMMAND;
        p_log_msg->source = MAIN;
        time(&p_log_msg->timestamp);
        strcpy((char *) &p_log_msg->str, "heartbeat");
        mq_send(log_mqd, (char *) p_log_msg, sizeof(log_msg_t), PRIORITY_TWO);


	 /* Heartbeat to temperature*/
        temp_msg_t * p_temp_msg;
        p_temp_msg->id = TEMP_HEARTBEATREQ;
        p_log_msg->source = T_MAIN;
        mq_send(temp_mqd, (char *) p_temp_msg, sizeof(temp_msg_t), PRIORITY_TWO);


         /* Heartbeat to light*/
        light_msg_t * p_light_msg;
        p_light_msg->id =  LIGHT_HEARTBEATREQ;
	p_light_msg->source = L_MAIN;
        mq_send(light_mqd, (char *) p_light_msg, sizeof(light_msg_t), PRIORITY_TWO);

	   /* Heartbeat to remote*/
        remote_msg_t * p_remote_msg;
        p_light_msg->id = R_MAIN;
        mq_send(remote_mqd, (char *) p_remote_msg, sizeof(remote_msg_t), PRIORITY_TWO);




        // Repeat above for other modules
        usleep(HEARTBEAT_TIME_US);
        for (i_threads = 0; i_threads < N_THREADS; i_threads++)
        {
            if (mq_receive(main_mqd, (char *) msg_main, sizeof(main_msg_t), NULL))
            {
                perror("(Main) [ERROR]: A thread did not heartbeat.\n");
                return FAILURE;
            }
        }

        /* USR LED functionality goes here */
    }

    /* Cleanup child threads */


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
