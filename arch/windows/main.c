#include <stdio.h>
#include <math.h>
//#include <readline/readline.h>
//#include <readline/history.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#include "parser.h"

extern bool __RUNNING;
extern bool __STOPPED;

static void sigint_handler(int signum)
{
	signal(SIGINT, sigint_handler);
	if (__RUNNING) {
		__RUNNING = false;
		__STOPPED = true;
		printf("STOP\n");
		fflush(stdout);
	}
}
#if 0
static char* readline_gets()
{
	char* line_read = (char*)readline("");
	if (line_read!=NULL && *line_read!='\0') {
		add_history(line_read);
	}

	return line_read;
}
#endif
int out(int ch)
{
	putchar(ch);
	return 1;
}

int in(void)
{
	return getchar();
}
extern char* gets(char*);
void repl(void)
{
	puts(" _               _      ");
	puts("| |__   __ _ ___(_) ___ ");
	puts("| '_ \\ / _` / __| |/ __|");
	puts("| |_) | (_| \\__ \\ | (__ ");
	puts("|_.__/ \\__,_|___/_|\\___|");
	puts("(c) 2015-2016 Johan Van den Brande");
	//using_history();

	char input[1024] = { 0 };
	while ((gets(input)) != NULL)
	{
		if (strcmp(input, "QUIT") == 0) {
			memset(input, 0, 1024);
			break;
		}

		basic_eval(input);

		if (evaluate_last_error()) {
			printf("ERROR: %s\n", evaluate_last_error());
			clear_last_error();
		}

		memset(input, 0, 1024);
	}

	//clear_history();
}

void run(char* file_name) 
{
	FILE* file = fopen(file_name, "r");

	if (file == NULL) {
		fprintf(stderr, "Can't open %s\n", file_name);
		return;
	}

	char line[tokenizer_string_length] = { 0 };
	while (fgets(line, sizeof(line), file)) {
		if (line[strlen(line) - 1] != '\n')
		{
			printf("ERROR: NO EOL\n");
			exit(1);
		}
		basic_eval(line);
	}
	fclose(file);

	basic_run();
}

int main(int argc, char* argv[])
{
	signal(SIGINT, sigint_handler);

	basic_init(1024 * 8, 2048);
	basic_register_io(out, in);

	if (argc > 1) {
		run(argv[1]);
	}
	else {
		repl();
	}

	basic_destroy();

	return EXIT_SUCCESS;
}
