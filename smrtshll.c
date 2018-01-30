#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <sys/mman.h>

typedef enum bool {
    False,
    True,
} bool;

typedef struct bgp bgp;

struct bgp {
    pid_t pid;
    bgp *next;
    char *argvstr;
    bool done;
    char *outputbuff;
};

typedef struct bgps {
    bgp *head;
    int count;
} bgps;

int main();
bgp *getbgpstail(bgp *bgprocess);
void addbgp(bgps *bgprocesses, bgp *bgprocess);
void printbgpsstatus(bgps *bgprocesses);
void deletebgps(bgps bgprocesses);
void instance(char *cwd, int argc, char *argv[], bgp *process);
void basic(char *cwd, int argc, char *argv[]);
void cd(char *cwd, char *path);
char *readNextStrTok();
char *printbuff(char *buff, char *input);

int main() {
    char cwd[1024];
    char *prompt = (char *)malloc(sizeof(cwd) + 14 * sizeof(char));
    pid_t pid;
    bgps bgprocesses = {.head = NULL, .count = 0};

    bool bailout = False;
    while (bailout == False) {
        /* get current workding directory */
        getcwd(cwd, sizeof(cwd));
        sprintf(prompt, "\nSMrTSHll: %s > ", cwd);

        /* read stdin line in w/o prompt */
        char *reply = readline(prompt);
        printf("\n");

        /* catch exit request */
        if (!strcmp(reply, "bye") || !strcmp(reply, "quit") ||
            !strcmp(reply, "exit")) {
            bailout = True;
        }
        else {
            /* copying reply */
            char *replycpy = (char *)malloc(sizeof(reply));
            strcpy(replycpy, reply);
            char *argvstr = (char *)malloc(sizeof(reply));
            strcpy(argvstr, reply);

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
            int i;
            for (i = 1; replytok != NULL; i++) {
                replytok = readNextStrTok();
                argv[i] = replytok;
            }

            /* control input */
            if (!strcmp(reply, "bglist")) {
                printbgpsstatus(&bgprocesses);
            }
            else if (sizeof(reply) / sizeof(char) >= 2 && reply[0] == 'b' &&
                     reply[1] == 'g') {
                /* snippet inspired by:
                 * https://stackoverflow.com/questions/13274786/how-to-share-memory-between-process-fork
                 */
                bgp *nextbgp =
                    mmap(NULL, sizeof nextbgp, PROT_READ | PROT_WRITE,
                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);

                /* assign values to background process struct */
                nextbgp->next = NULL;
                nextbgp->argvstr = (char *)malloc(sizeof(argvstr));
                strcpy(nextbgp->argvstr, &argvstr[2]);
                nextbgp->done = False;
                nextbgp->outputbuff = (char *)malloc(sizeof(4096));

                /* start fork */
                pid = fork();

                /* child */
                if (pid == 0) {
                    instance(cwd, argc - 1, &argv[1], nextbgp);
                    break;
                } /* parent */
                else {
                    nextbgp->pid = pid;
                    addbgp(&bgprocesses, nextbgp);
                    /* wait - to be taken out once buffers handled */
                    while (waitpid(pid, NULL, WUNTRACED) > 0)
                        ;
                }
            }
            else {
                instance(cwd, argc, argv, NULL);
            }

            free(replycpy);
            free(argvstr);
        }

        free(reply);
    }

    /* tidy up */
    deletebgps(bgprocesses);
    free(prompt);
    if (bailout == True)
        printf("Bye Bye\n");
}

/* addbgp - add background process onto linked list
 * @bgprocesses {bgps *} - background process struct
 * @bgprocess {bgp *} - background process to be added
 */
void addbgp(bgps *bgprocesses, bgp *bgprocess) {
    /* if first to be added to list */
    if (bgprocesses->head == NULL) {
        bgprocesses->count = 1;
        bgprocesses->head = bgprocess;
    }
    else { /* if adding onto existing list */
        bgp *bgptail = getbgpstail(bgprocesses->head); // find tail
        bgptail->next = bgprocess;                     // assign as next on
        bgprocesses->count += 1;
    }
}

/* deletebgps - iteritively frees background process structs on call
 * @bgprocesses {bgps *} - background process struct
 * @returns {bgp *} - tail of background processes linked list
 */
bgp *getbgpstail(bgp *bgprocess) {
    if (bgprocess->next == NULL)
        return bgprocess;
    else
        return getbgpstail(bgprocess->next);
}

/* printbgpsstatus - print background process onto console
 * @bgprocesses {bgps *} - background process struct
 */
void printbgpsstatus(bgps *bgprocesses) {
    int count = 0;
    bgp *bgpcursor = bgprocesses->head;
    /* while there are processes to iterate over */
    while (bgpcursor != NULL) {
        /* build string for process list */
        char pidstr[8];
        sprintf(pidstr, "%d: ", bgpcursor->pid);
        char donestr[16];
        if (bgpcursor->done == True) {
            strcpy(donestr, "has terminated.");
        }
        else {
            count += 1;
            strcpy(donestr, "");
        }
        /* print process string */
        printf("%8s %s %s\n", pidstr, bgpcursor->argvstr, donestr);
        bgpcursor = bgpcursor->next;
    }
    printf("Total background jobs: %d\n", count);
}

/* deletebgps - iteritively frees background process structs on call
 * @bgprocesses {bgps} - background process struct
 */
void deletebgps(bgps bgprocesses) {
    bgp *bgpcursor = bgprocesses.head;
    /* while there are processes to iterate over */
    while (bgpcursor != NULL) {
        bgp *tmpbgpcursor = bgpcursor;
        bgpcursor = bgpcursor->next;
        /* free & unmap memory used */
        free(tmpbgpcursor->argvstr);
        free(tmpbgpcursor->outputbuff);
        munmap(tmpbgpcursor, sizeof *tmpbgpcursor);
    }
}

/* instance - handles args
 * @cwd {char *} - current working directory string
 * @argc {int} - number of arguments
 * @argv {char* []} pointer to array of arguments
 * @process {bgp *} pointer background process struct
 */
void instance(char *cwd, int argc, char *argv[], bgp *process) {
    if (argc >= 1) {
        if (!strcmp(argv[0], "cd") && argc >= 2) {
            cd(cwd, argv[1]);
        }
        else {
            basic(cwd, argc, argv);
        }
        if (process != NULL) {
            process->done = True;
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
            printf("Invalid command");
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

char *printbuff(char *buff, char *input) {
    strcat(buff, input);
    return buff;
}
