#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <readline/history.h>
#include <readline/readline.h>

int main();
void instance(char *cwd, char *input);
char *readNextStrTok();
void ls(char *cwd);
void cd(char *cwd);

int main() {
    char cwd[1024];
    char *prompt = (char *)malloc(sizeof(cwd) + 14 * sizeof(char));
    pid_t pid;

    int bailout = 0;
    while (!bailout) {
        /* get current workding directory */
        getcwd(cwd, sizeof(cwd));
        sprintf(prompt, "\nSMrTSHll: %s > ", cwd);

        /* read stdin line in w/o prompt */
        char *reply = readline(prompt);

        /* catch exit request */
        if (!strcmp(reply, "bye") || !strcmp(reply, "quit") ||
            !strcmp(reply, "exit")) {
            bailout = 1;
        }
        else {
            /* show interpreted reply */
            printf("\nYou said: %s\n", reply);

            /* copying reply */
            char *replycpy[sizeof(reply)];
            strcpy(replycpy, reply);

            /* show tokenized reply */
            printf("\nTokenizing reply:\n");
            char *tokreply = strtok(replycpy, " ");
            while (tokreply != NULL) {
                printf("%s\n", tokreply);
                tokreply = readNextStrTok();
            }
            printf("\n");

            /* control input */
            if (sizeof(reply) / sizeof(char) >= 2 && reply[0] == 'b' &&
                reply[1] == 'g') {
                pid = fork();
                if (pid == 0) {
                    instance(cwd, &reply[3]);
                }
            }
        }

        free(reply);
    }

    free(prompt);
    printf("Bye Bye\n");
}

void instance(char *cwd, char *input) {
    char *tokinput = strtok(input, " ");
    printf("next token: %s\n\n", tokinput);

    if (tokinput != NULL) {
        if (!strcmp(tokinput, "ls")) {
            ls(cwd);
        }
        else if (!strcmp(tokinput, "cd")) {
            cd(cwd);
        }
        else {
            printf("Unknown command: %s\n", tokinput);
        }
    }
    return;
}

void ls(char *cwd) {
    printf("activated ls\n");
    DIR *dir;
    struct dirent *ent;

    dir = opendir(cwd);
    if (dir != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            switch (ent->d_type) {
                case DT_REG:
                    printf("%s\n", ent->d_name);
                    break;
                case DT_DIR:
                    printf("%s/\n", ent->d_name);
                    break;
                case DT_LNK:
                    printf("@%s\n", ent->d_name);
                    break;
                default:
                    printf("%s*\n", ent->d_name);
            }
        }
        closedir(dir);
    }
    return;
}

void cd(char *cwd) {
    printf("activated cd\n");
    DIR *dir;
    char *path = readNextStrTok();
    char nextpath[256];
    nextpath[0] = '\0';

    if (path[0] == '/') {
        strcpy(nextpath, path);
    }
    else if (path[0] == '.' && path[1] == '/') {
        path++;
        strcat(nextpath, cwd);
        strcat(nextpath, path);
    }
    else {
        strcat(nextpath, cwd);
        char tmp[] = "/";
        strcat(nextpath, tmp);
        strcat(nextpath, path);
    }
    printf("new path: %s\n", nextpath);

    dir = opendir(nextpath);
    if (dir) {
        chdir(path);
    }
    else if (ENOENT == errno) {
        printf("CRISIS ALERTN");
    }
    else {
        printf("I dunno");
    }
    return;
}

char *readNextStrTok() { return strtok(NULL, " "); }
