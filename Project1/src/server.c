/**
*@file: server.c
*
*@description: This header file provides definitions
*               remote server.
*
*@Date:2nd March 2017
*
*@Author:Diptarshi Chakraborty and Connor Shapiro
*/


#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../includes/i2c.h"
#include "../includes/temp.h"
#include "../includes/logger.h"
#include "../includes/light.h"
#include "../includes/main.h"
