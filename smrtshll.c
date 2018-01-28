#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int main();
void instance();
char *readNextStrTok();

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
		if (!strcmp(reply, "bye") || !strcmp(reply, "quit"))
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
			printf("%s\n\n", tokreply);
			tokreply = strtok(reply, " ");
			instance(tokreply);
		}

		free(reply);
	}

	free(prompt);
	printf("Bye Bye\n");
}

void instance(char *input)
{
	char *nextstrtok = readNextStrTok();
	printf("next token: %s\n", nextstrtok);
	if (nextstrtok != NULL && !strcmp(nextstrtok, "cd"))
	{
		printf("cd selected");
	}
	return;
}

void cd(char *input)
{
	return;
}

char *readNextStrTok()
{
	return strtok(NULL, " ");
}
