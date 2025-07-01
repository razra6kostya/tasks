#include "cmd_manager.h"
#include "word_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static int number_word(Word_item *word_item)
{
    int num = 0;
    while (word_item) {
        num++;
        word_item = word_item->next;
    }
    return num;
}

static int fork_and_exe(char **cmdline)
{
    int pid;
    pid = fork();
    if (pid == -1) {
        perror("fork\n"); 
        exit(1);
    }
    if (pid == 0) {
        execvp(cmdline[0], cmdline);
        perror(cmdline[0]);
        exit(1);
    }
    return pid;
}

static int wait_procces(int pid)
{
    int wr, status;
    wr = wait(&status);
    if(wr == -1) {
        printf("There are no child processes at all\n");
    } else {
        printf("Process with pid=%d finished.\n", wr);
        if (WIFEXITED(status)) {
            printf("It has exited with code=%d.\n",
                    WEXITSTATUS(status));
        }
    } 
    
    return 0;
}
int exec_cmd(Word_item *word_item)
{
    int i = 0;
    int num = number_word(word_item);
    char **cmdline = malloc(sizeof(char *) * (num+1));
    while (i < num && word_item) {
        cmdline[i] = word_item->word; 
        printf("CMD: %s\n", cmdline[i]);
        word_item = word_item->next;
        i++;
    }
    cmdline[i] = NULL;
    printf("CMD: %s\n", cmdline[i]);
    wait_procces(fork_and_exe(cmdline));
    return 0;
}
