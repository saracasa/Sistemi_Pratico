//CHAT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 128

void search_name(const char *dir_path, const char *target_name);
void check_executable(const char *file_path);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    search_name(".", argv[1]);
    return 0;
}

void search_name(const char *dir_path, const char *target_name) {
    struct dirent *entry;
    DIR *dir = opendir(dir_path);

    if (!dir) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        char path[BUFFER_SIZE];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                search_name(path, target_name);
            }
        } else if (entry->d_type == DT_REG) {
            if (strcmp(entry->d_name, target_name) == 0) {
                check_executable(path);
            }
        }
    }

    closedir(dir);
}

void check_executable(const char *file_path) {
    struct stat statbuf;
    if (stat(file_path, &statbuf) == -1) {
        perror("stat");
        return;
    }

    if (statbuf.st_mode & S_IXUSR) {
        int fd = open(file_path, O_RDONLY);
        if (fd == -1) {
            perror("open");
            return;
        }

        char buffer[5] = {0};
        if (read(fd, buffer, 4) == -1) {
            perror("read");
            close(fd);
            return;
        }

        if (strncmp(buffer, "#!", 2) == 0) {
            printf("%s: script\n", file_path);
        } else if (strncmp(buffer, "\x7f""ELF", 4) == 0) {
            printf("%s: ELF executable\n", file_path);
        } else {
            printf("%s: other executable\n", file_path);
        }

        close(fd);
    }
}
