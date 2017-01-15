/*******************************************************************************
* 	FILE NAME	: client_implement.c
*
* 	DESCRIPTION	: Functions for all client service
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

/***********************************************************************
*		Header
***********************************************************************/
#include "client_header.h"

#include "client_error_header.h"	
/***********************************************************************
*
*	Function Name :	mapping 
*	
*	Description :	It maps the int choice to char
*
*	Return Values :	void
*
***********************************************************************/

void mapping(
		IN MY_INT ch,				//numeric choice
		IN_OUT char *choice			//alpha choice
	    )
{
	A_TRACE(BRIEF,("\nentering in mapping\n"));
	
	if(ch == 1)
		*choice = 'l';
	else if(ch == 2)
		*choice = 'r';
	else
		*choice = 'e';

}
		
/***********************************************************************
*
*	Function Name :	user_choice
*
*	Description :	it gives choice for user
*
*	Return Values :	Choice in character
*
***********************************************************************/

MY_INT user_choice()
{
	
	MY_INT choice = 0, check = 0;

	A_TRACE(BRIEF,("\nentering user choice function\n"));
	
	A_TRACE(NOTRACE,("\nEnter your choice\n"));

	A_TRACE(NOTRACE,("\n1 - Login \n2 - Register \n3 - Exit\n:"));

	fflush(stdout);
	
	fscanf(stdin,"%d",&choice);

	while(getchar() != '\n')
	{
		check++;
		
		if(1 == check)
		{
			choice = 0;
		}
	}

	if(3 < choice || 0 >= choice)
	{
		
		LOG_ERROR(ERROR_MINOR,ERROR_CHOICE_SELECT);		
	
	}
	
	return choice;
}

/***********************************************************************
*
*	Function Name : get_user_info_login
*
*	Description :	It takes user for login and authenticate
*
*	Return Values :	SUCCESS or FAILURE
*
***********************************************************************/

MY_INT get_user_info_login(
			IN_OUT user_info **data,	//user data structure
			IN MY_INT sd,		//socket descriptor
			IN char ch		//choice
		       )
{

	MY_INT ret;

	A_TRACE(BRIEF,("\nentering login function\n"));

	*data = (user_info*)malloc(sizeof(user_info));
	
	if( NULL == *data )
	{
		LOG_ERROR(ERROR_MAJOR,ERROR_MALLOC_FAILURE);		
	}
	
	(*data)->username = (char *)malloc(MAX_UNAMEPWD);

	if( NULL == (*data)->username )
	{
		LOG_ERROR(ERROR_MAJOR,ERROR_MALLOC_FAILURE);		
	}
	
	while(SUCCESS != (ret = get_username(data)));

	(*data)->password = (char *)malloc(MAX_UNAMEPWD);
	
	if( NULL == (*data)->password )
	{
		LOG_ERROR(ERROR_MAJOR,ERROR_MALLOC_FAILURE);		
	}

	while(SUCCESS != (ret = get_password(data)));

	encrypt_password(data);

	ret = write_to_server(sd,*data,ch);

	if(FAILURE == ret)
	{
		return FAILURE;
	}

	ret = read_from_server(sd,ch);	

	if(FAILURE == ret)
	{
		return FAILURE;
	}
		
	free_memory(data);
	
	return SUCCESS;
	
}

/***********************************************************************
*
*	Function Name : get_username
*
*	Description :	It get the username from user
*
*	Return Values :	SUCCESS or FAILURE
*
***********************************************************************/

MY_INT get_username(
			IN_OUT user_info **data	//user data structure
		)
{
	MY_INT len = 0,index;
	
	char temp_buf[MAX_UNAMEPWD+2];	

	memset(temp_buf,'\0',MAX_UNAMEPWD+2);
	
	A_TRACE(BRIEF,("\nentering Username function\n"));

	A_TRACE(NOTRACE,("\nUSERNAME:"));
	
	fgets(temp_buf, MAX_UNAMEPWD+2, stdin);

	if('\0' != temp_buf[MAX_UNAMEPWD])
	{
		if('\n' != temp_buf[MAX_UNAMEPWD])
		{

			A_TRACE(NOTRACE,("\nusername length exceeds..\n"));

			while(getchar()!='\n');

			return FAILURE;
		}

	}

	if('\0' == temp_buf[1])
	{

		A_TRACE(NOTRACE,("\nempty string not allowed..\n"));

		return FAILURE; 
	}

	A_TRACE(DETAIL,("\ntemp_buf=%s\n",temp_buf));

	len = strlen(temp_buf);

	temp_buf[len-1] = '\0';
	
	for(index = 0; index < len-1 ;index++)
	{


		if(0 != isalnum(temp_buf[index]) || '$' == temp_buf[index] || '@' == temp_buf[index] || '_' == temp_buf[index] || '.' == temp_buf[index] || '*' == temp_buf[index])
		{
			continue;
		}
				A_TRACE(NOTRACE,("\nOnly alphanumeric or {*,@,$,_,.} are permitted ..\n"));

				return FAILURE;
	}
	
 	A_TRACE(DETAIL,("\nusername=%s\n",temp_buf));


 	A_TRACE(DETAIL,("\nbefore string copy..\n"));

	strcpy((*data)->username,temp_buf);

 	A_TRACE(DETAIL,("\nusername=%s\n",(*data)->username));

	return SUCCESS;
}

/***********************************************************************
*
*	Function Name : get_password
*
*	Description :	It takes password from user
*
*	Return Values :	SUCCESS or FAILURE
*
***********************************************************************/

MY_INT get_password(
			IN_OUT user_info **data	//structure object
		 )
{
	MY_INT len = 0, index;

	char temp_buf[MAX_UNAMEPWD+2];
	
	A_TRACE(BRIEF,("\nentering password_function\n"));

	memset(temp_buf,'\0',MAX_UNAMEPWD+2);

	A_TRACE(NOTRACE,("\nPassword:"));

	system("stty -echo");

	fgets(temp_buf, MAX_UNAMEPWD+2, stdin);

	system("stty echo");

	if('\0' != temp_buf[MAX_UNAMEPWD]) 
	{
		if('\n' != temp_buf[MAX_UNAMEPWD])
		{

			A_TRACE(NOTRACE,("\npassword length exceeds..\n"));

			while(getchar()!='\n');

			return FAILURE;
		}
	}
	
	if('\0' == temp_buf[1])
	{

		A_TRACE(NOTRACE,("\nempty string not allowed..\n"));

		return FAILURE; 
	}

	len = strlen(temp_buf);

	temp_buf[len-1] = '\0';
	
	for(index = 0; index < len-1 ;index++)
	{


		if(0 != isalnum(temp_buf[index]) || '$' == temp_buf[index] || '@' == temp_buf[index] || '_' == temp_buf[index] || '.' == temp_buf[index] || '*' == temp_buf[index])
		{
			continue;
		}
				A_TRACE(NOTRACE,("\nOnly alphanumeric or {*,@,$,_,.} are permitted ..\n"));

				return FAILURE;
	}

	strcpy((*data)->password,temp_buf);
	
	return SUCCESS;
}

/***********************************************************************
*
*	Function Name : get_user_info_register 
*
*	Description : It takes Register as choice and authenticate
*
*	Return Values : SUCCESS or FAILURE
*
***********************************************************************/


MY_INT get_user_info_register(
				IN_OUT user_info **data,//user structure
				IN MY_INT sd,		//socket descriptor
				IN char ch		//choice
			   )
{

	MY_INT ack, ret;
	
	char ch_1='s';

	A_TRACE(BRIEF,("\nentering register_function\n"));

	*data = (user_info*)malloc(sizeof(user_info));
	
	if( NULL == *data )
	{
		LOG_ERROR(ERROR_MAJOR,ERROR_MALLOC_FAILURE);		
		
	}

	(*data)->username = (char *)malloc(MAX_UNAMEPWD);
	
	if( NULL == (*data)->username )
	{
		LOG_ERROR(ERROR_MAJOR,ERROR_MALLOC_FAILURE);		

	}
	
	while(SUCCESS != (ret = get_username(data)));

	(*data)->password = (char *)malloc(MAX_UNAMEPWD);
	
	if( NULL == (*data)->password )
	{

		LOG_ERROR(ERROR_MAJOR,ERROR_MALLOC_FAILURE);		
	}
	
	ack = check_username(*data,sd,ch);

	if(SUCCESS == ack)
	{

		while(SUCCESS != (ret = get_password(data)));
		
		encrypt_password(data);

		ret = write_to_server(sd,*data,ch_1);

		if(FAILURE == ret)
		{
			return FAILURE;
		}
		
		ret = read_from_server(sd,ch_1);

		if(FAILURE == ret)
		{
			return FAILURE;
		}
		
		free_memory(data);
		
		return LOGIN;
	}
	else
	{
		free_memory(data);		
	
		A_TRACE(NOTRACE,("\nUsername already exist: Try other username\n"));
	
		return RETRY;
	}

}

/***********************************************************************
*
*	Function Name : Socket_create
*
*	Description : It create socket and connect to the server
*
*	Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT socket_create(	
			IN MY_INT *sd,			 	// socket descriptor
			IN struct sockaddr_in *server_addr, 	// server address
			IN char * ip_address, 			// Ip address of server
			IN char * port_no			// Port number of server
		 )
{

	A_TRACE(BRIEF,("\nentering socket_create function\n"));
	
	*sd = socket(AF_INET,SOCK_STREAM,0);

	if(-1 == *sd)
	{

		LOG_ERROR(ERROR_MAJOR,ERROR_CREAT_SOCKET);	
			
		return FAILURE;
	}

	memset(server_addr,0,sizeof(server_addr));

	(*server_addr).sin_family = AF_INET;
	
	(*server_addr).sin_port = htons( atoi(port_no) );//PORT NO
	
	(*server_addr).sin_addr.s_addr = inet_addr(ip_address); //ADDRESS
	
	if (connect(*sd,(struct sockaddr*)server_addr,
					sizeof(*server_addr))<0)
	{
         	
		LOG_ERROR(ERROR_MAJOR,ERROR_CONNECT_SOCKET);		

		close(*sd);		

        	return FAILURE;
	}
	

	return SUCCESS;

}


/***********************************************************************
*
*	Function Name : check_username
*
*	Description : It checks whether username is available or not for registering
*
*	Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT check_username(
			IN_OUT user_info *user,	//user structure
			IN MY_INT sd,			//socket descriptor
			IN char ch			//choice
		  )
{

	MY_INT ret;	

	char data[MAX_DATA];

	A_TRACE(BRIEF,("\nentering check username\n"));
	
	ret = write_to_server(sd,user,ch);

	if(FAILURE == ret)
	{
		return FAILURE;
	}
	
	memset(data,'\0',MAX_DATA);
	
	ret = read_from_server(sd,ch);
	
	if(FAILURE == ret)
	{
		return FAILURE;	
	}

		
	return SUCCESS;
}

/***********************************************************************
*
*	Function Name : encrypt_password
*
*	Description : It encrypts the password
*
*	Return Values :SUCCESS
*
***********************************************************************/

MY_INT encrypt_password(
			IN_OUT user_info **data	//user structure
		    )
{
	
	MY_INT i=0, random_no = 0,flag = 0;

	char random_char[MAX_DATA];

	random_no = rand() % 1000;

	A_TRACE(BRIEF,("\nentering encrypt password\n"));
	
	A_TRACE(DETAIL,("\nthe random no is %d\n",random_no));
	
	while((*data)->password[i] != '\0')
	{
	       	if( ';' == (*data)->password[i] || '|' == (*data)->password[i] )
		{
			flag = 1;
		}
	 
		(*data)->password[i] = (*data)->password[i]+random_no;
	
	        i++;
	}
	if(1 == flag)
	{
		while((*data)->password[i] != '\0')
 		{

         		(*data)->password[i] = (*data)->password[i] + MORPH_ENCRYPT;

         		i++;
 		}
	}
	
	random_no += MORPH_ENCRYPT;

	strcat((*data)->password,"|");

	sprintf(random_char,"%d",random_no);

	A_TRACE(DETAIL,("\nencypt = %s random = %s\n ",(*data)->password,random_char));

	strcat( (*data)->password,random_char );

	A_TRACE(DETAIL,("\nThe sending password is %s\n",(*data)->password));
	
	return SUCCESS;
}

/***********************************************************************
*
*	Function Name : read_from_server
*
*	Description : It reads data from server
*
*	Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT read_from_server(
			IN MY_INT sd,	//socket descriptor
			IN char ch	//choice
		    )

{

	char buf[MAX_DATA];

        char byte_data = '\0';

        MY_INT ret =0, index=0; 
	
	A_TRACE(BRIEF,("\nentering read from server\n"));

	memset(buf, '\0', MAX_DATA);

	while( (ret = read(sd, &byte_data, 1)) > 0)
        {
                if('\0' == byte_data)
                {
			//A_TRACE(DETAIL,("\nentered end of string condition and hitting break..\n"));
                        break;
                }
		
		if('l' == ch)
		{
        	        A_TRACE(NOTRACE,("%c",byte_data));
		}
		else
		{
			buf[index] = tolower(byte_data);
			index++;
		}
        }
	if('r' == ch || 's' == ch)
	{
		if(0 == strcmp(buf,"success"))
		{
			return SUCCESS;
		}
		else
		{
			return FAILURE;
		}
	}
	
	if(ret < 0)
	{
		
		LOG_ERROR(ERROR_MAJOR,ERROR_READ_FILE);		
	}

	return SUCCESS;
}
 

/***********************************************************************
*
*	Function Name : write_to_server
*
*	Description : It writes the user data to server
*
*	Return Values : SUCCESS or FAILURE
*
***********************************************************************/


MY_INT write_to_server(	
			IN MY_INT sd,			//socket descriptor 
			IN_OUT user_info *user_details,	//user structure
			IN char ch			//choice
		    )
{

        MY_INT n, len = 0, ret = 0; 
	
	char buf[MAX_DATA];

	A_TRACE(BRIEF,("\nentering write to server\n"));
	
	memset(buf,'\0',MAX_DATA);
	
	if('l' == ch)
	{
		strcpy(buf,"l;");
	}
	
	if('s' == ch)
	{
		strcpy(buf,"s;");
	}
	
	if('r' == ch)
	{
		strcpy(buf,"r;");

		strcat(buf,user_details->username);

		strcat(buf,";XXXX");
	}
	else
	{
		strcat(buf,user_details->username);
		
		strcat(buf,";");

		strcat(buf,user_details->password);
	
	}
	
	n = strlen(buf)+1;

	buf[n-1] = '\0';

	A_TRACE(DETAIL,("\nthe sending data to server is %s\n",buf));

        while(len < n)
        {

                ret = write(sd, buf+len, n-len);

                if(ret < 0)
                {

			LOG_ERROR(ERROR_MAJOR,ERROR_WRITE_SOCKET);		

                        //exit(1);

			return FAILURE;

                }

                len += ret;
        }
	

	return SUCCESS;
}


/***********************************************************************
*
*	Function Name : exit_user
*
*	Description : It shutdown the socket and closes it
*
*	Return Values : void
*
***********************************************************************/

void exit_user(
		IN MY_INT sd	//socket descriptor
	      )
{
	A_TRACE(BRIEF,("\nentering exiting the user\n"));

	//shutdown(sd,SHUT_RDWR);

	close(sd);

	exit(0);

}



/***********************************************************************
*
*	Function Name : free_memory
*
*	Description : It frees up the memory allocated dynamically
*
*	Return Values : void
*
***********************************************************************/


void free_memory(
			IN_OUT user_info ** user_details //user structure
		)
{
	A_TRACE(BRIEF,("\nentering free the memory\n"));

	free((*user_details)->username);

	free((*user_details)->password);

	free(*user_details);

}
