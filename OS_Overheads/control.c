#include<stdio.h>
#include <fcntl.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CONTROL 333
#define LIST_APPS 334
#define CLEAR_APPS_LIST 335
#define ADD_APP 336

#define N_SYSCALLS (1000 * 1000 * 2)
#define BILLION (1000000000)

inline unsigned long get_current_time(void) {
	unsigned long int time = 0;
	struct timespec tv;

	clock_gettime(CLOCK_MONOTONIC, &tv);

	time = ((tv.tv_sec * BILLION) + tv.tv_nsec);
	return time;
}

void measure_syscall_overhead() {
	int i = 0;
	unsigned long sum = 0, start = 0, end = 0;

	for(i = 0; i < N_SYSCALLS; ++i) {
		start = get_current_time();
		syscall(CONTROL, 64);
		end = get_current_time();
		sum +=  (end - start);
	}
	printf("Average User->Kernel->User Context switch time = %lf ns\n",
		(double)(sum) / N_SYSCALLS);
}

int main(int argc, char **argv) {

	if (argc < 2 || (argc == 2 &&
	(strcmp(argv[1],"--help") == 0 || strcmp(argv[1], "help") == 0))) {
		printf("Usage: ./ep_control [--add | --list-apps | --clear app_name |"
		" --clear-all | --ctrl #number | --add-stats | --clear-stats |\n"
		" --clear-all-stats | --reset-stats | --reset-all-stats | --list-stats]\n");
		printf("--ctrl #number options:\n1) enabled stack dump (-1) for disabling\n");
		printf("2) enabled prints (-2) for disabling \n");
		printf("3) enabled stats (-3) disabling stats \n");
		printf("4) enabled alloc overhead stats (-4) disabling\n");
		printf("64) Empty Syscall for time calculation\n");
		return 0;
	}

	if (strcmp(argv[1], "--ctrl") == 0) {
		if (argc == 3) {
			int cmd = atoi(argv[2]);
			if (cmd == 64) {
				measure_syscall_overhead();
			} else {
				printf("Calling sys_oo_control_syscall syscall. Returned %x\n",
					(unsigned int) syscall(CONTROL, cmd));
			}
		} else {
			printf("Invalid number of arguments for %s\n", argv[1]);
		}
	} else if (strcmp(argv[1], "--add") == 0) {
	 	if (argc == 3) {
			syscall(ADD_APP, &argv[2]);
		} else {
			printf("Invalid number of arguments for %s\n", argv[1]);
		}
	} else if (strcmp(argv[1], "--list-stats") == 0) {
		printf("Calling sys_list_apps syscall. Returned %x\n",
			(unsigned int) syscall(LIST_APPS, 1));
	} else if (strcmp(argv[1], "--list-apps") == 0) {
		printf("Calling sys_list_apps syscall. Returned %x\n",
			(unsigned int) syscall(LIST_APPS, 0));
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
	} else if (strcmp(argv[1], "--reset-all-stats") == 0 ) {
			printf("Calling sys_clear_apps syscall. Returned %x\n",
				(unsigned int) syscall(CLEAR_APPS_LIST, NULL, 2));
	}

	else {
		printf("Invalid argument %s\n", argv[1]);
	}
	return 0;
}


