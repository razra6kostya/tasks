#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

void search_file(const char *target_filename, const char *current_path)
{
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char full_path[1024];

    dir = opendir(current_path); 
    if (!dir) {
        perror("opendir");
        fprintf(stderr, "Could not open directory: %s\n", current_path);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".." to avoid infinite recursion
        if ((!strcmp(entry->d_name, ".")) || (!strcmp(entry->d_name, ".."))) {
            continue;
        }
        // Construct the full path to the current entry
        // snprintf is safer than sprintf as it prevents buffer overflows
        snprintf(full_path, sizeof(full_path), "%s/%s", current_path, entry->d_name); 
        // Get file status to determine if it's a file or directory
        stat(full_path, &file_stat);

        if(S_ISREG(file_stat.st_mode)) { /* this is file */
            if (!strcmp(entry->d_name, target_filename)) {
                printf("%s\n", full_path); 
            }
        } else {
            if (S_ISDIR(file_stat.st_mode)) { /* this is directory */
                search_file(target_filename, full_path); 
            }
        }
    }

    closedir(dir);
}

int main(int argc, char **argv)
{
    const char *current_dir_path = ".";
    const char *target_filename;

    if (argc != 2) {
        fprintf(stderr, "Needed two argument.\n");
        return 1;
    }
    target_filename = argv[1];

    search_file(target_filename, current_dir_path);
    return 0;
}
