/**
*@file: Kthread.c
*
*@Description: use the kthread API to create
*a kernel module with a second thread that
*allows the two to communicate via queues (kfifo).
*The first thread should send information to the
*second thread on a timed interval through the
*fifo. The second thread should take data from the kfifo
*and print it to the kernel logger.
*The info you should pass should relate to the currently scheduled
*processes in the rbtree. What was
*the ID and vruntime of the previous, current, and next PID
*
*@Author : Diptarshi Chakraborty
*
*@Date : February 3rd 2018
*
*
*/
