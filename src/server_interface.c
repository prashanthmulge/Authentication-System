/*******************************************************************************
* 	FILE NAME	: server_interface.c
*
* 	DESCRIPTION	: Interface file for server side functionalities 
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

/*****************************************************************************
*		Header
******************************************************************************/
#include "server_header.h"
#include "server_error_header.h"

/*****************************************************************************
*
*       Function Name : main
*
*       Description : It forks the main process and assigns each child process to one client
*
*       Return Values : SUCCESS or FAILURE
*
******************************************************************************/
MY_INT g_trace_level = NOTRACE;			//Default trace level for tracing 

MY_INT main(
		MY_INT argc,
		char* argv[]
	)
{
	

	MY_INT sd, connfd, ret_bind,ret_read, ret;
			
	user_details *uname_pwd = NULL;

	pid_t pid = 0;

	struct sockaddr_in server_addr, client_addr;
	
	char buf[MAX_BUF_SERVER];

	if(FAILURE == socket_create(&sd,&server_addr))
	{
		A_TRACE(NOTRACE,("\nsocket creation ERROR..\n"));
		
		return FAILURE;
	
	}

	if(-1 == (ret_bind = bind(sd, (struct sockaddr*)&server_addr, sizeof(server_addr))))
	{

		LOG_ERROR(ERROR_MAJOR,ERROR_BIND);

		return FAILURE;
	}

	A_TRACE(NOTRACE,("\nserver socket binded..\n"));
	
	if(-1 == listen(sd, QUE_LEN))
	{

		LOG_ERROR(ERROR_MAJOR,ERROR_LISTEN);

		return FAILURE;

	}

	A_TRACE(DETAIL,("\nout of server listen func..\n"));
	
	for(;;)
	{


		A_TRACE(NOTRACE,("\nWaiting for the client to connect...\n"));
		
		memset(buf, '\0', MAX_BUF_SERVER);

		if( FAILURE == connection_status(&connfd,sd,buf,&client_addr))
		{
			A_TRACE(NOTRACE,("\nunable to connect client..\n"));

			return FAILURE;
		}

		A_TRACE(NOTRACE,("\naccept(connfd) success..\n"));

		if((pid = fork()) == 0)
		{

			A_TRACE(BRIEF,("\nentered fork..\n"));

			char choice='\0';

			close(sd);

			if(FAILURE == (ret_read = read_from_client(connfd, buf)))
			{

				A_TRACE(NOTRACE,("\nread from client failed..\n"));

				A_TRACE(NOTRACE,("\nclosing client descriptor and exiting..\n"));

				close(connfd);

				exit(0);

			}
			
			A_TRACE(DETAIL,("\nread from client finished..\n"));

			A_TRACE(DETAIL,("\ndata from client=%s\n",buf));

			if( FAILURE == tokenisation(buf,&choice,&uname_pwd))
			{

				A_TRACE(NOTRACE,("\ntokenisation failure..\n"));

				exit(0);

			}

			A_TRACE(DETAIL,("\ntokenized..\n"));

			A_TRACE(DETAIL,("\nchoice=%c,uname=%s,pwd=%s\n",choice,uname_pwd->username,uname_pwd->password));

			ret=choice_of_login_register_store(choice,uname_pwd);

			A_TRACE(BRIEF,("\nchoice selection done and status of login/register updated..\n"));

			A_TRACE(DETAIL,("\nchoice ret=%d\n",ret));

			memset(buf, '\0', MAX_BUF_SERVER);

			decision_on_return_status(ret,choice,buf,uname_pwd,connfd);

			//A_TRACE(BRIEF,("\ndecision done..\n"));

			//A_TRACE(NOTRACE,("\nchild finished serving client...exiting\n"));

			//exit(0);

		}
		
		signal(SIGINT, sigint_handler);

		A_TRACE(BRIEF,("\nout of fork by main..\n"));

		close(connfd);

	}
	
	return SUCCESS;
}
