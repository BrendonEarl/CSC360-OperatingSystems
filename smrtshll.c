#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int main()
{
	char cwd[1024];
	char *prompt = (char *)malloc(sizeof(cwd) + 14 * sizeof(char));

	int bailout = 0;
	while (!bailout)
	{
		/* get current workding directory */
		getcwd(cwd, sizeof(cwd));
		sprintf(prompt, "SMrTSHll: %s > ", cwd);

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
			printf("\nYou said: %s\n\n", reply);
		}

		free(reply);
	}
	free(prompt);
	printf("Bye Bye\n");
}
