## Introduction

This project involved developing a custom shell in C, with full support for inter-process communication using pipes. All functionality was implemented from scratch without the use of external libraries, providing in-depth experience with low-level process control and system programming.

## Development Requirements

The following are development requirements for the project to be fulfilled.

1. While most shells allow the user to customize the prompt, the shell will always use the following prompt string:
   
   ```text
   $> 
   ```
   
   A dollar sign ($), followed by a greater than (>), followed by a space ( )
2. The shell must support up to 10 command line arguments for each command (separated by spaces)
3. The shell must support up to 5 commands run sequentially (separated by semicolon)
4. While there many 'flavors' of the ```exec() ```system call, for this project it is required to use ```execvp()```.
5. Your shell must support a single internal command called 'quit' which stops the execution loop
6. The project must use ```fork()``` and ```execvp()``` to create child processes and execute commands.  The use of the ```system()``` function is not allowed for the project.
7. The shell must be free of memory leaks and segmentation faults.
8. The shell only needs to support a max user input of 256 characters.

Additional requirements for TTSH with command 'pipes':

1. The shell must support up to 5 commands run sequentially with redirected input and output (separated by the pipe character - '|')
2. The shell must support a mixture of commands run sequentially with semicolons (without input/output chaining) and pipes (with input/output chaining).
3. Some shells allow for input and output redirection to a file using the '<' and/or '>' character.  The shell does ***NOT*** need to support file redirection.
4. All processes must close unneeded ends of pipes 
5. The shell must not redirect stderr for any child process.


## Sample Execution

Here is a sample output for an execution of the shell.

***NOTE:*** the first line and last line shows the prompt presented from ```bash``` prior to running the shell.

```text
user@pc:~$ ./ttsh
$> ls
Makefile  ttsh  ttsh.c  ttsh.o
$> ls -l
total 48
-rw-r--r-- 1 user user   277 Jun  6 14:19 Makefile
-rwxr-xr-x 1 user user 22072 Aug  8 12:31 ttsh
-rw-r--r-- 1 user user  7237 Aug  8 12:31 ttsh.c
-rw-r--r-- 1 user user 15888 Aug  8 12:31 ttsh.o
$> ls | grep Make
Makefile
$> echo Listing ttsh files ; ls | grep ttsh ; echo I found them
Listing ttsh files
ttsh
ttsh.c
ttsh.o
I found them
$> asdfasdfasdfasdf
asdfasdfasdfasdf: failed to execute command
$> quit
user@pc:~/$
```
