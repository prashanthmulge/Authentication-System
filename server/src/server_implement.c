/*******************************************************************************
* 	FILE NAME	: server_implement.c
*
* 	DESCRIPTION	: Contains all implementation functions on server side
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
				HEADER FILES
*******************************************************************************/


#include "server_header.h"
#include "server_error_header.h"

/***********************************************************************
*
*       Function Name : connection_status
*
*       Description : It performs accept functionality and displays connected client details
*
*       Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT connection_status(
			IN MY_INT *connfd, //connection descriptor  
			IN MY_INT sd,  // socket descriptor
			IN_OUT char *buf,  // buffer to store client address
			IN struct sockaddr_in *client_addr // client address
		)
{
	MY_INT len = sizeof(*client_addr);	
		
	memset(client_addr, 0, sizeof(struct sockaddr_in));

	*connfd = accept(sd, (struct sockaddr*)client_addr, &len);

	if(-1 == *connfd)
	{

		LOG_ERROR(ERROR_MAJOR, ERROR_ACCEPT);

		return FAILURE;

	}

	buf = inet_ntoa(client_addr->sin_addr);

	A_TRACE(DETAIL,("\ngot connection from %s\n", buf));

	return SUCCESS;
}

/***********************************************************************
*
*       Function Name : Socket_create
*
*       Description : It create socket and connect to the server
*
*       Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT socket_create(
                        IN MY_INT *sd,                     //socket descriptor
                        IN struct sockaddr_in *server_addr //server address
                 )
{

        A_TRACE(BRIEF,("\nentered socket_create..\n"));

        *sd = socket(AF_INET,SOCK_STREAM,0);

        if(0 > *sd)
        {
                LOG_ERROR(ERROR_MAJOR,ERROR_CREAT_SOCKET);

                return FAILURE;
        }

	A_TRACE(DETAIL,("\nserver socket created..\n"));

	memset(server_addr, 0, sizeof(struct sockaddr_in));

        (*server_addr).sin_family = AF_INET;

        (*server_addr).sin_port = htons(MY_PORT);//PORT NO

        (*server_addr).sin_addr.s_addr = htonl(INADDR_ANY);//ADDRESS

        return SUCCESS;

}


/***********************************************************************
*
*       Function Name : sigint_handler
*
*       Description : It will release resources of server process when ever 
		      server interrupted with SIGINT
*
*       Return Values : SUCCESS or FAILURE
*
***********************************************************************/

void sigint_handler(
			IN MY_INT val
		   )
{

	A_TRACE(NOTRACE,("\nserver exiting..\n"));

	exit(0);

}

/***********************************************************************
*
*       Function Name : read_from_file
*
*       Description : It will read contents from text file
*
*       Return Values : nothing
*
***********************************************************************/

void read_from_file(
			IN_OUT char *buf, // buffer to read contents from file 
			IN FILE *p_file_to_read // file pointer
		   )
{

	A_TRACE(BRIEF,("\nreading from file..\n"));

	fgets(buf,MAX_BUF_SERVER,p_file_to_read);
	
	A_TRACE(DETAIL,("\nfile content------------>%s\n",buf));


}

/***********************************************************************
*
*       Function Name : read_file_write_to_client
*
*       Description : It reads file contents and write to authenticated users 
*
*       Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT read_file_write_to_client(
				IN FILE *p_file_to_read, // file pointer
				IN_OUT char *buf, // buffer
				IN MY_INT connfd  // connection descriptor
			     )
{


	A_TRACE(BRIEF,("\nentered read_file_write_to_client..\n"));

	while(1)
	{
		
		memset(buf, 0, MAX_BUF_SERVER); 

		read_from_file(buf,p_file_to_read);

		if(feof(p_file_to_read))
		{

			A_TRACE(DETAIL,("\nend of read file hits..\n"));

			write_to_client(connfd,buf,ACK);

			break;

		}

		if( FAILURE == write_to_client(connfd,buf,FILE_CONTENTS))
	
			return FAILURE;

	}

	return SUCCESS;

}

/***********************************************************************
*
*       Function Name : free_memory
*
*       Description : It free up the memory of user_detail structure
*
*       Return Values : nothing
*
***********************************************************************/

void free_memory(
			IN user_details *uname_pwd  // structure of user credentials
	      )
{
	
	free(uname_pwd->username);

	free(uname_pwd->password);

	free(uname_pwd);

	A_TRACE(DETAIL,("\nmemory free done in exit_user function..\n"));

}

/***********************************************************************
*
*       Function Name : exit_user
*
*       Description : It exits client by closing client descriptor
*
*       Return Values : nothing
*
***********************************************************************/

void exit_user(
			IN MY_INT connfd,  // connection descriptor
			IN user_details *uname_pwd  // structure of user credentials
	      )
{
	
	A_TRACE(BRIEF,("\nexit_user hits..\n"));

	close(connfd);
	
	free_memory(uname_pwd);

	A_TRACE(DETAIL,("\nmemory free done in exit_user function..\n"));

	exit(0);

}


/***********************************************************************
*
*       Function Name : decision_on_return_status
*
*       Description : It controls the decisions based on return values from login and registration
*
*       Return Values : nothing
*
***********************************************************************/

void decision_on_return_status(
					IN MY_INT ret,  // return value of various functions
					IN char choice,  // user choice
					IN_OUT char *buf,  // buffer for read-write purpose
					IN user_details *uname_pwd,  // structure of user credentials
					IN MY_INT connfd   // connection descriptor
			       )
{

	FILE *p_file_to_read = NULL;

	MY_INT ret_read;

	A_TRACE(BRIEF,("\nentered decision_on_return_status..\n"));

	if(ret == SUCCESS && 'l' == tolower(choice))
	{

		memset(buf,0,MAX_BUF_SERVER);

		A_TRACE(DETAIL,("\nhits login success case..\n"));

		if (FAILURE == open_file(&p_file_to_read,ret))

			A_TRACE(NOTRACE,("\nopen_file failed..\n"));

		if (FAILURE == read_file_write_to_client(p_file_to_read,buf,connfd))

			A_TRACE(NOTRACE,("\nread_file_write_to_client failed..\n"));

		if (FAILURE == close_file(p_file_to_read))

			A_TRACE(NOTRACE,("\nclose_file failed..\n"));

		exit_user(connfd, uname_pwd);

	}

	if(ret == FAILURE && 'l' == tolower(choice))
	{

		A_TRACE(DETAIL,("\nhits login failure case..\n"));

		strcpy(buf,"\n\nauthentication failed!!!!!!!\n\n");

		A_TRACE(DETAIL,("\nwriting_to_client..\n"));

		if (FAILURE == write_to_client(connfd,buf,ACK))

			A_TRACE(NOTRACE,("\nunable to write to client..\n"));

		A_TRACE(DETAIL,("\nfreeing up memory in login failure case..\n"));

		exit_user(connfd, uname_pwd);

	}

	if('r' == tolower(choice))
	{

		A_TRACE(DETAIL,("\nhits registration..\n"));

		if(ret == SUCCESS)

			strcpy(buf,"success");
		else

			strcpy(buf,"failure");

		if(FAILURE == write_to_client(connfd,buf,ACK))
		{
		
			A_TRACE(NOTRACE,("\nunable to write to client..\n"));

			exit_user(connfd,uname_pwd);

		}

		memset(buf, 0, MAX_BUF_SERVER);
	
		A_TRACE(DETAIL,("\nfreeing up memory in reg-failure case..\n"));

		free_memory(uname_pwd);

		A_TRACE(DETAIL,("\nbefore read in reg failure case..\n"));

		if (FAILURE == (ret_read = read_from_client(connfd,buf)))
		{

			A_TRACE(NOTRACE,("\nuser exiting..\n"));

			close(connfd);
		
			exit(0);

		}

		A_TRACE(DETAIL,("after read in reg failure case..buf =%s\n",buf));


		if( FAILURE == tokenisation(buf,&choice,&uname_pwd))
		{

			A_TRACE(NOTRACE,("\ntokenisation failure..\n"));
	
			exit_user(connfd, uname_pwd);

		}
		

		if(ret == SUCCESS)
		{
			memset(buf,0,MAX_BUF_SERVER);

			if(FAILURE == store_details(uname_pwd))
			{
				
				A_TRACE(NOTRACE,("\nstore_details failure..\n"));

				strcpy(buf,"failure");

				write_to_client(connfd,buf,ACK);
				
				exit_user(connfd, uname_pwd);

			}

			strcpy(buf,"success");

			write_to_client(connfd,buf,ACK);
			
			free_memory(uname_pwd);

			A_TRACE(DETAIL,("\nafter reg successfull waiting for login..\n"));

			if (FAILURE == (ret_read = read_from_client(connfd,buf)))
			{

				A_TRACE(NOTRACE,("\nuser exiting..\n"));

				close(connfd);
		
				exit(0);

			}

			A_TRACE(DETAIL,("after reading from client in reg success..buf =%s\n",buf));

			if( FAILURE == tokenisation(buf,&choice,&uname_pwd))
			{

				A_TRACE(NOTRACE,("\ntokenisation failure..\n"));
	
				exit_user(connfd, uname_pwd);

			}
				

		}

		A_TRACE(NOTRACE,("\nprocessing after registration failure or success..\n"));

		ret = choice_of_login_register_store(choice,uname_pwd);

		decision_on_return_status(ret,choice,buf,uname_pwd,connfd);

	}


}

/***********************************************************************
*
*       Function Name : tokenisation
*
*       Description : It will tokenise the user given credentials 
*
*       Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT tokenisation(
			IN char *buf,  // buffer containing username password
			IN char *choice,  // user choice
			IN_OUT user_details **uname_pwd  // structure to user credentials
		)
{

	char *token=NULL;

	A_TRACE(BRIEF,("\nentered tokenisation..\n"));

	if(NULL == (*uname_pwd = (user_details*)malloc(sizeof(user_details))))
	{

		LOG_ERROR(ERROR_MAJOR,ERROR_MALLOC_FAILURE);

		return FAILURE;

	}

	if(NULL == ((*uname_pwd)->username = (char*)malloc(sizeof(char)*MAX_UNAMEPWD)))
	{

		LOG_ERROR(ERROR_MAJOR,ERROR_MALLOC_FAILURE);

		return FAILURE;

	}

	if(NULL == ((*uname_pwd)->password = (char*)malloc(sizeof(char)*MAX_UNAMEPWD)))
	{
		
		LOG_ERROR(ERROR_MAJOR,ERROR_MALLOC_FAILURE);

		return FAILURE;

	}

	A_TRACE(DETAIL,("\nbuf-tokenisation=%s\n",buf));
	
	token = strtok(buf, ";");

	*choice = token[0];

	A_TRACE(DETAIL,("\nchoice:%c\n",*choice));

	token = strtok(NULL,";");

	strcpy((*uname_pwd)->username,token); 

	A_TRACE(DETAIL,("\nusername:%s\n",(*uname_pwd)->username));

	token = strtok(NULL,";");
		
	strcpy((*uname_pwd)->password,token);

	A_TRACE(DETAIL,("\npassword:%s\n",(*uname_pwd)->password));

	A_TRACE(DETAIL,("\nat d end of tokenisation..\n"));

	return SUCCESS;

}

/***********************************************************************
*
*       Function Name : choice_of_login_register_store
*
*       Description : It switches between login,register based on users choice
*
*       Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT choice_of_login_register_store(
					IN char choice,// user choice
					IN user_details *uname_pwd  // structure to user credentials
				   )
{

	MY_INT ret;

	A_TRACE(BRIEF,("\nentered choice_of_lrs..\n"));

	if('l' == tolower(choice))
	{

		ret = login_verification(uname_pwd,choice);

	}


	else
	{

		ret = registration(uname_pwd,choice);

	}

	return ret;

}

/***********************************************************************
*
*       Function Name : login_verification
*
*       Description : It authenticates client given username and password 
		      with data base file credentials
*
*       Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT login_verification(
				IN user_details *uname_pwd,  // structure for user credentials
				IN char choice  // user choice
		      )
{

	MY_INT ret_search;

	A_TRACE(BRIEF,("\nentered login_verfication..\n"));

	if (FAILURE == (ret_search = file_search(uname_pwd,choice)))
	{

		A_TRACE(NOTRACE,("\nauthentication failed..\n"));

		return FAILURE;

	}

	return SUCCESS;	

}

/***********************************************************************
*
*       Function Name : registration
*
*       Description : It registers given username by checking its existence
*
*       Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT registration(
			IN user_details *uname_pwd, // structure to user credentials
			IN char choice  // user choice
		)
{

	MY_INT ret_search;

	A_TRACE(BRIEF,("\nentered registration..\n"));

	if(FAILURE == (ret_search = file_search(uname_pwd,choice)))
	{

		A_TRACE(NOTRACE,("\nregistration success..\n"));

		return SUCCESS;

	}

	return FAILURE;

}

/***********************************************************************
*
*       Function Name : file_search
*
*       Description : It will search data base file for username and password existence
*
*       Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT file_search(
			IN user_details *client_uname_pwd,  // structure to user credentials
			IN char choice  // user choice
	       )
{

	FILE *p_db_file = NULL;

	MY_INT ret = FAILURE;

	char file_uname[MAX_UNAMEPWD];

	char file_pwd[MAX_UNAMEPWD];

	A_TRACE(BRIEF,("\nentered file_search..\n"));

	if (SUCCESS == open_file(&p_db_file,ret))
	{

		A_TRACE(DETAIL,("\nfile open success..\n"));

	}

	memset(file_uname,0,MAX_UNAMEPWD);

	memset(file_pwd,0,MAX_UNAMEPWD);

	if (FAILURE == compare_username(client_uname_pwd->username,file_uname,file_pwd,p_db_file))
	{

		A_TRACE(BRIEF,("\nusername not exist..\n"));

		if (SUCCESS == close_file(p_db_file))
		{

			A_TRACE(DETAIL,("\nfile closing success..\n"));

		}

		return FAILURE;

	}


	if('l' == tolower(choice))
	{

		A_TRACE(BRIEF,("\nentered login case in file search..\n"));

		A_TRACE(DETAIL,("\nfile_uname,cli_pwd , file_pwd=%s %s %s\n",file_uname,client_uname_pwd->password,file_pwd));

		A_TRACE(DETAIL,("\nbefore dec-->client_pwd=%s,file_pwd=%s\n",client_uname_pwd->password,file_pwd));

		decrypt_password(client_uname_pwd->password);

		decrypt_password(file_pwd);

		A_TRACE(DETAIL,("\nclient_pwd=%s,file_pwd=%s\n",client_uname_pwd->password,file_pwd));

		if (FAILURE == compare_password(client_uname_pwd->password,file_pwd))
		{

			A_TRACE(BRIEF,("\npassword not exist..\n"));
		
			if (SUCCESS == close_file(p_db_file))
			{

				A_TRACE(DETAIL,("\nfile closing success\n"));

			}

			return FAILURE;

		}

	}

	if (SUCCESS == close_file(p_db_file))
	{

		A_TRACE(DETAIL,("\nfile closing success..\n"));

	}

	return SUCCESS;	

}

/***********************************************************************
*
*       Function Name : decrypt_password
*
*       Description : It will decrypt the given encrypted password to plain text password
*
*       Return Values : SUCCESS or FAILURE
*
***********************************************************************/

void decrypt_password(
			IN_OUT char *dec_pwd  // encrypted password
		     )
{

        MY_INT index = 0,random_no = 0;

	char passwd[MAX_BUF_SERVER],random_char[MAX_BUF_SERVER],*token;

	strcpy(passwd,(token = strtok(dec_pwd,"|")));
	
	strcpy(random_char,(token = strtok(NULL,"|")));

	A_TRACE(BRIEF,("\nentered decrypt_password..\n"));

	random_no = atoi(random_char);

	memset(dec_pwd,0,MAX_UNAMEPWD);

        while(passwd[index] != '\0')
        {

                dec_pwd[index] = passwd[index] - random_no;

                index++;

        }
	A_TRACE(DETAIL,("\ndec-pwd=%s\n",dec_pwd));
}

/***********************************************************************
*
*       Function Name : compare_username
*
*       Description : It will compare client given username with username in data base file
*
*       Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT compare_username(
			IN char *client_uname,  //username which user has entered
			IN char *file_uname, // username mapped from database
			IN char *file_pwd,  // password mapped from database
			IN FILE *p_db_file   // pointer to database file
		    )
{

	char buf[MAX_BUF_SERVER];

	MY_INT flag=0;

	A_TRACE(BRIEF,("\nentered compare_username..\n"));

	memset(buf,0,MAX_BUF_SERVER);

	while(!feof(p_db_file))
	{

		A_TRACE(DETAIL,("\ninside while of compare username..\n"));

		if(EOF == fscanf(p_db_file,"%s",buf))

			break;

		A_TRACE(DETAIL,("file buf =%s\n",buf));

		strcpy(file_uname,strtok(buf,";"));

		A_TRACE(DETAIL,("\nusername(strcpy) in compare_uname=%s\n",file_uname));	

		if(!strcmp(file_uname,client_uname))
		{

			flag=1;

			strcpy(file_pwd,strtok(NULL,";"));

			A_TRACE(DETAIL,("\npasswd in compare_username=%s\n",file_pwd));

			return SUCCESS;

		}

	}

	A_TRACE(DETAIL,("\nout of while in compare_username..\n"));

	return FAILURE;

}

/***********************************************************************
*
*       Function Name : compare_password
*
*       Description : It will compare client given password with existing data base password
*
*       Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT compare_password(
			IN char *client_pwd, // password given by user
			IN char *file_pwd  // password mapped from database
		    )
{

	A_TRACE(BRIEF,("\nentered compare_pwd..\n"));

	A_TRACE(DETAIL,("\nclient_pwd,file_pwd=%s %s\n",client_pwd,file_pwd));

	if(!strcmp(client_pwd, file_pwd))

		return SUCCESS;

	return FAILURE;

}

/***********************************************************************
*
*       Function Name : read_from_client
*
*       Description : It reads the client input using client socket descriptor
*
*       Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT read_from_client(
				IN MY_INT connfd, // connection descriptor
				IN_OUT char* buf // buffer to read contents to
		    )
{

	memset(buf, 0, MAX_BUF_SERVER);

	char ch = '\0';

	MY_INT ret=1, index=0,flag=0;

	A_TRACE(BRIEF,("\nentered read_from_client..\n"));
	
	while((index < MAX_BUF_SERVER) && ((ret = read(connfd, &ch, 1)) > 0))
	{

		if(0 > ret)
		{

			LOG_ERROR(ERROR_MAJOR,ERROR_READ_SOCKET);

			return FAILURE;

		} 

		if('\0' == ch)
		{

			flag = 1;

			break;

		}

		buf[index] = ch;

		index++;

	}

	A_TRACE(DETAIL,("\nret & flag of read_from_client =%d %d\n",ret,flag));

	buf[index] = '\0';

	if(1 == flag)

		return SUCCESS;

	else

		return FAILURE; 
	
}

/***********************************************************************
*
*       Function Name : write_to_client
*
*       Description : It will write buffer content to client using client socket descriptor 
*
*       Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT write_to_client(
				IN MY_INT connfd, // connection descriptor
				IN char* buf,  // buffer containing data to be written
				IN MY_INT flag  //ACK
		    )
{

	A_TRACE(BRIEF,("\nentered write_to_client..\n"));

	MY_INT n = strlen(buf), len = 0, ret = 0, buf_len=0;

	char *temp_buf = NULL;
	
	if(NULL == (temp_buf = (char*)malloc(sizeof(char)*(strlen(buf)+1))))
	{

		LOG_ERROR(ERROR_MAJOR,ERROR_MALLOC_FAILURE);

		return FAILURE;
	
	}
	
	A_TRACE(DETAIL,("\nbuf inside write_to_client=%s\n",buf));

	buf_len = strlen(buf);

	strcpy(temp_buf, buf);
	
	if(ACK != flag)
	{
		temp_buf[buf_len-1] = ' ';

		temp_buf[buf_len] = '\n';

	}

		
	while(len <= n)
	{

		ret = write(connfd, temp_buf+len, (n+1)-len);

		if(ret < 0)
		{

			LOG_ERROR(ERROR_MAJOR,ERROR_WRITE_SOCKET);

			return FAILURE;

		}

		len += ret;

	}

	free(temp_buf);

	A_TRACE(DETAIL,("\nout of while in write_to_client..\n"));

	return SUCCESS;

}

/***********************************************************************
*
*       Function Name : open_file
*
*       Description : It will open database file/text file 
*
*       Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT open_file(
			IN FILE **p_file, // file pointer
			IN_OUT MY_INT ret // return sataus
	     )
{

	A_TRACE(BRIEF,("\nentered open_file..\n"));

	if(ret == FAILURE)
	{

		if((NULL == (*p_file = fopen("../src/db_file_user.txt", "a+"))))
		{

			LOG_ERROR(ERROR_MINOR,ERROR_OPEN_FILE);

			return FAILURE;

		}

	}
	else
	{

		if((NULL == (*p_file = fopen("../src/file_to_read.txt", "a+"))))
		{

			LOG_ERROR(ERROR_MINOR,ERROR_OPEN_FILE);
		
			return FAILURE;

		}

		
	}

	return SUCCESS;

}

/***********************************************************************
*
*       Function Name : close_file
*
*       Description : It will close the file
*
*       Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT close_file(
			IN FILE *p_file // file pointer
	      )
{

	A_TRACE(BRIEF,("\nentered close_file..\n"));

	if(0 != fclose(p_file))
	{
	
		A_TRACE(DETAIL,("\nfclose not equal to zero..\n"));

		LOG_ERROR(ERROR_MINOR,ERROR_CLOSE_FILE);

		return FAILURE;

	}

	return SUCCESS;

}

/***********************************************************************
*
*       Function Name : write_to_file
*
*       Description : It will write username and password into data base file
*
*       Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT write_to_file(
			IN user_details *uname_pwd, // structure to user credentials
			IN FILE *p_db_file  // pointer to database file
		 )
{

	A_TRACE(BRIEF,("\nentered write_to_file..\n"));

	if(0 > fprintf(p_db_file, "%s;%s\n", uname_pwd->username, uname_pwd->password))
	{

		LOG_ERROR(ERROR_MAJOR,ERROR_WRITE_FILE);

		return FAILURE;

	}

	return SUCCESS;

}


/***********************************************************************
*
*       Function Name : store_details
*
*       Description : It will store username and password into data base file
*
*       Return Values : SUCCESS or FAILURE
*
***********************************************************************/

MY_INT store_details(
			IN user_details *uname_pwd // structure to user credentials
		 )
{
	FILE *p_db_file=NULL;

	MY_INT ret;

	A_TRACE(BRIEF,("\nentered store_details..\n"));

	if (FAILURE == (ret = open_file(&p_db_file,FAILURE)))
	{

		A_TRACE(DETAIL,("\nfile open failure..\n"));
		
		return FAILURE;

	}

	if (FAILURE == (ret = write_to_file(uname_pwd, p_db_file)))
	{

		A_TRACE(DETAIL,("\nfile writing failure..\n"));

		return FAILURE;

	}

	if (FAILURE == (ret = close_file(p_db_file)))
	{

		A_TRACE(DETAIL,("\nfile closing failure..\n"));

		return FAILURE;

	}

	return SUCCESS;
}
