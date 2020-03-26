// Shell starter file
// You may make any changes to any part of this file.

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <pwd.h>

#define COMMAND_LENGTH 1024
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)
#define HISTORY_DEPTH 10

char *tokens[NUM_TOKENS];
char history[HISTORY_DEPTH][COMMAND_LENGTH];
int history_arr[HISTORY_DEPTH] = {0}; //Error
int command_count = 0;
// int ctrlC = 0;

int overwrite_command(char cur_history[HISTORY_DEPTH][COMMAND_LENGTH], int cur_history_arr[HISTORY_DEPTH],int index){
	if(index == -1){
		cur_history_arr[command_count-1] = 0;
		command_count--;
		return 0;
	}
	int i = 0;
	if(command_count > HISTORY_DEPTH){
		for (i = 0; i < HISTORY_DEPTH; i++){
			if(cur_history_arr[i] == (index)){
				// memcpy(cur_history[(HISTORY_DEPTH-1)], cur_history[i], strlen(cur_history[i])+1);
				strcpy(history[HISTORY_DEPTH-1], history[i]);
					// tokenize_command(cur_buff, tokens);
				break;
			}
		}
	}
	else {
		// memcpy(cur_history[(command_count-1)], cur_history[index - 1], strlen(cur_history[index - 1]));
		strcpy(cur_history[(command_count-1)], cur_history[index - 1]);
		i = command_count-1;

		// char output_str[256];  //for test
		// sprintf(output_str, "%ld",strlen(cur_history[(command_count-1)]));
		// write(STDOUT_FILENO, output_str, strlen(output_str));
		// write(STDOUT_FILENO, "\t", strlen("\t"));
	}
	return i;
}




void add_command(char cur_history[HISTORY_DEPTH][COMMAND_LENGTH], char *buff, int cur_history_arr[HISTORY_DEPTH]){
	// if(buff[0] == '!'){
	// 	return;
	// }
	int k = 0;
	while(buff[k] != '\n'){
		k++;
	}
	buff[k] = '\0';

	if(command_count >= HISTORY_DEPTH){
		for(int y = 0; y < (HISTORY_DEPTH-1); y++){
			// memcpy(cur_history[y],cur_history[y+1], strlen(cur_history[y+1]));
			strcpy(cur_history[y],cur_history[y+1]);
			cur_history_arr[y] = cur_history_arr[y+1];
		}
		// memcpy(cur_history[(HISTORY_DEPTH-1)], buff, strlen(buff));
		strcpy(cur_history[(HISTORY_DEPTH-1)], buff);
		cur_history_arr[(HISTORY_DEPTH-1)] = (command_count+1);
	}
	else {
		// memcpy(cur_history[command_count], buff, strlen(buff));
		strcpy(cur_history[command_count], buff);
		cur_history_arr[command_count] = (command_count+1);
	}
}


void print_history(char cur_history[HISTORY_DEPTH][COMMAND_LENGTH], int cur_history_arr[HISTORY_DEPTH]){
	int end_index = HISTORY_DEPTH;
	for(int i = 0; i < HISTORY_DEPTH; i++){
		if(cur_history_arr[i] == 0){
			end_index = i;
			break;
		}
	}
	char output_str[256];
	for(int j = 0; j < end_index; j++){
		sprintf(output_str, "%d",cur_history_arr[j]);
		write(STDOUT_FILENO, output_str, strlen(output_str));
		write(STDOUT_FILENO, "\t", strlen("\t"));

		// sprintf(output_str, "%ld",strlen(cur_history[(j)]));  //for test
		// write(STDOUT_FILENO, output_str, strlen(output_str));
		// write(STDOUT_FILENO, "\t", strlen("\t"));
		
		write(STDOUT_FILENO, cur_history[(j)], strlen(cur_history[(j)]));
		write(STDOUT_FILENO, "\n", strlen("\n"));
	}
}


/**
 * Command Input and Processing
 */

/*
 * Tokenize the string in 'buff' into 'tokens'.
 * buff: Character array containing string to tokenize.
 *       Will be modified: all whitespace replaced with '\0'
 * tokens: array of pointers of size at least COMMAND_LENGTH/2 + 1.
 *       Will be modified so tokens[i] points to the i'th token
 *       in the string buff. All returned tokens will be non-empty.
 *       NOTE: pointers in tokens[] will all point into buff!
 *       Ends with a null pointer.
 * returns: number of tokens.
 */
int tokenize_command(char *buff, char *tokens[])
{
	int token_count = 0;
	_Bool in_token = false;
	int num_chars = strnlen(buff, COMMAND_LENGTH);

	// printf("%d\n",num_chars); //for test
	
	for (int i = 0; i < num_chars; i++) {
		switch (buff[i]) {
		// Handle token delimiter	s (ends):
		case ' ':
		case '\t':
		case '\n':
			buff[i] = '\0';
			in_token = false;
			break;

		// Handle other characters (may be start)
		default:
			if (!in_token) {
				tokens[token_count] = &buff[i];
				token_count++;
				in_token = true;
			}
		}
	}
	tokens[token_count] = NULL;
	return token_count;
}

/**
 * Read a command from the keyboard into the buffer 'buff' and tokenize it
 * such that 'tokens[i]' points into 'buff' to the i'th token in the command.
 * buff: Buffer allocated by the calling code. Must be at least
 *       COMMAND_LENGTH bytes long.
 * tokens[]: Array of character pointers which point into 'buff'. Must be at
 *       least NUM_TOKENS long. Will strip out up to one final '&' token.
 *       tokens will be NULL terminated (a NULL pointer indicates end of tokens).
 * in_background: pointer to a boolean variable. Set to true if user entered
 *       an & as their last token; otherwise set to false.
 */






void read_command(char *buff, char *tokens[], _Bool *in_background)
{
	*in_background = false;

	// Read input
	int length = read(STDIN_FILENO, buff, COMMAND_LENGTH-1);
	
	add_command(history, buff, history_arr);
	command_count ++;


	if ( (length < 0) && (errno !=EINTR) ){
  		perror("Unable to read command. Terminating.\n");
    	exit(-1);  /* terminate with error */
	}

	// Null terminate and strip \n.
	buff[length] = '\0';
	if (buff[strlen(buff) - 1] == '\n') {
		buff[strlen(buff) - 1] = '\0';
	}

	// Tokenize (saving original command string)
	int token_count = tokenize_command(buff, tokens);
	if (token_count == 0) {
		return;
	}

	// Extract if running in background:
	if (token_count > 0 && strcmp(tokens[(token_count-1)], "&") == 0) {
		*in_background = true;
		tokens[token_count - 1] = 0;
	}
}

void handle_SIGINT(){
	write(STDIN_FILENO, "\n", strlen("\n"));
	print_history(history, history_arr);

	command_count--;
}



/**
 * Main and Execute Commands
 */
int main(int argc, char* argv[])
{
	char input_buffer[COMMAND_LENGTH];
	// char *tokens[NUM_TOKENS];
	int status;

	struct sigaction handler;
	handler.sa_handler = handle_SIGINT;
	handler.sa_flags = 0;
	sigemptyset(&handler.sa_mask);	
	sigaction(SIGINT, &handler, NULL);
	
	char cwd[256];
	char temphomecwd[256];
	getcwd(temphomecwd,sizeof(temphomecwd));
	char prevcwd[256];
	getcwd(prevcwd,sizeof(prevcwd));
	char curcwd[256];
	
	char cur_buff[1024];

	while (true) {

		while (waitpid(-1, NULL, WNOHANG) > 0);	//clean up
		getcwd(curcwd, sizeof(curcwd));
		write(STDOUT_FILENO, curcwd, strlen(curcwd));		
		write(STDOUT_FILENO, "$ ", strlen("$ "));
		_Bool in_background = false;
		read_command(input_buffer, tokens, &in_background);
		
		// if (ctrlC == 1){
		// 	ctrlC = 0;
		// }
		restartLoop: 
			if (tokens[0] == NULL){
				overwrite_command(history, history_arr, -1);
				continue;
			}
			else if (strcmp(tokens[0],"!!") == 0){
				int line_num;
				line_num = command_count-1;
				// if(line_num != 0){
				if( line_num <= 0){
					write(STDIN_FILENO, "SHELL: Unknown history command.\n", strlen("SHELL: Unknown history command.\n"));
					overwrite_command(history, history_arr, -1);
					continue;
				}
				else{
					int next_index = overwrite_command(history, history_arr, line_num);
					write(STDIN_FILENO, history[next_index%10],strlen(history[next_index%10]));
					write(STDIN_FILENO, "\n", strlen("\n"));
					// tokenize_command(history[next_index % 10], tokens);
					// memcpy(cur_buff, history[next_index % 10], strlen(history[next_index % 10])+1);
					strcpy(cur_buff, history[next_index % 10]);

					// write(STDIN_FILENO, cur_buff, strlen(cur_buff));
					// write(STDIN_FILENO, "\n", strlen("\n"));
					
					tokenize_command(cur_buff, tokens);
					goto restartLoop;
				}
			}
			else if (strchr(tokens[0],'!')){
				int line_num;
				sscanf(tokens[0], "!%d", &line_num);

				if( line_num <= 0  || line_num < (command_count - 9) || line_num >= command_count){
					write(STDIN_FILENO, "SHELL: Unknown history command.\n", strlen("SHELL: Unknown history command.\n"));
					overwrite_command(history, history_arr, -1);
					continue;
				}
				else{
					int next_index = overwrite_command(history, history_arr, line_num);
					// write(STDIN_FILENO, history[next_index%10],strlen(history[next_index%10]));
					// write(STDIN_FILENO, "\n", strlen("\n"));
					// tokenize_command(history[next_index % 10], tokens);
					// printf("%ld\n", strlen(history[next_index % 10]));
					// memcpy(cur_buff, history[next_index % 10], strlen(history[next_index % 10])+1);
					strcpy(cur_buff, history[next_index % 10]);
					// printf("%ld\n", strlen(cur_buff));
					// write(STDIN_FILENO, cur_buff,strlen(cur_buff));
					// write(STDIN_FILENO, "\n", strlen("\n"));

					tokenize_command(cur_buff, tokens);
					goto restartLoop;
				}
			}

			// restartLoop: 

			else if(strcmp(tokens[0], "exit") == 0){
				if(tokens[1] != NULL){
					write(STDOUT_FILENO, "Error: Could not Exit as unexpected argument occured.\n", strlen("Error: Could not Exit as unexpected argument occured.\n"));
				}
				else{
					return 0;
				}
			}
			else if(strcmp(tokens[0], "pwd") == 0){
				getcwd(cwd, sizeof(cwd));
			}
			else if(strcmp(tokens[0], "cd") == 0){
				getcwd(prevcwd,sizeof(prevcwd));
				if (chdir(tokens[1])!=0){
					write(STDOUT_FILENO, "Error: Could not change directory.\n", strlen("Error: Could not change directory.\n"));
				}
			}
			else if (strcmp(tokens[0], "history") == 0){
				if(tokens[1] != NULL){
					write(STDOUT_FILENO, "Error: Unexpected Argument Occured.\n", strlen("Error: Unexpected Argument Occured.\n"));
				}
				else{
					print_history(history, history_arr);
				}
			}

		pid_t pid;
		pid = fork();
		if (pid <0){
			write(STDOUT_FILENO, "Fork Failed\n", strlen("Fork Failed\n"));
			exit(-1);
		}
		else if (pid == 0){ 
			execvp(tokens[0], tokens);	//child process excute the command
			exit(-1);
		}
		else {
			if (in_background == false){
					while(waitpid(pid, &status, 0)>0){
						;
					}
			}
		}

		

		if (in_background) {
			write(STDOUT_FILENO, "Run in background.", strlen("Run in background."));
			write(STDIN_FILENO, "\n", strlen("\n"));
		}

		/**
		 * Steps For Basic Shell:
		 * 1. Fork a child process
		 * 2. Child process invokes execvp() using results in token array.
		 * 3. If in_background is false, parent waits for
		 *    child to finish. Otherwise, parent loops back to
		 *    read_command() again immediately.
		 */

	}
	return 0;
}
