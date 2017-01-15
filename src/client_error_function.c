/*******************************************************************************
* 	FILE NAME	: error_function.c
*
* 	DESCRIPTION	: contains the error function declaration
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

#include "client_error_header.h"
#include "client_header.h"


/*******************************************************************************
				GLOBAL DECALARATIONS 
*******************************************************************************/

int g_error_level = ERROR_ALL;

char *err_str[] = {
			"ERROR IN MEMORY ALLOCATION\n",
		  	"ERROR IN READING A SOCKET\n",
			"ERROR IN WRITING A SOCKET\n",
			"ERROR IN CREATING SOCKET\n",
			"ERROR IN CONNECTING SOCKET\n",
			"ERROR IN OPENING FILE\n",
			"ERROR IN CLOSING OF FILE\n",
			"IP ADDRESS NOT MENTIONED\n",
			"PORT NO NOT MENTIONED\n",
			"USER LOGIN FAILURE\n",
			"USER REGISTRATION FAILURE\n",
			"ERROR IN BIND ADDRESS\n",
			"ERROR IN ACCEPT FROM CLIENT\n",
			"ERROR IN LISTEN FROM CLIENT\n",
			"ERROR IN WRITING TO FILE\n"
			"ERROR IN READING FILE\n"
		  };


/*******************************************************************************
*
*	FUNCTION NAME :	error_handler
*	
*	DESCRIPTION   :
*
*	RETURNS	      :
*
*******************************************************************************/

void error_handler( MY_INT error_level,	// level of the error 
		   MY_INT error_code) 	// code corresponding to error 
{

    FILE *p_log_file; 			//file pointer  for log file 
    
    p_log_file = NULL;

    p_log_file = fopen("../src/client_error_log.txt", "a+");	//open the log file 


    if(NULL == p_log_file)		//if unable to open log file
    
    {
	
	A_TRACE(BRIEF,("\nError opening log file\n"));
	
	exit(FAILURE);

    }
	
    if(error_level <= g_error_level)
    {
	switch(error_code)
	{

		case 0:  A_TRACE(NOTRACE,("\nmalloc failure..\n"));
			 print_date_time(p_log_file);
			 fputs(err_str[0], p_log_file);
			 fclose(p_log_file);
			 exit(FAILURE);

		case 1:  A_TRACE(NOTRACE,("\nError in reading socket\n"));
			 print_date_time(p_log_file);
			 fputs(err_str[1], p_log_file);
			 fclose(p_log_file);
			 break;             	
 
	        case 2:  A_TRACE(NOTRACE,("\nError in writing socket\n"));
                         print_date_time(p_log_file);
                         fputs(err_str[2],p_log_file);
			 fclose(p_log_file);
			 break;

                case 3:  A_TRACE(NOTRACE,("\nError creating socket\n"));
                         print_date_time(p_log_file);
                         fputs(err_str[3],p_log_file);
			 fclose(p_log_file);
			 break;		

                case 4:  A_TRACE(NOTRACE,("\nError in connect\n"));
                         print_date_time(p_log_file);
                         fputs(err_str[4], p_log_file);
			 fclose(p_log_file);
			 break;

                case 5:  A_TRACE(NOTRACE,("\nError in opening file\n"));
                         print_date_time(p_log_file);
                         fputs(err_str[5], p_log_file);
			 fclose(p_log_file);
                         exit(FAILURE);

                case 6:  A_TRACE(NOTRACE,("\nclosing file\n"));
                         print_date_time(p_log_file);
                         fputs(err_str[6], p_log_file);
			 fclose(p_log_file);
                         exit(FAILURE);
        
                case 7:  A_TRACE(NOTRACE,("\nIp address not mentioned\n"));
        		 print_date_time(p_log_file);
        		 fputs(err_str[7], p_log_file);
			 fclose(p_log_file);
        		 exit(FAILURE);

                case 8:  A_TRACE(NOTRACE,("\nPort number not mentioned\n"));
                         print_date_time(p_log_file);
                         fputs(err_str[8], p_log_file);
			 fclose(p_log_file);
                         exit(FAILURE);

                case 9:  A_TRACE(NOTRACE,("\nLogin Failure\n"));
                         print_date_time(p_log_file);
                         fputs(err_str[9], p_log_file);
			 fclose(p_log_file);
                         exit(FAILURE);
                
		case 10: A_TRACE(NOTRACE,("\nRegistration failure\n"));
                         print_date_time(p_log_file);
                         fputs(err_str[10], p_log_file);
			 fclose(p_log_file);
			 break;

                case 11: A_TRACE(NOTRACE,("\nError in bind\n"));
                         print_date_time(p_log_file);
                         fputs(err_str[11], p_log_file);
			 fclose(p_log_file);
                         exit(FAILURE);
 
                case 12: A_TRACE(NOTRACE,("\nError in accept\n"));
                         print_date_time(p_log_file);
                         fputs(err_str[12], p_log_file);
			 fclose(p_log_file);
                         exit(FAILURE);
 
                case 13: A_TRACE(NOTRACE,("\nError in listen\n"));
                         print_date_time(p_log_file);
                         fputs(err_str[13], p_log_file);
			 fclose(p_log_file);
                         exit(FAILURE);

                case 14: A_TRACE(NOTRACE,("\nwriting to file error\n"));
                         print_date_time(p_log_file);
                         fputs(err_str[14], p_log_file);
			 fclose(p_log_file);
                         exit(FAILURE);

                case 15: A_TRACE(NOTRACE,("\nreading from file error\n"));
                         print_date_time(p_log_file);
                         fputs(err_str[15], p_log_file);
			 fclose(p_log_file);
                         exit(FAILURE);
		
		default:
			
			fclose(p_log_file);	
			break;
	}
    }
}


/*******************************************************************************
*       FUNCTION NAME :print_date_time
*       ARGUEMENT     :FILE *
*       RETURN VALUE  :SUCCESS/FAILURE
*
*******************************************************************************/

MY_INT print_date_time(FILE *p_log_file) /* file pointer where date is to be written */
{
	time_t time_val = 0; /* stores the time value */

	char *date_time; /* date in string format */

	time(&time_val);	/*get the current time */

	date_time = ctime(&time_val);

	date_time[strlen(date_time)-1] = ' ';
	
	fputs(date_time, p_log_file);

     return SUCCESS;
}


