#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <string.h>

int main() {
	struct passwd* user;

	if ((user = getpwuid(getuid())) == NULL) {
		printf("User is null");
		return -1;
	}

	char *userName = user->pw_name;


	char path[1024];
	getcwd(path, 1024);

	while (1) {
		printf("[%s@%s] $", userName, path);

		char input[1024];
		scanf("%s", input);

		if (strcmp(input, "exit") == 0) {
			printf("Do widzenia :)\n");
			break;
		}
	}

    return 0;
  }