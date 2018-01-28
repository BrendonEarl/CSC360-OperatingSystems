#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int main();
void instance();

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
		/* Note that readline strips away the final \n */
		/* For Perl junkies, readline automatically chomps the line read */

		if (!strcmp(reply, "bye") || !strcmp(reply, "quit"))
		{
			bailout = 1;
		}
		else
		{
			/* show interpreted reply */
			printf("\nYou said: %s\n", reply);

			/* show tokenized reply */
			printf("\nTokenizing reply:\n");
			char *tokRply = strtok(reply, " ");
			while (tokRply != NULL)
			{
				printf("%s\n", tokRply);
				tokRply = strtok(NULL, " ,.-");
			}
			printf("\n");

			/* control input */
			instance(tokRply);
		}

		free(reply);
	}

	free(prompt);
	printf("Bye Bye\n");
}

void instance(char *input)
{
	return
}
