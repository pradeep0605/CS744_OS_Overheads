#include<stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>

#define CONTROL 333
#define LIST_APPS 334
#define CLEAR_APPS_LIST 335
#define ADD_APP 336
int main(int argc, char **argv) {

	if (argc < 2) {
		printf("Usage: ./control [--add | --list-apps | --list-stats | --clear app_name |"
		" --clear-all | --ctrl #number | --reset-stats app_name | --reset-stats-all ]\n");
		printf("--ctrl #number options: \n1) enabled stack dump (-1) for disabling ]\n");
		return 0;
	}
	
	if (strcmp(argv[1], "--add") == 0) {
	 	if (argc == 3) {
			syscall(ADD_APP, &argv[2]);
		} else {
			printf("Invalid number of arguments for %s\n", argv[1]);
		}
	} else if (strcmp(argv[1], "--list-stats") == 0) {
		printf("Calling sys_list_apps syscall. Returned %x\n",
			(unsigned int) syscall(LIST_APPS));
	} else if (strcmp(argv[1], "--list-apps") == 0) {
		printf("Calling sys_list_apps syscall. Returned %x\n",
			(unsigned int) syscall(LIST_APPS));
	} else if (strcmp(argv[1], "--clear-all") == 0) {
		printf("Calling sys_clear_list syscall. Returned %x\n",
			(unsigned int) syscall(CLEAR_APPS_LIST, NULL, 1));
	} else if (strcmp(argv[1], "--clear") == 0) {
	 	if (argc == 3) {
			printf("Calling sys_clear_list syscall. Returned %x\n",
				(unsigned int) syscall(CLEAR_APPS_LIST, &argv[2], 1));
		} else {
			printf("Invalid number of arguments for %s\n", argv[1]);
		}
	} else if (strcmp(argv[1], "--reset-stats") == 0 ) {
		if (argc == 3) {
			printf("Calling sys_clear_apps syscall. Returned %x\n",
				(unsigned int) syscall(CLEAR_APPS_LIST, &argv[2], 2));
		} else {
			printf("Invalid number of arguments for %s\n", argv[1]);
		}
	} else if (strcmp(argv[1], "--reset-stats-all") == 0 ) {
			printf("Calling sys_clear_apps syscall. Returned %x\n",
				(unsigned int) syscall(CLEAR_APPS_LIST, NULL, 2));
	}
	
	else {
		printf("Invalid argument %s\n", argv[1]);
	}
	return 0;
}
