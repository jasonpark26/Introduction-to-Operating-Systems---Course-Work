
/*
 * Assignment 1
 * CSC 360, Summer 2021
 * Jason Park
 * V00946007
 */

#define MAX_INPUT_LINE 80
#define MAX_NUM_PROMPT 10
#define MAX_NUM_DIR 10
#define MAX_NUM_ARG 8

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char paths[MAX_NUM_DIR][MAX_INPUT_LINE]; //global variable for paths from .vshrc
char *token[MAX_NUM_ARG]; //global variable for tokenized string
char *t;
int num_tokens; // number of tokens should not exceed 8
int i;
int line_len;

// CHECKS TO SEE IF A STRING STARTS WITH "::"
int prefix(const char *str)
{
    return strncmp("::", str, 2) == 0;
    // returns 1 if the string has a prefix of ::
}

//CHECKS TO SEE IF A STRING ENDS WITH "::"
//https://stackoverflow.com/questions/10347689/how-can-i-check-whether-a-string-ends-with-csv-in-c
int suffix(const char *string)
{
    string = strrchr(string, ":");
    if( string != NULL )
        return (strcmp(string, "::")) == 0;
    return( -1 );
}


int main(int argc, char *argv[]) {
    char *args[MAX_NUM_ARG];
    char *envp[] = { 0 };
    char input[MAX_INPUT_LINE];
    int pid;
    int status;
    char *token[MAX_NUM_ARG + 1];
    char *t;
    int  num_tokens;
    int prefix_flag = 0; //flag to check if a string starts with ::
    int suffix_flag = 0; //flag to check if a string ends with ::
    int time_flag = 0; //flag to check if token ends with ##
    char output_file[] = ""; //string to hold where the outout would go to
    char input_file[] = ""; //string to hold where the input would be from
    struct timeval before, after; // from appendix_f.c
    int fd;

    // from appendix_a.c
    for(;;) {
            fprintf(stdout, "vsh%% ");
            fflush(stdout);
            fgets(input, MAX_INPUT_LINE, stdin);
            if (input[strlen(input) - 1] == '\n') {
                input[strlen(input) - 1] = '\0';
            }

            if (strcmp(input, "exit") == 0) {
                exit(1);
            }

            if (strlen(input) > 80){
                fprintf(stderr, "Input Exceeds max character length\n");
            }
            //CREATES AN ARRAY THAT READS FROM THE VSHRC FILES
            // https://stackoverflow.com/questions/4237084/reading-from-a-file-and-storing-in-array
            int i = 0;
            FILE* fp;
            fp = fopen(".vshrc-1", "r");
            if (fp == 0)
            {
                fprintf(stderr, "failed to open text file\n");
                exit(1);
            }
            while(i < MAX_NUM_DIR && fgets(paths[i], sizeof(paths[0]), fp)){
                paths[i][strlen(paths[i]) - 1] = '\0';
                // adds / to end of path to concatenate it easier with tokens
                strcat(paths[i], "/");
                i = i + 1;
            }
            // -----------------------------------------------


            //TOKENIZES THE INPUT FROM THE USER
            // from appendix_e.c
            t = strtok(input, " ");
            while (t != NULL && num_tokens < 10) {
                token[num_tokens] = t;
                num_tokens++;
                t = strtok(NULL, " ");
            }


            // CHECKS TO SEE IF THE TOKENIZED STRINGS STARTS WITH ::
            // IF IT SEES THAT A STRING STARTS WTIH :: SET FLAG TO 1
            // IF IT SEES STRING WITH :: MARKS THAT AS OUTPUT
            // THEN CREATES NEW ARGS WITHOUT ::
            for (i = 0; i < num_tokens; i++) {
                if (prefix(token[i]) == 1) {
                    prefix_flag = 1;
                    //strcpy(output_file, (token[i]+2));
                    token[i] = token [i+1];
                }
            }

            // CHECKS TO SEE IF THE TOKENIZED STRINGS ENDS WITH ::
            // IF IT SEES THAT A STRING  WTIH :: SET FLAG TO 1
            // IF IT SEES STRING WITH :: MARKS THAT AS INPUT
            // THEN CREATES NEW ARGS WITHOUT ::
            for (i = 0; i < num_tokens; i++) {
                if (suffix(token[i]) == 1) {
                    suffix_flag = 1;
                    //char *input_file = token[i] + 2;
                    token[i] = token [i+1];
                }
            }

            // CHECKS TO SEE IF THE TOKEN ENDS WITH TIME
            for (i = 0; i < num_tokens; i++) {
                if (strcmp(token[i], "##") == 0) {
                    time_flag = 1;
                    // make ## NULL as it is not an argument
                }
                //NEED TO CHECK IF TIME IS ONLY THE LAST STRING ELSE ERROR
            }

            printf("%s\n", output_file);

            //IF THERE ARE NO FLAGS AT ALL, RUN AS NORMAL CMD
            //FORKING AND RUNNINC EXECVE FROM appendix_b.c
            if ((prefix_flag == 0) && (suffix_flag == 0) && (time_flag == 0)){
                if (num_tokens > 8) {
                    fprintf(stderr, "Arguments exceeds max length\n");
                }
                if ((pid = fork()) == 0) {
                    strncpy(args, token, sizeof(args));
                    args[0] = strcat(paths[0], args[0]);
                    if (execve(args[0], args, envp) == -1){
                        fprintf(stderr, "Execve failed to run\n");
                    }
                }
                while (wait(&status) > 0) {}
            }

            //IF THERE IS ONLY THE FLAG FOR PREFIX
            //FORKING AND RUNNINC EXECVE FROM appendix_b.c
            //OUTPUTTING TO FILE FROM appendix_c.c
            if ((prefix_flag == 1) && (suffix_flag == 0) && (time_flag == 0)){
                if (num_tokens > 8) {
                    fprintf(stderr, "Arguments exceeds max length\n");
                }
                if ((pid = fork()) == 0) {
                    fd = open(output_file, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
                    if (fd == -1) {
                        fprintf(stderr, "cannot open text file for writing\n");
                        exit(1);
                    }
                    strncpy(args, token, sizeof(args));
                    args[0] = strcat(paths[0], args[0]);
                    dup2(fd, 1);
                    dup2(fd, 2);
                    if (execve(args[0], args, envp) == -1){
                        fprintf(stderr, "Execve failed to run\n");
                    }
                }
                while (wait(&status) > 0) {}
            }

            //IF THERE IS A FLAG FOR PREFIX AND TIME
            //FORKING AND RUNNINC EXECVE FROM appendix_b.c
            //TIME ASPECTS FROM appendix_f.c
            if ((prefix_flag == 1) && (suffix_flag == 0) && (time_flag == 1)){
                if (num_tokens > 8) {
                    fprintf(stderr, "Arguments exceeds max length\n");
                }
                if ((pid = fork()) == 0) {
                    strncpy(args, token, sizeof(args));
                    args[0] = strcat(paths[0], args[0]);
                    gettimeofday(&before, NULL);
                    if (execve(args[0], args, envp) == -1){
                        fprintf(stderr, "Execve failed to run\n");
                    }
                    gettimeofday(&after, NULL);
                    printf("The process took %lu microseconds\n",
                        (after.tv_sec - before.tv_sec) * 1000000 +
                            after.tv_usec - before.tv_usec);
                }
                while (wait(&status) > 0) {}

            }
            fclose(fp);
    }
}
