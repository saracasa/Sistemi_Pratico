/* CHAT
gcc -o search_name Es\ 1.c
./search_name testprog
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

void search_name(const char *dir_path, const char *target_name) {
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char path[1024];
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
        struct stat st;
        if (stat(path, &st) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            search_name(path, target_name);
        } else if (S_ISREG(st.st_mode) && strcmp(entry->d_name, target_name) == 0) {
            if (access(path, X_OK) == 0) {
                FILE *file = fopen(path, "rb");
                if (file == NULL) {
                    perror("fopen");
                    continue;
                }
                char header[5] = {0};
                fread(header, 1, 4, file);
                fclose(file);

                if (memcmp(header, "#!", 2) == 0) {
                    printf("%s: script\n", path);
                } else if (memcmp(header, "\x7f" "ELF", 4) == 0) {
                    printf("%s: ELF executable\n", path);
                } else {
                    printf("%s: unknown executable type\n", path);
                }
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    search_name(".", argv[1]);
    return 0;
}
