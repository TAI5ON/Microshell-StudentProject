#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <string.h>


#define RESET "\x1B[0m"

#define BOLD "\e[1m"

#define GRAY "\x1B[30m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define PURPLE "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"

#define BOLD_GRAY "\e[1m\x1B[30m"
#define BOLD_RED "\e[1m\x1B[31m"
#define BOLD_GREEN "\e[1m\x1B[32m"
#define BOLD_YELLOW "\e[1m\x1B[33m"
#define BOLD_BLUE "\e[1m\x1B[34m"
#define BOLD_PURPLE "\e[1m\x1B[35m"
#define BOLD_CYAN "\e[1m\x1B[36m"
#define BOLD_WHITE "\e[1m\x1B[37m"

#define BACKGROUND_BLUE "\x1B[44m"
#define BACKGROUND_GREEN "\x1B[42m"



#define BUFF_SIZE 1024

/* Declarations - Implementations under main function */
void help();
void cd();
void clear();

int main() {

	/*get user information*/
	struct passwd* user;
	if ((user = getpwuid(getuid())) == NULL) {
		printf("User is null");
		return -1;
	}

	/*get username*/
	char *userName = user->pw_name;

	/*get path*/
	char path[BUFF_SIZE];
	getcwd(path, BUFF_SIZE);

	/* prints username and actual path and listens to input command in infinite loop
	until type exit */
	while (1) {

		/* [{username}:{path}] $ */
		printf(BACKGROUND_GREEN);
		printf(BOLD);
		printf("[");
		printf("%s", userName);
		printf(":");
		printf(BACKGROUND_BLUE);
		printf("%s", path);
		printf("]");
		printf(RESET);
		printf(BOLD_GRAY);
		printf(" $ ");
		printf(RESET);

		/* Takes user input */
		char input[BUFF_SIZE];
		fgets(input, BUFF_SIZE, stdin);

		/* remove "\n" */
		input[strcspn(input, "\n")] = 0;

		/* exit */
		if (strcmp(input, "exit") == 0) {
			break;
		}

		/* help */
		else if (strcmp(input, "help") == 0) {
			help();
		}

		/* cd */
		else if (strcmp(input, "cd") == 0) {
			cd(path);
		}

		/* clear */
		else if (strcmp(input, "clear") == 0) {
			clear();
		}

	}

    return 0;
}

/* help implementation */
void help () {
	printf("#############################################################\n");
	printf("#####################  FUNKCJONALNOŚCI  #####################\n");
	printf("#############################################################\n");
	printf("#             cd - przechodzi do wskazanego katalogu        #\n");
	printf("#           exit - kończy pracę Microshella                 #\n");
	printf("#           help - wyświetla informacje o autorze           #\n");
	printf("#                  i funkcjonalnościach                     #\n");
	printf("#          clear - czyści konsolę                           #\n");
	printf("#                                                           #\n");
	printf("# Aby dowiedzieć się więcej o komendach -> help --{komenda} #\n");
	printf("#                    Przykład: help --cd                    #\n");
	printf("#############################################################\n");
	printf("\n");
	printf("#############################################################\n");
	printf("##########################  AUTOR  ##########################\n");
	printf("#############################################################\n");
	printf("#              Imię i nazwisko: Michał Łuczak               #\n");
	printf("#                        Grupa: 15                          #\n");
	printf("#                Numer indeksu: 481871                      #\n");
	printf("#                      Kontakt: micluc2@st.amu.edu.pl       #\n");
	printf("#                       GitHub: https://github.com/TAI5ON   #\n");
	printf("#############################################################\n");
}

/* cd implementation */
void cd(char *path) {
	/* TODO */
}

/* clear implementation */
void clear() {
	printf("\e[1;1H\e[2J");
}
