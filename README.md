## Introduction

In a previous assignment, you worked with a shell program that allows a user to execute one or more command in sequence.

In this assignment, you will be extending the functionality of that shell to use inter-process communication mechanism (i.e. pipes) to add one important feature to your shell.

Work on the assignment is to be done ***individually***.  You are welcome to collaborate with class members, but the project must be your own work.

## Development Requirements

The following are existing development requirements from the previous TTSH assignment.  These requirements must still be fulfilled.

1. While most shells allow the user to customize the prompt, your shell will always use the following prompt string:
   
   ```text
   $> 
   ```
   
   A dollar sign ($), followed by a greater than (>), followed by a space ( )
2. Your ```ttsh``` must support up to 10 command line arguments for each command (separated by spaces)
3. Your ```ttsh``` must support up to 5 commands run sequentially (separated by semicolon)
4. While there many 'flavors' of the ```exec() ```system call, for this assignment you are required to use ```execvp()```.  See the man page for the specifics of how ```execvp()``` works.
5. Your shell must support a single internal command called 'quit' which stops the execution loop
6. You must use ```fork()``` and ```execvp()``` to create child processes and execute commands.  The use of the ```system()``` function is not allowed for this assignment.
7. Your ```ttsh``` must be free of memory leaks and segmentation faults.
8. Your ```ttsh``` only needs to support a max user input of 256 characters.

Additional requirements for TTSH with command 'pipes':

1. Your ```ttsh``` must support up to 5 commands run sequentially with redirected input and output (separated by the pipe character - '|')
2. Your ```ttsh``` must support a mixture of commands run sequentially with semicolons (without input/output chaining) and pipes (with input/output chaining).
3. Some shells allow for input and output redirection to a file using the '<' and/or '>' character.  Your TTSH does ***NOT*** need to support file redirection.
4. All processes ***MUST*** close unneeded ends of pipes 
5. Your ```ttsh``` must ***NOT*** redirect stderr for any child process.


## Sample Execution

Here is a sample output for an execution of ```ttsh```.

***NOTE:*** the first line and last line shows the prompt presented from ```bash``` prior to running ```ttsh```.

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
