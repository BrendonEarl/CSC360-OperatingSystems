#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <readline/history.h>
#include <readline/readline.h>

int main();
void instance(char *cwd, int argc, char *argv[]);
char *readNextStrTok();
void basic(char *cwd, int argc, char *argv[]);
void cd(char *cwd, char *path);

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

            /* count arguments (using copy) */
            int argc = 0;
            char *cpyreplytok = strtok(replycpy, " ");
            if (cpyreplytok != NULL) {
                while (cpyreplytok != NULL) {
                    cpyreplytok = readNextStrTok();
                    argc++;
                }
            }

            /* tokenize reply */
            char *argv[argc];
            char *replytok = strtok(reply, " ");
            argv[0] = replytok;
            for (int i = 1; replytok != NULL; i++) {
                replytok = readNextStrTok();
                argv[i] = replytok;
            }

            /* control input */
            if (sizeof(reply) / sizeof(char) >= 2 && reply[0] == 'b' &&
                reply[1] == 'g') {
                pid = fork();
                if (pid == 0) {
                    instance(cwd, argc, argv);
                }
            }
            else {
                instance(cwd, argc, argv);
            }
        }

        free(reply);
    }

    free(prompt);
    printf("Bye Bye\n");
}

/* instance - handles args
 * @cwd {char *} - current working directory string
 * @argc {int} - number of arguments
 * @argv {char* []} pointer to array of arguments
 */
void instance(char *cwd, int argc, char *argv[]) {
    if (argc >= 1) {
        if (!strcmp(argv[0], "cd") && argc >= 2) {
            cd(cwd, argv[1]);
        }
        else {
            basic(cwd, argc, argv);
        }
    }
    return;
}

/* basic - calls command passed in with arguments using execvp
 * @cwd {char *} - current working directory string
 * @argc {int} - number of arguments
 * @argv {char* []} pointer to array of arguments
 */
void basic(char *cwd, int argc, char *argv[]) {
    /* snippet inspired by:
     * http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html
     */
    // fork and execute command passed in
    pid_t pid = fork();
    if (pid < 0) {
        printf("Error occured when forking");
    }
    else if (pid == 0) {
        if (execvp(argv[0], argv) < 0) {
            printf("Error occurent whith execvp");
        }
    }
    else {
        while (waitpid(pid, NULL, WUNTRACED) > 0)
            ;
    }

    return;
}

/* cd - change directory
 * @cwd {char *} - current working directory string
 * @path {char *} - path argument to move directories
 */
void cd(char *cwd, char *path) {
    DIR *dir;
    char nextpath[256];
    nextpath[0] = '\0';

    // handle cd based on start characters
    if (path == NULL) {
        strcat(nextpath, cwd);
        char tmp[] = "/..";
        strcat(nextpath, tmp);
    }
    else if (path[0] == '~') {
        char *tmp = getenv("HOME");
        strcat(nextpath, tmp);
    }
    else if (path[0] == '/') {
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

    /* snippet inspired by:
     * https://stackoverflow.com/questions/12510874/how-can-i-check-if-a-directory-exists
     */
    // check if path exists, and move if so
    dir = opendir(nextpath);
    if (dir) {
        chdir(nextpath);
    }
    else if (ENOENT == errno) {
        printf("Path does not exist");
    }
    else {
        printf("Unexpected error has occured");
    }
    return;
}

char *readNextStrTok() { return strtok(NULL, " "); }
