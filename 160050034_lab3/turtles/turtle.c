#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

int lsh_launch(char **args,int start)
{
  
  if(args[start] == 0 || (args[start][0] == ';' && args[start+1] == 0)) {
  	return 1;
  }

  pid_t pid, wpid;
  int status;

  if(args[start][0] == 'c' && args[start][1] == 'd' && args[start][2] == 0){
  	if(args[start+1] != 0 && ((args[start+2] != 0 && args[start+2][0] == ';') || (args[start+2] == 0))){
  		if(chdir(args[start+1]) != 0){
  			printf("Unable to open directory\n");
  		}
  		else{
  			if(args[start+2] != 0 && args[start+2][0] == ';'){
				lsh_launch(args,start+3);
  			}
  		}
  	}
  	else{
  		printf("Wrong nummber of arguments\n");

  		int x=0;
  		int k = start;
	  	while(args[k] != 0){
	  		if(args[k][0] == ';') {
	  			x = 1;
	  			break;
	  		}
	  		k++;
	  	}

	  	// printf("%d\n",k);

	  	if(x == 1){
			lsh_launch(args,k+1);
	  	}
  	}
  }

  else if(strcmp(args[start],"exit") == 0){
  	printf("Stopping all processes\n");
  	exit(0);
  }

  else{

  	int x = 0;
  	int k = start;
  	while(args[k] != 0){
  		if(args[k][0] == '>') x = 1;
  		k++;
  	}
  	k = start;
  	while(args[k] != 0){
  		if(args[k][0] == ';') x = 0;
  		k++;
  	}

  	if(x == 1){
  		char **curr = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  		char **curr2 = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	    int t=start;
	    int i = start;
	    while(args[i] != 0 && args[i][0] != '>'){
	     	curr[i-t] = args[i];
	     	i++;
	    }
	    curr[i-t] = 0;
	    i++;

	    t = i;
	    while(args[i] != 0){
	    	curr2[i-t] = args[i];
	    	i++;
	    }

	    curr2[i-t] = 0;

	    FILE *test=fopen(*curr2, "w");
    	int fd=fileno(test),pid;
    	pid = fork();

	    if (pid == 0) {

	    	dup2(fd,1);
	 
			if (execvp(curr[0], curr) == -1) {
		    perror("Turtle");
			}
			
			// puts(curr);
			// exit(EXIT_FAILURE);
			fclose(fd);
		 } 
		 else if (pid < 0) {
		   // Error forking
		   perror("Turtle");
		 } 
		 else {
		   // Parent process
		   do {
		     wpid = waitpid(pid, &status, WUNTRACED);
		   } while (!WIFEXITED(status) && !WIFSIGNALED(status));
		 }

  	}

  	else{

	  	char **curr = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	    int t=start;
	    int i = start;
	    while(args[i] != 0 && args[i][0] != ';'){
	     	curr[i-t] = args[i];
	     	i++;
	    }
	    // printf("%d\n",i+1);

	    curr[i-t] = 0;

		pid = fork();
		if (pid == 0) {
			  // Child process
			if (execvp(curr[0], curr) == -1) {
		    perror("Turtle");
			}
			exit(EXIT_FAILURE);
		 } 
		 else if (pid < 0) {
		   // Error forking
		   perror("Turtle");
		 } 
		 else {
		   // Parent process
		   do {
		     wpid = waitpid(pid, &status, WUNTRACED);
		   } while (!WIFEXITED(status) && !WIFSIGNALED(status));
		 }

		lsh_launch(args,i+1);

			 
		}
	}

  return 1;
}



char **tokenize(char *line)
{
    char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
    char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
    int i, tokenIndex = 0, tokenNo = 0;

    for(i =0; i < strlen(line); i++){

        char readChar = line[i];

        if (readChar == ' ' || readChar == '\n' || readChar == '\t') {
            token[tokenIndex] = '\0';
            if (tokenIndex != 0){
                tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
                strcpy(tokens[tokenNo++], token);
                tokenIndex = 0; 
            }
        } 
        else {
            token[tokenIndex++] = readChar;
        }
    }
 
    free(token);
    tokens[tokenNo] = NULL ;
    return tokens;
}

void handler(){
  return;
}


void main(void)
{

    signal (SIGINT,handler);
    while (1) {   
	    char  line[MAX_INPUT_SIZE];         
	    char  **tokens;                      
       
        printf("Hello>");
        bzero(line, MAX_INPUT_SIZE);
        gets(line);           
        // printf("Got command %s\n", line);
        line[strlen(line)] = '\n';
        tokens = tokenize(line);
   

        // for(i=0;tokens[i]!=NULL;i++){
        //     printf("found token %s\n", tokens[i]);
        // }

        // for(i=0;tokens[i]!=NULL;i++){
        //     if(tokens[i] == ";;"){
        //     	curr[i] = NULL;
        //     	t = i+1;
        //     	lsh_launch(curr);
        //  	}
        //  	else{
        //  		curr[i-t] = tokens[i];
        //  	}
        // }
        lsh_launch(tokens,0);


        // for(int i=0;tokens[i]!=NULL;i++){
        //     free(tokens[i]);
        // }
        // free(tokens);
    }
     

}

                
