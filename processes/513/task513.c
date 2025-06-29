#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define DELIMITER ";;"

struct item {
    char **cmdline;
    int cmdline_size;
    char *name;
    pid_t pid;
    struct item *next;
};

struct item *create_item(char **argv, int start, int end)
{
    int j, k;
    struct item *tmp = NULL;
    int num_args = end - start + 2;

    tmp = malloc(sizeof(struct item));
    tmp->name = argv[start];
    tmp->cmdline = malloc(sizeof(char *) * num_args);
    for (j = start, k = 0; j <= end; j++, k++) {
        tmp->cmdline[k] = argv[j];
    }
    tmp->cmdline_size = num_args;
    tmp->cmdline[k] = NULL;
    tmp->next = NULL;
    return tmp;
}

struct item *add_item_in_list(struct item *cmd_list, struct item *item)
{
    struct item *tmp;
    if (!cmd_list) { 
        cmd_list = item;
    } else { 
        tmp = cmd_list;
        while (tmp) {
            if (!tmp->next) {
                tmp->next = item;
                break;
            } else {
                tmp = tmp->next; 
            }
        }
    }
    return cmd_list;
}

struct item *parse_cmd_for_list(int argc, char **argv)
{
    struct item *cmd_list = NULL;
    int is_delimiter, is_end_args, cmd_args, i;
    int cmd_idx_start = 1;

    for (i = 1; i <= argc; i++) {
        cmd_args = 0;
        is_delimiter = (i < argc && 0 == strcmp(argv[i], DELIMITER)); 
        is_end_args = (i == argc);
        
        if (is_delimiter || is_end_args) {

            cmd_args = i - cmd_idx_start;
            if (cmd_args == 0) {
                if (is_delimiter) {
                    cmd_idx_start = i + 1;
                    continue;
                } else {
                    break;
                }
            }

            cmd_list = add_item_in_list(
                    cmd_list, 
                    create_item(
                        argv, 
                        cmd_idx_start, 
                        cmd_idx_start + cmd_args - 1
                    )
                );
            cmd_idx_start = i + 1;
        }
    }   
    return cmd_list;
}

char *print_name(struct item *cur, pid_t wr)
{
    while (cur) {
        if (cur->pid == wr) {
            return cur->name;
        } else {
            cur = cur->next;
        }
    }
    return "\0";
}

void delete_item_in_list(struct item **pcur, pid_t del_pid)
{
    while (*pcur) {
        if ((*pcur)->pid == del_pid) {
            struct item *tmp = *pcur;
            *pcur = (*pcur)->next;
            free(tmp->cmdline);
            free(tmp);
        } else {
            pcur = &(*pcur)->next;
        }
    }
}

int main(int argc, char **argv)
{
    int wr, pid, status;
    struct item *tmp = NULL, *cmd_list = NULL;
    
    cmd_list = parse_cmd_for_list(argc, argv);

    tmp = cmd_list;
    while (tmp) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) {
            execvp(tmp->name, tmp->cmdline);
            perror(tmp->name);
            exit(1);
        } 
        tmp->pid = pid;
        tmp = tmp->next;
    }
    
    do {
        wr = wait(&status);
        if (wr > 0 && WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("command: [%s] has exited wiht code=%d\n", 
                    print_name(cmd_list, wr), 
                    WEXITSTATUS(status));
        }
        delete_item_in_list(&cmd_list, wr);
    } while (wr > 0);
    
    return 0;
}
