/*******************************************************************************
* 	FILE NAME	: client_interface.c
*
* 	DESCRIPTION	: Main function for the client.
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

/****************************************************************************************
*			HEARDER 							*
****************************************************************************************/		

#include "client_header.h"

#include "client_error_header.h"

/****************************************************************************************
*			MAIN								*
****************************************************************************************/

MY_INT g_trace_level = NOTRACE;

MY_INT main(MY_INT argc,char **argv)
{

	MY_INT socket_desp=0,ret=0;

	MY_INT ch=0,flag = 0;

	char choice;

	struct sockaddr_in server_address;

	user_info *data = NULL; 		//user data (username and password)

	srand(time(NULL));				// changing the seed value everytime for rand

	A_TRACE(BRIEF,("\nENTERING MAIN\n"));
	
	A_TRACE(NOTRACE,("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<WELCOME>>>>>>>>>>>>>>>>>>>>>>>>>>\n"));

	if(NULL == argv[1])
	{
		A_TRACE(NOTRACE,("\nNO IP ADDRESS and PORT number mentioned as argument...\n"));
		
		LOG_ERROR(ERROR_CRITICAL,ERROR_IP_ADDRESS_UNSPECIFIED);
	}
	
	if(NULL == argv[2])
	{
		A_TRACE(NOTRACE,("\nNO PORT ADDRESS found\n"));
		
		LOG_ERROR(ERROR_CRITICAL,ERROR_PORT_UNSPECIFIED);
	}

	ret = socket_create(&socket_desp,&server_address,argv[1],argv[2]);
	
	if(FAILURE == ret)
	{
		A_TRACE(NOTRACE,("\nFAILURE in creating a socket and exiting the user\n"));
		exit(0);
	}
	
    while(1)
    {

	while(1 > ch || 3 < ch)
	{
		ch = user_choice();
	}

	mapping(ch,&choice);

	switch(choice)
	{
		case 'l':	A_TRACE(NOTRACE,("\n*******Login*******:\n"));
				
				ret = get_user_info_login(&data,socket_desp,choice);

				if(ret == FAILURE)
				{
					
					LOG_ERROR(ERROR_MINOR,ERROR_LOGIN_FAILURE);
					
					free_memory(&data);
				}
				
				flag = 1;
				
				break;

		case 'r': 	A_TRACE(NOTRACE,("\n*******Register*******\n"));

				ret = get_user_info_register(&data,socket_desp,choice);
				
				if(RETRY == ret)
				{
					LOG_ERROR(ERROR_MAJOR,ERROR_REGISTER_FAILURE);
					
					ch = 0;
					
				}
				if(LOGIN == ret)
				{
					ch = 1;
					
					A_TRACE(DETAIL,("\nmaking ch to login ch = %d\n",ch));
				}
	
				break;
	
		case 'e': 	A_TRACE(NOTRACE,("\nuser entered for exiting application\n"));

				flag = 1;
	}
	if(1 == flag)
	{
		exit_user(socket_desp);
	
	}

    }
	return SUCCESS;
}

