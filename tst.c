#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define BUFLEN 40

time_t start_time;

void cleanup(void);
void toggle_term_mode(int term);

/* COMMANDS */

void write_time(void)
{
	time_t run_length;
	char buf[BUFLEN];

	printf("\033[2K\r"); /* clear line */
	run_length = time(NULL) - start_time;
	strftime(buf, BUFLEN, "%H:%M:%S", gmtime(&run_length));
	printf("%s", buf);
}

/* INIT */

void cleanup(void)
{
	toggle_term_mode(0);
}

/* when term is 1, turn off canonical mode. turn on when 0 */
void toggle_term_mode(int term)
{
	static struct termios orig, new;

	if (term == 1) {
		/* turn off icanon */
		tcgetattr(STDIN_FILENO, &orig);
		new = orig;
		new.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &new);
	} else {
		/* turn on icanon */
		tcsetattr(STDIN_FILENO, TCSANOW, &orig);
	}
}

int main(int argc, char **argv)
{
	atexit(cleanup);
	toggle_term_mode(1);

	int c;

	printf("h\n\n");
	start_time = time(NULL);
	while ((c = getchar()) != EOF && c != 3) { /* the second one is C-c */
		switch (c) {
			case ' ':
				write_time();
				break;
			case '\n':
				printf("\n");
				break;
			case 'q':
				ungetc(3, stdin); /* bodge solution */
				break;
		}
	}
	printf("\n");
	return 0;
}
