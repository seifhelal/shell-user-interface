//  main.c
//  OS_P1
//  Created by SEIF on 3/6/18.
//  Copyright © 2018 SEIF. All rights reserved.


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#define Max_line 80

char history[10][40]; //history array to store history commands
int count=0, temp_count=0; // count of commands
void update_history(char input_buffer[])
{    
    int f;
    // here we switch the elements of the array of history to update the new commands ang get rid of the old ones.
    for (f = 9;f>0; f--)
        strcpy(history[f], history[f-1]);
    strcpy(history[0],input_buffer); //put the new command line in the history
    count++;
    temp_count++;
    if(count>10)
        count=10;
}
void print_history()
{
    printf("History of Shell Command \n");
    int temp_counter = count;
    int i;
    for (i = 0; i<10;i++)
    {
        int c2 = 0;  // another counter to print the history with order
        printf("%d.  ", temp_counter);
        //printing
        while (history[i][c2] != '\n' && history[i][c2] != '\0')
        {
            printf("%c", history[i][c2]);
            c2++;
        }
        printf("\n");
        temp_counter--;
        if (temp_counter ==  0)
            break;
    }
}

int command(char input_buffer[], char *args[], int *flag)
{
    
    int args_counter=0;
    int command_length=strlen(input_buffer);
    char *token = strtok(input_buffer, " \t");
    while (token != NULL)
    {
        if (token[0]=='&')
        {
            *flag=1;
        }
        else
            args[args_counter]= token;
        token = strtok(NULL, " \t");
        args_counter++;
    }
    args[args_counter]=NULL;
    // checking if the command is history
    if(strcmp(args[0],"history")==0)
    {
        if(count>0) // check if there exist commands in the history
        {
            print_history();
            return(-1); // return zero in order not to excute the fokring
        }
        else
        {
            printf("\nYou haven't entered any commands to display !! \n");
            return(-1);
        }
    }
    //checking for !
    else if (args[0][0]-'!' ==0)
    {
        int second_letter = args[0][1];
        if(second_letter-48>count)
            printf("\nThis command does not exist \n");
        else
        {
            if(second_letter==33) // checking for !!
                strcpy(input_buffer,history[0]);  // return the 10th command
            else if(second_letter >=49)
                strcpy(input_buffer,history[count-(second_letter-48)]);
        }
    }
    update_history(input_buffer);
    return(0);
}
int main(void)
{
    char *args[Max_line / 2 + 1];
    char input_buffer[Max_line]=" ";
    int should_run = 1, cmd_val,var, var2;
    int flag=0;
    pid_t pid;
    while (should_run)
    {
        printf("osh>");
        fflush(stdout);
        flag=0;
        //read(STDIN_FILENO, input_buffer, Max_line);
        scanf(" %[^\n]s", input_buffer);
        if(strcmp(input_buffer,"exit")==0)        {
            return (0);
        }
        cmd_val=command(input_buffer, args, &flag);
        if( cmd_val!= -1)
        {
            pid=fork();
            if (pid < 0)//if pid is less than 0, forking fails
            {
                printf("Fork failed.\n");
                exit (1);
            }
            else if (pid == 0)//if pid ==0
            {
                if (execvp(args[0], args) == -1)
                    printf("Error executing command\n");
            }
            else
                if (flag == 0)
                {wait(NULL);}
                else
                    waitpid(-1, NULL, WNOHANG);
            
        }
        memset(input_buffer, 0, sizeof input_buffer);
    }
    //memory dealocation for args
    for (var=0; var < Max_line / 2 +1 ; var++)
        free(args[0]);
    return (0);
    
}
