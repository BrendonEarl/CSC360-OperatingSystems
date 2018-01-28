#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>

int main();
void instance(char *cwd, char *input);
char *readNextStrTok();
void ls(char *cwd);
void cd(char *cwd);

int main()
{
	char cwd[1024];
	char *prompt = (char *)malloc(sizeof(cwd) + 14 * sizeof(char));

	int bailout = 0;
	while (!bailout)
	{
		/* get current workding directory */
		getcwd(cwd, sizeof(cwd));
		sprintf(prompt, "\nSMrTSHll: %s > ", cwd);

		/* read stdin line in w/o prompt */
		char *reply = readline(prompt);

		/* catch exit request */
		if (!strcmp(reply, "bye") || !strcmp(reply, "quit") || !strcmp(reply, "exit"))
		{
			bailout = 1;
		}
		else
		{
			/* show interpreted reply */
			printf("\nYou said: %s\n", reply);

			/* copying reply */
			char *replycpy[sizeof(reply)];
			strcpy(replycpy, reply);

			/* show tokenized reply */
			printf("\nTokenizing reply:\n");
			char *tokreply = strtok(replycpy, " ");
			while (tokreply != NULL)
			{
				printf("%s\n", tokreply);
				tokreply = readNextStrTok();
			}
			printf("\n");

			/* control input */
			instance(reply);
		}

		free(reply);
	}

	free(prompt);
	printf("Bye Bye\n");
}

void instance(char *cwd, char *input)
{
	char *tokinput = strtok(input, " ");
	printf("in instance\n");
	printf("next token: %s\n", tokinput);
	if (tokinput != NULL && !strcmp(tokinput, "ls"))
	{
		printf("ls selected");
		ls(cwd);
	}
	if (tokinput != NULL && !strcmp(tokinput, "cd"))
	{
		printf("cd selected");
		cd(cwd);
	}
	return;
}

void ls(char *cwd)
{
	return;
}

void cd(char *cwd)
{
	printf("activated cd\n");
	char *path = readNextStrTok();
	printf("new path: %s\n", path);
	return;
}

char *readNextStrTok()
{
	return strtok(NULL, " ");
}
