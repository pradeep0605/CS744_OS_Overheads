#include <linux/os_overheads.h>
#include <linux/syscalls.h>

asmlinkage long sys_os_overheads_control(int val) {
	pr_err("\n\nValue passed from userspace = %d\n\n", val);
	return 7891011;
}


asmlinkage long sys_list_apps(int is_stat) {
	return 12131415;
}

asmlinkage long sys_clear_apps(char *name, int is_stat) {

	return 16171819;
}


