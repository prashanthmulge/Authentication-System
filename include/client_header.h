/*******************************************************************************
* 	FILE NAME	: client_header.h
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

/*****************************************************************
*		Header files
******************************************************************/

#include	<stdio.h>

#include	<string.h>

#include	<stdlib.h>

#include	<errno.h>

#include	<sys/socket.h>

#include	<arpa/inet.h>

#include	<netinet/in.h>

#include	<fcntl.h>

#include	<ctype.h>

#include	<unistd.h>

#include	<time.h>

/******************************************************************
*		MACRO DEFINITION
******************************************************************/

#define MAX_UNAMEPWD 25

#define MAX_DATA 100

#define SUCCESS 1

#define FAILURE 0

#define RETRY 2

#define MORPH_ENCRYPT 4

#define LOGIN 3

#define IN

#define OUT

#define IN_OUT

#define CHAR_TO_INT 48

/*************** TRACE MACRO ************************************/

#define NOTRACE 0

#define BRIEF 1

#define DETAIL 2

/******************************************************************
*		EXTERN VARIABLE
******************************************************************/

extern int g_trace_level;		// Default trace level for tracing

/******************************************************************
*		TRACE
******************************************************************/

#ifdef TRACE
 	
	#define A_TRACE(level,x) if(level <= g_trace_level) printf x

 #else
 	
	#define A_TRACE(level,x)

 #endif

/******************************************************************
*		Structure
*******************************************************************/

typedef struct data
{
	char *username;
	
	char *password;

}user_info;

/*******************************************************************
*		Function Delceration
*******************************************************************/

int user_choice();


int get_user_info_login(
			user_info**,		//structure address
			int,			//socket descriptor
			char 			//choice
		        );


int get_user_info_register(
				user_info**,	//structure address
				int,		//socket descriptor
				char		//choice
			  );

int socket_create(	
			int *,			//socket descriptor
			struct sockaddr_in *,	//server address
			char *,			//ip address of server
			char *			//port address of server		
		 );

int check_username(
			user_info *,		//structure
			int,			//socket descriptor
			char			//choice
		  );

int get_username(
			user_info **		//structure address
		);

int get_password(
			user_info **		//structure address
		);


int encrypt_password(
			user_info **		//structure address
		    );

int read_from_server(
			int,			//socket descriptor
			char			//choice
		    );

int write_to_server(
			int,			//socket descriptor	
			user_info *,		//structure
			char			//choice
		   );

void exit_user(
			int			//socket descriptor
	      );

void mapping(
			int,			//numeric choice
			char*			//alpha choice
	    );

void free_memory(
			user_info **		//strusture address
		);

