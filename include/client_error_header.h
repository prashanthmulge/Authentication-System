
/*******************************************************************************
* 	FILE NAME	: client_error_header.h
*
* 	DESCRIPTION	: Header file 
*
* 	DATE		NAME		   REFERENCE	      REASON
*---------------------------------------------------------------------------
*       13/Nov/2012      Raghavendra Gutta   N/A           Initial Creation
*       14/Nov/2012      Prashant Mulge      N/A           Minor Changes
*       15/Nov/2012      Srishti Dalela      N/A           ACG Review
*       16/Nov/2012      Kumar Gaurav        N/A           Minor Changes
*       18/Nov/2012      Mano Anantharaman   N/A           Review Comments*
*
*	Copyright (c) 2012 Aricent Technology (Holdings) Ltd.,
******************************************************************************/

/*******************************************************************************
				HEADERS AND MACROS
*******************************************************************************/

#ifndef ERROR_HEADER
#define ERROR_HEADER

/******************************************************************************
*		headers
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

/******************** ERROR LEVELS *********************************************/

 #define ERROR_ALL 3
 #define ERROR_MINOR 2
 #define ERROR_MAJOR 1
 #define ERROR_CRITICAL 0


/****************** ERROR CODES ***********************************************/

#define ERROR_MALLOC_FAILURE 0
#define ERROR_READ_SOCKET 1
#define ERROR_WRITE_SOCKET 2
#define ERROR_CREAT_SOCKET 3
#define ERROR_CONNECT_SOCKET 4
#define ERROR_OPEN_FILE 5
#define ERROR_CLOSE_FILE 6
#define ERROR_IP_ADDRESS_UNSPECIFIED 7
#define ERROR_PORT_UNSPECIFIED 8
#define ERROR_LOGIN_FAILURE 9
#define ERROR_REGISTER_FAILURE 10
#define ERROR_BIND 11
#define ERROR_ACCEPT 12
#define ERROR_LISTEN 13
#define ERROR_WRITE_FILE 14
#define ERROR_READ_FILE 15
#define ERROR_CHOICE_SELECT 16

/********************PORTABILITY********************************************/

#define MY_INT int32_t

/******************* EXTERN VARIALES **************************************/

extern int g_error_level;		// Default trace level for error handling

/***************** ERROR FUNCTION *****************************************/

 #define LOG_ERROR(error_level, error_code) if(g_error_level >= error_level) error_handler(error_level, error_code)

 
/***************************************************************************
*		Function prototypes
***************************************************************************/

 
MY_INT print_date_time(
			FILE *
		 );

void error_handler( 
			MY_INT error_level, 
			MY_INT error_code
	   	  );

#endif
