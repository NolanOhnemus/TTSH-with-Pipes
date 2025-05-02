/**
 * @file main.c
 * @brief Program entry point.  Runs the teeny tiny shell
 *
 * Course: CSC3210
 * Section: 002
 * Assignment: Teeny Tiny Shell with Pipes
 * Name: Nolan Ohnemus
 * 
 * Suggestions for next lab:
 * 
 * 
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define INPUT_MAX 256
#define CMD_MAX 5
#define PARAM_MAX 10
#define PIPE_MAX 5


/**
 * @brief Reads a string of user input
 * @param dest - buffer for user input string
 * @return 0 on success
 * @return -1 on error
 */
int read_cmd_string(char dest[INPUT_MAX]) {

    // Read user input
    if(fgets(dest, INPUT_MAX, stdin) == NULL) {
        fprintf(stderr, "Unable to read user input\n");
        return -1;
    }

    // Remove trailing return character
    int len = strlen(dest);
    if(dest[len-1] == '\n') {
        dest[len - 1] = '\0';
    }

    return 0;
}

/**
 * @brief Parses a string and divides it into individual commands
 * @param input - string containing user input
 * @param cmd_strs - the target array for command strings
 * @return the number of commands found in the input
 * @return -1 on error
 */
int parse_commands(char input[INPUT_MAX], char cmd_strs[CMD_MAX][INPUT_MAX]) {

    // Chop the input into command strings
    int cmd_count = 0;
    char* cmd_ptr = strtok(input, ";");
    while(cmd_ptr) {
        if(cmd_count >= CMD_MAX) {
            fprintf(stderr, "Too many commands\n");
            return -1;
        }
        strncpy(cmd_strs[cmd_count], cmd_ptr, INPUT_MAX);
        cmd_count++;
        cmd_ptr = strtok(NULL, ";");
    }

    return cmd_count;
}

/**
 * @brief Parses command string to exract the command and the parameters
 * @param command - The command to split
 * @param function - Destination for the function
 * @param parameters - The destination to store the paramters
 * @return 0 ~ 10 for the number of parameters
 * @return -1 if an error has occured
*/
int split_command(char command[INPUT_MAX/PARAM_MAX], char function[INPUT_MAX/PARAM_MAX], 
                    char parameters[PARAM_MAX + 1][INPUT_MAX/PARAM_MAX]){
    int param_count = 0;
    char* splits = strtok(command, " ");
    int checked = 0;
    while(splits){
        if(checked == 0){
            strncpy(function, splits, INPUT_MAX/PARAM_MAX);
            checked = 1;
            //splits = strtok(NULL, "");
        }
        else if(param_count >= PARAM_MAX){
            fprintf(stderr, "Too many parameters in comamnd\n");
            return -1;
        } else{
            strcpy(parameters[param_count], splits);
			//remove_spaces(*parameters[param_count]);
            ++param_count;
            splits = strtok(NULL, " ");
        }
    }
    return param_count;
}

/**
 * @brief Parses command string to exract the two sides of the pipe
 * @param command - The command to split
 * @param functions - Destination for the functions to be piped
 * @return 0 ~ 10 for the number of commands
 * @return -1 if an error has occured
*/
int split_pipe(char command[INPUT_MAX/PARAM_MAX], char functions[PIPE_MAX][INPUT_MAX]){
	// Chop the input into command strings
    int cmd_count = 0;
    char* cmd_ptr = strtok(command, "|");
    while(cmd_ptr) {
        if(cmd_count >= CMD_MAX) {
            fprintf(stderr, "Too many commands in pipe!\n");
            return -1;
        }
        strncpy(functions[cmd_count], cmd_ptr, INPUT_MAX);
        cmd_count++;
        cmd_ptr = strtok(NULL, "|");
    }
    return cmd_count;
}

/**
 * @brief Parses user input to check if the given char array contains quit
 * @param input - a char array to be checked for quit
 * @return 1 if quit was found
 * @return 0 if quit was not found
*/
int find_quit(char input[INPUT_MAX]){
    int i = 0;
    while(i < INPUT_MAX - 3){
        char current = input[i];
        if(current == 'q' || current == 'Q'){
            char u_check = input[i + 1];
            char i_check = input[i + 2];
            char t_check = input[i + 3];
            if((u_check == 'u' || u_check == 'U') && (i_check == 'i' || i_check == 'I') &&
               (t_check == 't' || t_check == 'T')){
                return 1;
            }
        }
        ++i;
    }
    return 0;
}


/**
 * @brief Parses user input to check if the given char array contains a pipe
 * @param input - a char array to be checked for pipe
 * @return 1 if quit was found
 * @return 0 if quit was not found
*/
int contains_pipe(char input[INPUT_MAX]){
	int i = 0;
	while(i < INPUT_MAX){
		char current = input[i];
		if(current == '|'){
			return 1;
		}
		else{
			++i;
		}
	}
	return 0;
}


/**
 * @brief Parses a string to remove any whitespaces for use in cleaning commands and parameters
 * @param s - a char array to be checked for quit
*/
void remove_spaces(char* s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}

/**
 * @brief cleans all of the parameters in an array of string 
 * @param s - a char array to be checked for quit
*/
void clean_parameters(char input[PARAM_MAX + 1][INPUT_MAX/PARAM_MAX]){
	for(int i = 0; i < PARAM_MAX; ++i){
		remove_spaces(input[i]);
	}
}

/**
 * @brief Program entry procedure for the shell
 */
int main(int argc, char* argv[])  {

    int exit_check = 0;
    while(exit_check == 0){

        char user_input[INPUT_MAX];
        char cmd_strs[CMD_MAX][INPUT_MAX];

        // Print the input prompt
        printf("$> ");

        // Read user input
        if(read_cmd_string(user_input) == -1) {
            return 1;
        }

        // handle the 'quit' command
        if(find_quit(user_input) == 1){
            return 0;
        }

        // Chop the input into command strings
        int cmd_count = parse_commands(user_input, cmd_strs);
        if(cmd_count == -1) {
            return 1;
        }

        // Chop the commands into arguments and execute one at a time
        for(int i = 0; i < cmd_count; i++) {
			//Get command we are going to use
			char* command = cmd_strs[i];
			//check if the command sets up pipes
			if(contains_pipe(command) == 1){ // if commands need to be piped
				// get the unique commands to feed into the pipes
				char pipe_strs[PIPE_MAX][INPUT_MAX];
				int pipe_count = split_pipe(command, pipe_strs);
				if(pipe_count < 2){
					fprintf(stderr, "Not enough commands to build pipe!\n");
					return -1;
				}
				//set up pipes
				int pipes[pipe_count - 1][2];
				for(int j = 0; j < pipe_count-1; ++j){
					pipe(pipes[j]);
				}

				for(int k = 0; k < pipe_count; ++k){

					//create child
					pid_t pid = fork();
					if(pid == -1){
						// if error in fork is made return
						fprintf(stderr, "Error in creating fork\n");
						for(int j = 0; j < pipe_count-1; ++j){
							close(pipes[j][0]);
							close(pipes[j][1]);
						}
						return 1;
					} else if (pid == 0){
						//Get command we are going to use
						char* command = pipe_strs[k];
						//Designate a place to store the function and the parameters
						char function[INPUT_MAX/PARAM_MAX];
						char param_strs[PARAM_MAX + 1][INPUT_MAX/PARAM_MAX];
						for(int j = 0; j < PARAM_MAX + 1; ++j){
							*param_strs[j] = '\0';
						}

						// split the command and parameters
						int num_param = split_command(command, function, param_strs);
						if(num_param == -1){
							return 1;
						}
						clean_parameters(param_strs);
						remove_spaces(command);

						// Initialize the pointer array for execvp
						char *param_ptrs[num_param + 1];

						// create an array of pointers to the char arrays in param_strs
						for(int j = 0; j < num_param + 1; ++j){
							if(j == num_param){
								param_ptrs[j] = NULL;
							} else {
								param_ptrs[j] = param_strs[j];
							}
						}

						if(k == 0){
							//child set std in and std out to proper ends of pipe
							dup2(pipes[k][1], STDOUT_FILENO);

							//Child closes all pipe ends after the call to dup2() to remove extra resources
							for(int j = 0; j < pipe_count-1; ++j){
								close(pipes[j][0]);
								close(pipes[j][1]);
							}

							//Children run execvp() to execute the command. 
							//File descriptors for stdin and stdout will remain in place after the call to execvp()
							if(execvp(command, param_ptrs) == -1){
								fprintf(stderr, "Failed to execute function: %s\n", strerror(errno));
								exit(-1);
							}
						}
						else if(k == pipe_count - 1){
							//child set std in and std out to proper ends of pipe
							dup2(pipes[k-1][0], STDIN_FILENO); 

							//Child closes all pipe ends after the call to dup2() to remove extra resources
							for(int j = 0; j < pipe_count-1; ++j){
								close(pipes[j][0]);
								close(pipes[j][1]);
							}

							//Children run execvp() to execute the command. 
							//File descriptors for stdin and stdout will remain in place after the call to execvp()
							if(execvp(command, param_ptrs) == -1){
								fprintf(stderr, "Failed to execute function: %s\n", strerror(errno));
								exit(-1);
							}
						}
						else{
							//child set std in and std out to proper ends of pipe
							dup2(pipes[k-1][0], STDIN_FILENO); 
							dup2(pipes[k][1], STDOUT_FILENO);
							//Child closes all pipe ends after the call to dup2() to remove extra resources
							for(int j = 0; j < pipe_count-1; ++j){
								close(pipes[j][0]);
								close(pipes[j][1]);
							}
							//Children run execvp() to execute the command. 
							//File descriptors for stdin and stdout will remain in place after the call to execvp()
							if(execvp(command, param_ptrs) == -1){
								fprintf(stderr, "Failed to execute function: %s\n", strerror(errno));
								exit(-1);
							}
						}
					}
				}

				//Parent closes all ends of all pipes
				for(int j = 0; j < pipe_count-1; ++j){
					close(pipes[j][0]);
					close(pipes[j][1]);
				}
				// if parent, wait
				for(int j = 0; j < pipe_count; ++j) {
					wait(NULL);
				}
			}
			else { // if we are not piping
				//Get command we are going to use
				char* command = cmd_strs[i];
				//Designate a place to store the function and the parameters
				char function[INPUT_MAX/PARAM_MAX];
				char param_strs[PARAM_MAX + 1][INPUT_MAX/PARAM_MAX];
				for(int j = 0; j < PARAM_MAX + 1; ++j){
					*param_strs[j] = '\0';
				}

				// split the command and parameters
				int num_param = split_command(command, function, param_strs);
				if(num_param == -1){
					return 1;
				}

				// Initialize the pointer array for execvp
				char *param_ptrs[num_param + 1];

				// create an array of pointers to the char arrays in param_strs
				for(int j = 0; j < num_param + 1; ++j){
					if(j == num_param){
						param_ptrs[j] = NULL;
					} else {
						param_ptrs[j] = param_strs[j];
					}
				}

				//Create a fork
				pid_t pid = fork();
				if(pid == -1){
					// if error in fork is made return
					fprintf(stderr, "Error in creating fork\n");
					return 1;
				} else if (pid == 0){
					// if child was made run command
					if(execvp(function, param_ptrs) == -1){
						fprintf(stderr, "Failed to execute function: %s\n", strerror(errno));
						exit(-1);
					}
				} else {
					// if parent, wait
					wait(NULL);
				}
			}

            
        }
    }
    return 0;
}
