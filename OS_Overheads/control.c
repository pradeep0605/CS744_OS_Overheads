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

int main(int argc, char **argv) {

	printf("Return val = %ld\n", syscall(CONTROL, 123456));
	return 0;

	if (argc < 2) {
		printf("Usage: ./control [--add | --list | --clear app_name |"
		" --clear-all | --ctrl #number ]\n");
		printf("--ctrl #number options: \n1) enabled stack dump (-1) for disabling ]\n");
		return 0;
	}
	
	if (strcmp(argv[1], "--add") == 0) {
	 	if (argc == 3) {
			syscall(CONTROL, &argv[2], argc-2, 1);
		} else {
			printf("Invalid number of arguments for %s\n", argv[1]);
		}

	} else if (strcmp(argv[1], "--list") == 0) {
		printf("Calling sys_list_apps syscall. Returned %x\n",
			(unsigned int) syscall(LIST_APPS));
	} else if (strcmp(argv[1], "--clear-all") == 0) {
		printf("Calling sys_clear_list syscall. Returned %x\n",
			(unsigned int) syscall(CLEAR_APPS_LIST, NULL));
	} else if (strcmp(argv[1], "--clear") == 0) {
	 	if (argc == 3) {
			printf("Calling sys_clear_list syscall. Returned %x\n",
				(unsigned int) syscall(CLEAR_APPS_LIST, argv[2]));
		} else {
			printf("Invalid number of arguments for %s\n", argv[1]);
		}
	} else {
		printf("Invalid argument %s\n", argv[1]);
	}
	
	return 0;
}
