#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <string.h>
#include <sys/wait.h>
#include <ftw.h>
#include <readline/readline.h>
#include <readline/history.h>

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

/* Declarations - (Implementations under main function) */
void help();
void cd();
void clear();
void cp();
void rm();

int remove_dir();


int main() {

	/*get user information*/
	struct passwd *user;
	if ((user = getpwuid(getuid())) == NULL) {
		printf("User is null");
		return -1;
	}

	/*get username*/
	char *userName = user -> pw_name;
	char *userHomeDir = user -> pw_dir;

	/* prints username and actual path and listens to input command in infinite loop until type exit */
	while (1) {

		/*get path*/
		char path[BUFF_SIZE];
		getcwd(path, BUFF_SIZE);

		/* [{username}:{path}] $ */
		char userNamePath[BUFF_SIZE] = BACKGROUND_GREEN;
		strcat(userNamePath, BOLD);
		strcat(userNamePath, "[");
		strcat(userNamePath, userName);
		strcat(userNamePath, ":");
		strcat(userNamePath, BACKGROUND_BLUE);
		strcat(userNamePath, path);
		strcat(userNamePath, "]");
		strcat(userNamePath, RESET);
		strcat(userNamePath, BOLD_GRAY);
		strcat(userNamePath, " $ ");
		strcat(userNamePath, RESET);

		/* Takes user input I used readline() because it allows you to step through the command history*/
		char *input = readline(userNamePath);
		
		/* Add input to history of inputs */
		add_history(input);

		/* Divides input for single words */
		char *word = strtok(input, " \n");
		int counter = 0;
		char *command[255];
		while (word != NULL) {
			command[counter] = word;
			word = strtok(NULL, " \n");
			counter++;
		}
    
		if (counter == 0) {
			continue;
		}

		command[0] = strtok(command[0], " \n");
    	command[counter] = NULL;

		/* exit */
		if (strcmp(command[0], "exit") == 0) {
			exit(0);
		}

		/* help */
		else if (strcmp(command[0], "help") == 0) {
			help();
		}

		/* cd */
		else if (strcmp(command[0], "cd") == 0) {
			cd(command, userHomeDir);
		}

		/* cp */
		else if (strcmp(command[0], "cp") == 0) {
			cp(command[1], command[2]);
		}

		/* rm */
		else if (strcmp(command[0], "rm") == 0) {
			rm(command);
		}

		/* clear */
		else if (strcmp(command[0], "clear") == 0) {
			clear();
		}

		else {
			switch(fork()) {
				case 0: {
					int result = execvp(command[0], command);
					if (result != 0) {
						perror("Error");
					}
					exit(result);
					break;
				}
				case -1: {
					printf("Error while executing fork function!");
					break;
				}
				default: {
					wait(NULL);
					break;
				}
			}
		}
	}

    return 0;
}

/* help implementation */
void help () {
	printf("%s#############################################################\n", BOLD_YELLOW);
	printf("#####################  %sFUNKCJONALNOŚCI%s  #####################\n", BOLD_CYAN, BOLD_YELLOW);
	printf("#############################################################\n");
	printf("#             %scd - przechodzi do wskazanego katalogu%s        #\n", BOLD_CYAN, BOLD_YELLOW);
	printf("#           %sexit - kończy pracę Microshella%s                 #\n", BOLD_CYAN, BOLD_YELLOW);
	printf("#           %shelp - wyświetla informacje o autorze%s           #\n", BOLD_CYAN, BOLD_YELLOW);
	printf("#                  %si funkcjonalnościach%s                     #\n", BOLD_CYAN, BOLD_YELLOW);
	printf("#          %sclear - czyści konsolę%s                           #\n", BOLD_CYAN, BOLD_YELLOW);
	printf("#             %scp - kopiuje zawartość pliku do drugiego%s      #\n", BOLD_CYAN, BOLD_YELLOW);
	printf("#             %srm - usuwa plik o podanej nazwie%s              #\n", BOLD_CYAN, BOLD_YELLOW);
	printf("#############################################################\n");
	printf("\n");
	printf("#############################################################\n");
	printf("##########################  %sAUTOR%s  ##########################\n", BOLD_CYAN, BOLD_YELLOW);
	printf("#############################################################\n");
	printf("#              %sImię i nazwisko: Michał Łuczak%s               #\n", BOLD_CYAN, BOLD_YELLOW);
	printf("#                        %sGrupa: 15%s                          #\n", BOLD_CYAN, BOLD_YELLOW);
	printf("#                %snumer indeksu: 481871%s                      #\n", BOLD_CYAN, BOLD_YELLOW);
	printf("#                      %sKontakt: micluc2@st.amu.edu.pl%s       #\n", BOLD_CYAN, BOLD_YELLOW);
	printf("#                       %sGitHub: https://github.com/michal-luczak%s   #\n", BOLD_CYAN, BOLD_YELLOW);
	printf("#############################################################\n");
}

/* cd implementation */
void cd(char **command, char *userHomeDir) {
	if (command[1] == NULL || (strcmp(command[1], "~") == 0)) {
		chdir(userHomeDir);
		return;
	}

	if (chdir(command[1]) != 0) {
		perror("Nie udało się zmienić katalogu");
		return;
	} 
	
}

/* clear implementation */
void clear() {
	printf("\e[1;1H\e[2J");
}

/* cp implementation */
void cp(char *source, char *destination) {

    if (source == NULL || destination == NULL) {
        printf("Nieprawidłowa liczba argumentów. Poprawne użycie: cp {źródło} {cel}\n");
        return;
    }

    FILE* sourceFile = fopen(source, "rb");
    FILE* destinationFile = fopen(destination, "wb");
    if (sourceFile == NULL) {
        perror("Nie udało się otworzyć pliku źródłowego");
        return;
    }
    if (destinationFile == NULL) {
        perror("Nie udało się otworzyć pliku docelowego");
        fclose(sourceFile);
        return;
    }

    char buffer[4096];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
        fwrite(buffer, 1, bytesRead, destinationFile);
    }

    fclose(sourceFile);
    fclose(destinationFile);
}

/* rm implemetation */
void rm(char**command) {
	
	int r = 0;
	int f = 0;

	int i=1;
	char *temp = command[1];

	while(temp != NULL) {
		if (strcmp(command[i], "-f") == 0) {
			f = 1;
		}
		if (strcmp(command[i], "-r") == 0) {
			r = 1;
		}
		i++;
		temp = command[i];
	}
	
    if (remove(command[1]) != 0 && f==0 && r==0) {
        perror("Nie udało się usunąć pliku");
		return;
    } else if (remove(command[1]) !=0 && r==1) {
		nftw(command[1], remove_dir, 64, FTW_DEPTH | FTW_PHYS);
	}
}

/* Function that is executed every time that function nftw go into folder */
int remove_dir(const char *path, const struct stat *s, int flag, struct FTW *ftw) {
	if (remove(path) != 0) {
		perror("Nie udało się usunąć pliku");
		return -1;
	}
}
