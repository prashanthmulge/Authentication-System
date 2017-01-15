/*******************************************************************************
* 	FILE NAME	: server_header.h
*
* 	DESCRIPTION	: Contains the MACRO and funtion declarations
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
* Header files
******************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
/******************************************************************
* MACRO DEFINITION
******************************************************************/

#define MAX_UNAMEPWD 25

#define MAX_BUF_SERVER 100

#define SUCCESS 1

#define FAILURE 0

#define MY_PORT 1234

#define QUE_LEN 10

#define ACK 2

#define FILE_CONTENTS 3

#define IN

#define OUT

#define IN_OUT

/******************************************************************
* EXTERN VARIABLE
******************************************************************/

extern int g_trace_level;

/******************************************************************
* TRACE_MACROS
******************************************************************/

#define NOTRACE 0

#define BRIEF 1

#define DETAIL 2

#ifdef TRACE

        #define A_TRACE(level,x) if(level <= g_trace_level) printf x

 #else

        #define A_TRACE(level,x)

 #endif


/******************************************************************
 * Structure
 *******************************************************************/

 typedef struct user_details
 {
         char *username;

         char *password;

 }user_details;

 /*******************************************************************
 * Function Declaration
 *******************************************************************/
void sigchld_handler(int);

void sigint_handler(int);

void decision_on_return_status(int, char, char *, user_details *, int);

int tokenisation(char *, char *,user_details **);

int choice_of_login_register_store(char,user_details *);

int login_verification(user_details *,char);

int registration(user_details *,char);

int file_search(user_details *,char);

void decrypt_password(char *);

int compare_username(char *,char *,char *, FILE *);

int compare_password(char *, char *);

int read_from_client(int, char*);

int write_to_client(int, char*, int);

int open_file(FILE **,int);

int close_file(FILE *);

int write_to_file(user_details *, FILE *);

int store_details(user_details *);

int read_file_write_to_client(FILE *, char*,int);

void read_from_file(char *, FILE *);

void exit_user(int, user_details *);

void free_memory(user_details *);

int connection_status(int *,int,char*, struct sockaddr_in *);

int socket_create( int *, struct sockaddr_in *);
