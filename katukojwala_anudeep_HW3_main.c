/**************************************************************
* Class:  CSC-415-02 Fall 2022
* Name: Anudeep Katukojwala
* Student ID: 922404701
* GitHub ID: anudeepkatukojwala
* Project: Assignment 3 – Simple Shell
*
* File: katukojwala_anudeep_HW3_main.c
*
* Description: C program to create a simple shell on top of 
*              the linux shell which accepts linux commands
*              and executes them from this code. 
*
**************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>

#define BUFFERSIZE 123



int main(int argc, char * argv[]){

    //Variables useful in the program
    //To satisfy the rubric "Your code must handle at least four command-line arguments"
    char * commandSubStrings[30];
    char command[BUFFERSIZE];


    //An infinite while loop to get the commands from user and execute them
    while(1){
        

        //Print the prefix prompt
        if(argc==1){
            printf("\n>  ");
        }     
        else{
            printf("\n%s  ",argv[1]); 
        }
        
        
        /*Reading lines from input*/

        //Read a line of input from stdin and save its return value
        char * returnOfFgets = fgets(command, BUFFERSIZE, stdin);

        //Report error and exit when fgets encounters an error
        if(returnOfFgets == NULL){
            printf("Error: While reading the line from stdin\n");
            break;
        }
        
        //variable to save the length of the entered command
        int lengthOfCommand = strlen(command);
        
        //fgets adds the newline character to buffer, when newline is read.
        //We need to replace that '\n' character with '\0' character 
        if ((lengthOfCommand > 0) && (command[lengthOfCommand - 1] == '\n')){
            command[lengthOfCommand - 1] = '\0';
        }
            
        //If the user entered empty command, we display error message
        //and wait for the next command   
        if(!strcmp("", command)){
            printf("Error: Empty line entered. Please enter a valid command\n");
            continue;
        }

        //Exit the SHELL when user enters exit command
        if(!strcmp("exit", command)){
            printf("exit command entered. SHELL exited\n");
            break;
        }

                               
        /*Split the entered command into tokens*/

        //Temporary variable to store the token from strtok function
        char * brokenToken;
        int i = 0;

        //Start tokenizing the user entered command
        brokenToken = strtok(command, " ");
        while(brokenToken != NULL){
        
        commandSubStrings[i] = brokenToken;
        //call strtok to get the next token from command
        brokenToken = strtok(NULL, " ");
        i++;
        }   
        //array passed to execvp must be NULL terminated, so we add NULL
        // at the end of the array of tokens
        commandSubStrings[i] = NULL;

        
        /*Execution of Command starts from here*/

        int returnOfFork;
        int statusOfWaitpid;
    

        //Calling fork to create child process, to execute the command
        returnOfFork = fork();
    
        //If fork returned -1 fork function is assumed to be failed
        if(returnOfFork == -1){
        
            printf("fork() call failed becuase: %s\n", strerror(errno));
            //To indicate that we are exiting with failure
            exit(1);
        }
        else if (returnOfFork == 0){
        
            printf("\n");

            //Execute the command entered by user using execvp
            int returnOfExecvp = execvp(commandSubStrings[0], commandSubStrings);
        
            //Display error with details if the command failed
            if(returnOfExecvp == -1){
                printf("Error: %s\n", strerror(errno));
                exit(1);
            }

            exit(0);
        }
        else{
            //We are in parent process and in parent process,
            // fork() return value will be child process ID
            //Parent waits until the child process dies
            waitpid(returnOfFork, &statusOfWaitpid, WUNTRACED);

            //Print child process ID (returnOfFork will have child process ID) and exit status
            printf("\nChild %d, exited with %d\n", returnOfFork, WEXITSTATUS(statusOfWaitpid));
        
        }
                             
    }

    return 0;
}