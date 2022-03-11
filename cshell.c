
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
 #include <time.h>
 
#define nEv  100
#define nC 100
#define MAX 1020
int Env_ctr=0; int log_ctr=0;

/* def of Env_var */
typedef struct EnvVar{
    char *name;
   char *value;
   
} EnvVar;


/* def of commandlog */
typedef struct Command{
    char *name;
    struct tm time;
    int code;
   
} Command;


void free_Env_ptr(EnvVar *Arr_Env2)
{  //printf("enter env free\n");
	unsigned int i = 0;

	if (Arr_Env2 == NULL)
		return;

	while (i<Env_ctr)
	{
		free(Arr_Env2[i].name);
		free(Arr_Env2[i].value);
		++i;
	}

	

	free(Arr_Env2);
	//printf("all free\n");
}

void free_log_ptr(Command *Arr_log2)
{  //printf("enter log free\n");
	unsigned int i = 0;

	if (Arr_log2 == NULL)
		return;

	while (i<log_ctr)
	{
		free(Arr_log2[i].name);
		++i;
	}

	

	free(Arr_log2);
	//printf("all free\n");
}

void free_all(char* buffer2,EnvVar *Arr_Env2,Command *Arr_log2)
{							if (buffer2!=NULL)
							{
				    		free(buffer2);
  							buffer2 = NULL;
  							}
  							free_Env_ptr(Arr_Env2);
  							free_log_ptr(Arr_log2);
}

void  parse(char *buffer2, char **argv)
{
   while (1) {
        // Eating whitespace 
        while (*buffer2 == ' ' || *buffer2 == '\n'|| *buffer2 == '\t' )
            *buffer2++ = '\0';

        // Checking if there are more command line arguments 
        if (!*buffer2)
          {
            // Terminating `argv` 
            *argv = NULL;
            return;
          }

        // Adding next command line argument to `argv` 
        *argv++ = buffer2;

        while (*buffer2 != '\0' && *buffer2!= ' ' && *buffer2 != '\t' && *buffer2 != '\n')
            buffer2++;
    }
}





void  execute(char **arguments,EnvVar *Arr_Env2,Command *Arr_log2)
{	

	//printf("enter execute\n");
	//int i=1; 
		


/* log */

if (strcmp(arguments[0], "log") == 0)
{
	const char * weekday[] = { "Sun", "Mon","Tue", "Wed","Thu", "Fri", "Sat"};
  	const char * Months[] = { "Jan", "Feb","Mar", "Apr","May", "Jun", "Jul","Aug","Sep","Oct","Nov","Dec"};

	//printf("in log\n");	
	//printf("\n\n\nlog so far..\n");
	for (int j=0;j<log_ctr;j++)
	{	//printf("inside loop..j is %d log_ctr is %d \n",j,log_ctr);
		printf("%s %s %02d %02d:%02d:%02d %d\n", weekday[Arr_log2[j].time.tm_wday], Months[Arr_log2[j].time.tm_mon], Arr_log2[j].time.tm_mday, Arr_log2[j].time.tm_hour, Arr_log2[j].time.tm_min, Arr_log2[j].time.tm_sec, Arr_log2[j].time.tm_year + 1900);
		printf("%s ",Arr_log2[j].name);
		printf("%d\n",Arr_log2[j].code);
	}

}


// print
else if (strcmp(arguments[0], "print") == 0)
{	
	//printf("enter print..\n");
	int i=1,k=0,flag=0,p=9; 
	char *temp_s;
	int  ch1 ='$';
	char* ptr1;
  while (arguments[i]!= NULL)
  {	
  		//printf("enter while..\n");
  		ptr1= strchr( arguments[i], ch1 );
  		if (ptr1!=NULL)
		{	//printf("$ found..\n");
			p=(int)(ptr1 - arguments[i]);
			if (p==0)
			{ //printf("p is 0..\n");
  				temp_s = malloc(strlen(ptr1+1) + 1);
				strncpy(temp_s,ptr1+1,strlen(ptr1+1)+1);
				//printf("copied..%s",temp_s);
				
		  		while(k<Env_ctr && flag==0)
		  		{	
		  			//printf("enter inner while..\n");
			  		if(strcmp(Arr_Env2[k].name,temp_s )==0)
					{	//printf("this is an Env Var\n");
						printf("%s ",Arr_Env2[k].value);
						flag=1;						
					}
					
					k++;
				}
				
				free(temp_s);flag=0;
			}
		}
		  	
  		else
  		{   //printf("not env..\n");
  			printf("%s ",arguments[i]); 
  		}
  
	//printf("%s ",arguments[i]); 
  	
 		i++;
  }	
printf("\n");
//printf("printttttt%d",ret_value);
/*
	printf("array so far..\n");
	for (int j=0;j<Env_ctr;j++)
	{	
		printf("name:%s\n",Arr_Env2[j].name);
	printf("val:%s\n",Arr_Env2[j].value);
	}
*/
//printf("\n\n");



}
 

/* theme -green, red, yellow */ 
 else if (strcmp(arguments[0], "theme") == 0)  
{	if(arguments[1]==NULL)
	{	 printf("*** ERROR: missing keyword or command, or permission problem***\n");
	}


	else if (strcmp(arguments[1], "green") == 0)  
	{	 printf("\e[32m");   			
   }
   
   else if (strcmp(arguments[1], "yellow") == 0)  
   {	
        printf("\e[33m");
   }
          		
   else if (strcmp(arguments[1], "red") == 0)  
   {		 
       printf("\e[31m");
          			 
   }
          	
   else
   {
       //printf("Error: Unrecognized color");
       printf("*** ERROR: missing keyword or command, or permission problem***\n");
       
   }
          		 
}	
          
/* non build in */          
else 
{	
	if (execvp(arguments[0], arguments) < 0) 
	{    
	     printf("*** ERROR: missing keyword or command, or permission problem***\n");
	}
		  
}


 
  

}





int main(int argc, char **ars)
{	
	//-------------------------------------interactive
	if(argc==1)
	{
				int status;
				pid_t pid;
				int ret_code=0;
				// Memory allocation for Environment variables
				EnvVar *Arr_Env = (EnvVar*)malloc(nEv * sizeof(EnvVar));
				
					// Memory allocation for command log
				Command *Arr_log = (Command*)malloc(nC * sizeof(Command));
				
				//const char*  ch2 = "=";
				int  ch3 ='$';
				int  ch1 = '=';
				int i,p;
			 
				int pipefds[2];
				int returnstatus;int n;

				  char  buffer[1024];             // the input line                 
				  char  *argv[64];              // the command line argument      

			 
				char readmessages[MAX];
			
			
				  while (1) 
				  {  ret_code=0;   
				  	 /* repeat until done ....         */
						 printf("cShell$ ");     /*   display a prompt             */
						 char *ptr1,*ptr2;
						 fgets(buffer, MAX, stdin); 
						
						 //int s_len=strlen(buffer);
					// replace '\n' added by getline with '\0'
						buffer[strlen(buffer) - 1] = '\0';
														
							char * s;
							
				//updating or new env Var
					i=0;
					while(buffer[i]==' ')
					{ i++;}
					
					if (buffer[i]=='$' )
					{	
						ptr1= strchr( buffer, ch1 );
						if (ptr1!=NULL)
						{	
							
							p=(int)(ptr1 - buffer);
							buffer[p]='\0';
							
								
							//extract name and value
							
							ptr2=strchr(buffer,ch3);
							
							s = malloc(strlen(ptr2+1) + 1);
							strncpy(s,ptr2+1,strlen(ptr2+1)+1);
							//printf("sss...%s\n",s);
							
							
							//check if env_var exists- if so update value
							int index=0,update=0;

							while(index<Env_ctr && update==0)
							{	
								
								if(strcmp(Arr_Env[index].name,s )==0)
								{	//printf("update value\n");
									//printf("update name:%s\n",Arr_Env[index].name);
									strncpy(Arr_Env[index].value,ptr1+1,strlen(ptr1+1)+1);
									update=1;
									
								}
								else index++;
							}
							
					
							if(update==0)
							{
									//creating new Env var		
									if(Env_ctr<nEv)
									{  //store in Arr_Env val
										Arr_Env[Env_ctr].name = malloc(strlen(ptr2+1) + 1);
										strncpy(Arr_Env[Env_ctr].name,ptr2+1,strlen(ptr2+1)+1);
										
										//store in Arr_Env val
										Arr_Env[Env_ctr].value = malloc(strlen(ptr1+1) + 1);
										strncpy(Arr_Env[Env_ctr].value,ptr1+1,strlen(ptr1+1)+1);
										
										
										//printf("1name:%s\n",Arr_Env[Env_ctr].name);
										//printf("val:%s\n",Arr_Env[Env_ctr].value);
										
										//update Env_ctr
										Env_ctr++;
									}
									else 
									{	printf("Error:Max Env variables reached\n"); }
								
							}
							
							free(s);
						}

						else
							{	printf("Error:No Env Value assigned to this new variable\n"); }
							
						

				/*		
				printf("array so far..\n");
				for (int j=0;j<Env_ctr;j++)
				{	printf("inside loop..j is %d Env_ctr is %d \n",j,Env_ctr);
					printf("name:%s\n",Arr_Env[j].name);
				printf("val:%s\n",Arr_Env[j].value);
				}
				*/
				
					}
				
			  else
					  {  
						 //printf("entering commands\n");
						 parse(buffer, argv);       //   parse the line              
					
						  
						  returnstatus = pipe(pipefds);
				
						if (returnstatus == -1) 
						{
							printf("Unable to create pipe\n");
							//free_all(buffer,Arr_Env,Arr_log);
							return 1;
						}
						  
						  
							  
						  pid = fork();
						  
						  if (pid < 0)
							{	 printf("forking child process failed\n");
								//free_all(buffer,Arr_Env,Arr_log);
								 exit(1);
							}
							
							else if (pid == 0)
							{		
								//printf("in child\n");
								 close(pipefds[0]);
								 dup2(pipefds[1], fileno(stdout));
								 
								 if (strcmp(argv[0], "exit") == 0)   
								 {		//free_all(buffer,Arr_Env,Arr_log);  
									  exit(0);            //   exit if it is                
								 }
								 else
								 {	  
								 	 	        
								 	execute(argv,Arr_Env,Arr_log);  // otherwise, execute the command 
								 	
				 				   close(pipefds[1]);
				 				   
								  	exit(0);
								  } 
						  }
						  
						  else
							{
									// wait for the child process to finish first 
									//printf("in parent\n");
									wait(&status);
									 close(pipefds[1]);
									 
							
							
							n=read(pipefds[0], readmessages, sizeof(readmessages));
							readmessages[n] = '\0';
							//message from child
							printf("%s\n",readmessages);
							//fputs(ErrorM,stdout);
							if (strcmp(readmessages, "*** ERROR: missing keyword or command, or permission problem***\n") == 0)
							{ //printf("Error found\n");
							  ret_code=1;
							}  
							
							//read(pipefds[0], readmessages, sizeof(readmessages));
							//printf("Parent Process - Reading from pipe –2is %s\n",readmessages);
							
							close(pipefds[0]);
								
							// printf("logging command\n");		
											//log name		   
											
						if(log_ctr>=nC)
						{ printf("***Error: Max number of commands reached***\n");
							return -1;
							
						} 	 	
						Arr_log[log_ctr].name = malloc(strlen(argv[0]) + 1);
						strncpy(Arr_log[log_ctr].name,argv[0],strlen(argv[0])+1);
						//printf("print logged command...%s\n",Arr_log[log_ctr].name);
										
						//log time
						// printf("logging time\n");
						 
						 time_t t = time(NULL);
										
						 Arr_log[log_ctr].time =*localtime(&t);
						
						
						
						//return code
						Arr_log[log_ctr].code =ret_code;
						 //printf("ret code is %d\n",Arr_log[log_ctr].code);
										
						log_ctr++;	    		
					/*		    	
							printf("\n\n\nlog so far..\n");
							for (int j=0;j<log_ctr;j++)
							{	//printf("inside loop..j is %d log_ctr is %d \n",j,log_ctr);
								printf("name:%s\n",Arr_log[j].name);
								printf("sec is %02d\n",Arr_log[j].time.tm_sec);
								printf("code is %d\n",Arr_log[j].code);
							}
					*/
							
									
									
									
									
									
									
									
						  			 if (strcmp(argv[0], "exit") == 0)      
									 { //free_all(buffer,Arr_Env,Arr_log);
									 	exit(0);            //   exit if it is  
									 }              
							 }
					 
						}
					}
				free_all(buffer,Arr_Env,Arr_log);	
			return 0;




			}

//---------------script------------------------------
else

{
	//printf("\n\n\nscript..\n");
	int status;
	pid_t pid;
	int ret_code=0;
	// Memory allocation for Environment variables
	EnvVar *Arr_Env = (EnvVar*)malloc(nEv * sizeof(EnvVar));
	
		// Memory allocation for command log
	Command *Arr_log = (Command*)malloc(nC * sizeof(Command));
	
	//const char*  ch2 = "=";
	int  ch3 ='$';
   int  ch1 = '=';
	int i,p;
 
   int pipefds[2];
   int returnstatus;int n;

     //char  buffer[1024];             /* the input line                 */
     char  *argv[64];              /* the command line argument      */

  /* Open the file for reading */
  char *buffer = NULL;
  size_t buf_size = 0;
  
  
  
  
 
   char readmessages[MAX];



 // open the file
  FILE *f = fopen(ars[1] , "r");
   // if there was an error
  if(f == NULL)
  {
    perror("Error opening file"); // print error
    free_all(buffer,Arr_Env,Arr_log);
    return(-1); 
  }
  
   
  
  while ( getline(&buffer, &buf_size, f)!=-1)
  // inf Loop till file done
 
  {
  
 
     
     ret_code=0;   
     	
          
      char *ptr1,*ptr2;
          
         
      //int s_len=strlen(buffer);
		
			buffer[strlen(buffer) - 1] = '\0';

		char * s;
				
	//updating or new env Var
		i=0;
		while(buffer[i]==' ')
		{ i++;}
		
		if (buffer[i]=='$' )
		{	
			ptr1= strchr( buffer, ch1 );
			if (ptr1!=NULL)
			{	
				
				p=(int)(ptr1 - buffer);
				buffer[p]='\0';
				
					
				//extract name and value
				
				ptr2=strchr(buffer,ch3);
				
				s = malloc(strlen(ptr2+1) + 1);
				strncpy(s,ptr2+1,strlen(ptr2+1)+1);
				//printf("sss...%s\n",s);
				
				
				//check if env_var exists- if so update value
				int index=0,update=0;

				while(index<Env_ctr && update==0)
				{	
					
					if(strcmp(Arr_Env[index].name,s )==0)
					{	//printf("update value\n");
						//printf("update name:%s\n",Arr_Env[index].name);
						strncpy(Arr_Env[index].value,ptr1+1,strlen(ptr1+1)+1);
						update=1;
						
					}
					else index++;
				}
				
		
				if(update==0)
				{
						//creating new Env var		
						if(Env_ctr<nEv)
						{  //store in Arr_Env val
							Arr_Env[Env_ctr].name = malloc(strlen(ptr2+1) + 1);
							strncpy(Arr_Env[Env_ctr].name,ptr2+1,strlen(ptr2+1)+1);
							
							//store in Arr_Env val
							Arr_Env[Env_ctr].value = malloc(strlen(ptr1+1) + 1);
							strncpy(Arr_Env[Env_ctr].value,ptr1+1,strlen(ptr1+1)+1);
							
							
							//printf("1name:%s\n",Arr_Env[Env_ctr].name);
							//printf("val:%s\n",Arr_Env[Env_ctr].value);
							
							//update Env_ctr
							Env_ctr++;
						}
						else 
						{	printf("Error:Max Env variables reached\n"); }
					
				}
				free(s);
			}

			else
				{	printf("Error:No Env Value assigned to this new variable\n"); 
					
				}
				
			

	/*		
	printf("array so far..\n");
	for (int j=0;j<Env_ctr;j++)
	{	printf("inside loop..j is %d Env_ctr is %d \n",j,Env_ctr);
		printf("name:%s\n",Arr_Env[j].name);
	printf("val:%s\n",Arr_Env[j].value);
	}
	*/
	
	}
   
  else
        {  
          //printf("entering commands\n");
          parse(buffer, argv);       //   parse the line              
      
      	  
      	  returnstatus = pipe(pipefds);
   
			if (returnstatus == -1) 
			{
				printf("Unable to create pipe\n");
				free_all(buffer,Arr_Env,Arr_log);			
				
				return 1;
			}
      	  
      	  
      	     
			  pid = fork();
			  
			  if (pid < 0)
				{	 printf("forking child process failed\n");
						free_all(buffer,Arr_Env,Arr_log);
					 _exit(1);
				}
				
				else if (pid == 0)
				{		
					//printf("in child\n");
				    close(pipefds[0]);
				    dup2(pipefds[1], fileno(stdout));
				    
				    if (strcmp(argv[0], "exit") == 0)  
				    {   
				    		
				        _exit(0);            //   exit if it is   
				     }             
				    
				    else
				    {	  
				    	 	        
				    	execute(argv,Arr_Env,Arr_log);  // otherwise, execute the command 
				    	
    				   close(pipefds[1]);
    				   
				     	exit(0);
				     } 
			  }
			  
			  else
				{
						// wait for the child process to finish first 
						//printf("in parent\n");
						wait(&status);
						 close(pipefds[1]);
						 
				
				
				n=read(pipefds[0], readmessages, sizeof(readmessages));
				readmessages[n] = '\0';
				//message from child
				printf("%s\n",readmessages);
				
				if (strcmp(readmessages, "*** ERROR: missing keyword or command, or permission problem***\n") == 0)
				{ //printf("Error found\n");
				  ret_code=1;
				}  
				
				//read(pipefds[0], readmessages, sizeof(readmessages));
				//printf("Parent Process - Reading from pipe –2is %s\n",readmessages);
      		
				close(pipefds[0]);
				
				if(log_ctr>=nC)
						{ printf("***Error: Max number of commands reached***\n");
							return -1;
							
						}
					
				// printf("logging command\n");		
								//log name		    	 	
			Arr_log[log_ctr].name = malloc(strlen(argv[0]) + 1);
			strncpy(Arr_log[log_ctr].name,argv[0],strlen(argv[0])+1);
			//printf("print logged command...%s\n",Arr_log[log_ctr].name);
						   
			//log time
			// printf("logging time\n");
			 
			 time_t t = time(NULL);
						   
			 Arr_log[log_ctr].time =*localtime(&t);
		   
			
			
			//return code
			Arr_log[log_ctr].code =ret_code;
		    //printf("ret code is %d\n",Arr_log[log_ctr].code);
						   
			log_ctr++;	    		
		/*		    	
				printf("\n\n\nlog so far..\n");
				for (int j=0;j<log_ctr;j++)
				{	//printf("inside loop..j is %d log_ctr is %d \n",j,log_ctr);
					printf("name:%s\n",Arr_log[j].name);
					printf("sec is %02d\n",Arr_log[j].time.tm_sec);
					printf("code is %d\n",Arr_log[j].code);
				}
		*/
				
						
						
						
						
						
						
						
			  			 if (strcmp(argv[0], "exit") == 0) 
			  			 {     
			  			 	free_all(buffer,Arr_Env,Arr_log);
				        _exit(1);            //   exit if it is  
				        
				        }              
				 }
       
			}
			
		
			
			
		}
		
	//printf("reached end...\n");	
	  // Free the  buffer 
free_all(buffer,Arr_Env,Arr_log);
  
  // close file
  fclose(f);
  

	
	return 0;
}

}

