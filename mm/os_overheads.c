#include <linux/os_overheads.h>
#include <linux/syscalls.h>
#include <linux/semaphore.h>
#include <linux/mutex.h>


/* ALL golbals here */
// char process_track_list[MAX_TRACK_SIZE][MAX_PROC_NAME_LEN];
oo_stats_t oo_statistics[MAX_TRACK_SIZE];
oo_stats_t zeroed_stats;

unsigned char enable_dump_stack;
unsigned char enable_prints;
unsigned char enable_stats;
unsigned char enable_alloc_overhead_stats;
unsigned char disable_stats_on_child;

DEFINE_MUTEX(oo_mutex);

oo_stats_t* indexof_process_stats(const char* proc_name);
char sem_not_initialized;

inline unsigned long get_current_time(void) {
	unsigned long int time = 0;
	struct timespec tv;

	getnstimeofday(&tv);

	time = ((tv.tv_sec * BILLION) + tv.tv_nsec);
	return time;
}

inline void record_alloc_event(oo_stats_t *application, oo_event_t event,
	unsigned long order) {
	mutex_lock(&oo_mutex);
	switch(event) {
		case OO_ALLOC_ORDER_EVENT:
			if (application) {
				application->orders[order]++;
			}
			break;
		case OO_ALLOC_REQ_FROM_USR_SPACE:
			if (application) {
				application->user_mem_req += order;
			}
			break;
		default:
			break;
	}
	mutex_unlock(&oo_mutex);
}

inline void record_start_event(oo_stats_t *application, oo_event_t event) {
	if (application != NULL) {
		mutex_lock(&oo_mutex);
		application->start_time = get_current_time();
		application->kernel_entry++;

		if (event < OO_MAX_EVENT) {
			application->counters[event]++;
		}
		mutex_unlock(&oo_mutex);
	}
}

inline void record_end_event(oo_stats_t *application, oo_event_t event) {
	mutex_lock(&oo_mutex);
	if (application != NULL) {
		application->end_time = get_current_time();
		application->kernel_time += (application->end_time - 
				application->start_time);

		if (event < OO_MAX_EVENT) {
			application->timers[event] +=
				(application->end_time - application->start_time);
		}
	}
	mutex_unlock(&oo_mutex);
}

inline void incr_pgfault_count(oo_stats_t *application) {
	mutex_lock(&oo_mutex);
	if (application != NULL) {
		application->counters[OO_PGFAULT_EVENT]++;
	}
	mutex_unlock(&oo_mutex);
}

inline void incr_mmap_count(oo_stats_t *application) {
	mutex_lock(&oo_mutex);
	if (application != NULL) {
		application->counters[OO_MMAP_EVENT]++;
	}
	mutex_unlock(&oo_mutex);
}

inline void incr_mremap_count(oo_stats_t *application) {
	mutex_lock(&oo_mutex);
	if (application != NULL) {
		application->counters[OO_MREMAP_EVENT]++;
	}
	mutex_unlock(&oo_mutex);
}

inline void dec_event_counter(oo_stats_t *application, oo_event_t event) {
	mutex_lock(&oo_mutex);
	if (application != NULL && event < OO_MAX_EVENT) {
		if (application->counters[event] > 0) {
			application->counters[event]--;
		}
	}
	mutex_unlock(&oo_mutex);
}

// int indexof_tracked_process(const char* proc_name);

asmlinkage long sys_os_overheads_control(int val) {
	/* For testing User->Kernel->User space context switch times */
	if (val == 64) {
		return 0xceed;
	}

	/*
	pr_err("\n======================Inside system calls (%s)"
		"============================", __func__);
	*/

	switch (val) {
		case 1:
			enable_dump_stack = 1;
			pr_err("OO: Enabled stack dump");
			break;
		case -1:
			enable_dump_stack = 0;
			pr_err("OO: Disabled stack dump");
			break;

		case 2:
			enable_prints = 1;
			pr_err("OO: Enabled mmap prints");
			break;
		case -2:
			enable_prints = 0;
			pr_err("OO: disabled mmap prints");
			break;
		case 3:
			enable_stats = 1;
			pr_err("OO: Enabled stats");
			break;
		case -3:
			enable_stats = 0;
			pr_err("OO: Disabled stats");
			break;
		case 4:
			pr_err("OO: Enabled alloc overheads stats");
			enable_alloc_overhead_stats = 1;
			break;
		case -4:
			pr_err("OO: Disabled alloc overheads stats");
			enable_alloc_overhead_stats = 0;
			break;
	}
	return 0;
}

// asmlinkage long sys_list_oo_apps(void) {
asmlinkage long sys_list_apps(int is_stat) {
	int i = 0, j = 0, k = 0;
	char str[900] = {0};
	int len = 0;
	unsigned long total_allocated = 0;

	// mutex_lock(&oo_mutex);
	if (is_stat == 0) {
		for (i = 0; i < MAX_TRACK_SIZE; ++i) {
			if (oo_statistics[i].name[0] != '\0') {
				pr_err("%s\n", oo_statistics[i].name);
			}
		}
		return 0;
	}

	pr_err(" =================== Listing all Statistics Enabled"
		" Applications =========================\n");
	for (i = 0; i < MAX_TRACK_SIZE; ++i) {
		total_allocated = 0;
		str[0]= '\0';
		if (oo_statistics[i].name[0] != '\0') {
			pr_err("%s----------------------------#\n", oo_statistics[i].name);
			pr_err(
			"\n\t\tKernel Entry    : %-11lu"
			"\n\t\tKernel Time     : %-11lu"
			"\n\t\tPGFLT Count     : %-11lu"
			"\n\t\tPGFLT Minor     : %-11lu"
			"\n\t\tPGFLT Major     : %-11lu"
			"\n\t\tPGFLT Time      : %-11lu"
			"\n\t\tPGFLT Minor Time: %-11lu"
			"\n\t\tPGFLT Major Time: %-11lu"
			"\n\t\tBRK Count			 : %-11lu"
			"\n\t\tBRK Time        : %-11lu"
			"\n\t\tMmap Count      : %-11lu"
			"\n\t\tMmap Time       : %-11lu"
			"\n\t\tMremap Count    : %-11lu"
			"\n\t\tMemap Time      : %-11lu"
			"\n\t\tTotal Time      : %-11lu\n",
			oo_statistics[i].kernel_entry,
			oo_statistics[i].kernel_time,
			oo_statistics[i].counters[OO_PGFAULT_EVENT],
			oo_statistics[i].counters[OO_PGFAULT_MINOR_EVENT],
			oo_statistics[i].counters[OO_PGFAULT_MAJOR_EVENT],
			oo_statistics[i].timers[OO_PGFAULT_EVENT],
			oo_statistics[i].timers[OO_PGFAULT_MINOR_EVENT],
			oo_statistics[i].timers[OO_PGFAULT_MAJOR_EVENT],
			oo_statistics[i].counters[OO_BRK_EVENT],
			oo_statistics[i].timers[OO_BRK_EVENT],
			oo_statistics[i].counters[OO_MMAP_EVENT],
			oo_statistics[i].timers[OO_MMAP_EVENT],
			oo_statistics[i].counters[OO_MREMAP_EVENT],
			oo_statistics[i].timers[OO_MREMAP_EVENT],
			(oo_statistics[i].kernel_time +
			(oo_statistics[i].kernel_entry * CTXT_SWTCH_TIME)));

			k = OO_MAX_ORDER;
			while (k >= 0 && oo_statistics[i].orders[k] == 0) { k--;}

			for (j = 0; j <= k; ++j) {
				len = strlen(str);
				total_allocated +=
				(oo_statistics[i].orders[j] * (PAGE_SIZE << j));
				if (j == 9) {	/* Print things in the next line */
					sprintf(&str[len], "\n\t\t\t");
					len = strlen(str);
				}
				sprintf(&str[len],
					"O%-2d:%lu, ", j, oo_statistics[i].orders[j]);
			}

			pr_err("\t\tOrder: %s\n", str);
			pr_err("\t\tTotal Kernel Allocation:"
				" %lu (B), %lu (KB), %lu (MB) with PAGE_SIZE = %lu",
				(total_allocated), (total_allocated / 1024),
				(total_allocated / (1024 * 1024)), PAGE_SIZE);
			pr_err("\t\tUser Space Requested: %lu (B), %lu (KB), %lu (MB)\n",
				oo_statistics[i].user_mem_req, oo_statistics[i].user_mem_req / 1024,
				oo_statistics[i].user_mem_req / (1024 * 1024));
		}
	}
	// mutex_unlock(&oo_mutex);
	return 0xdeadbeef;
}

asmlinkage long sys_clear_apps(const char __user** process_name, int
	is_stats) {
	int i = 0;
	pr_err("\n======================Inside system calls (%s)"
		"============================", __func__);
	
	// mutex_lock(&oo_mutex);
	if (is_stats == FOR_STATISTICS) {
		/* syscall is to remove all application stats information */
		if (process_name == NULL) {
			for ( i = 0 ; i < MAX_TRACK_SIZE ; i++ ) {
				if (oo_statistics[i].name[0] != '\0') {
					oo_statistics[i] = zeroed_stats;
				}
			}
		} else {
			/* remove process_name[0]'s stats */
			oo_stats_t *ret = indexof_process_stats(process_name[0]);
			if (ret != NULL) {
				*ret = zeroed_stats;
			} else {
				pr_err("Application name %s not found in the list\n", process_name[0]);
			}
		}
	} else if (is_stats == 2) { 
		/* reset the counters of all stat applications (not remove) */
		if (process_name == NULL) {
			for ( i = 0 ; i < MAX_TRACK_SIZE ; i++ ) {
				if (oo_statistics[i].name[0] != '\0') {
					char temp[MAX_PROC_NAME_LEN];

					strncpy(temp, oo_statistics[i].name, MAX_PROC_NAME_LEN);

					oo_statistics[i] = zeroed_stats;

					strncpy(oo_statistics[i].name, temp, MAX_PROC_NAME_LEN);
				}
			}
		} else {
			oo_stats_t *ret = indexof_process_stats(process_name[0]);
			if (ret != NULL) {
				char temp[MAX_PROC_NAME_LEN];
				strncpy(temp, ret->name, MAX_PROC_NAME_LEN);

				*ret = zeroed_stats;

				strncpy(ret->name, temp, MAX_PROC_NAME_LEN);

			} else {
				pr_err("Application name %s not found in the list\n", process_name[0]);
			}
		}
	}

	// mutex_unlock(&oo_mutex);

	if (is_stats > 1) {
		sys_list_apps(1);
	} else {
		sys_list_apps(0);
	}
	return 0xdeadbeef;
}


/* Register process for collecting statistics */
int oo_register_process(const char *proc_name) {
  unsigned int i = 0;
	char* ret;
	oo_stats_t *stats = NULL;
	printk(KERN_ALERT "In function %s with proc_name = %s\n", __func__,
		 proc_name);

	// mutex_lock(&oo_mutex);
	/* Enable collection of statistics for the process `proc_name` */
	i = 0;
	stats = indexof_process_stats(proc_name);
	if (stats == NULL) {
		while(i < MAX_TRACK_SIZE && oo_statistics[i].name[0] != '\0') {
			i++;
		}
		if (i < MAX_TRACK_SIZE) {
			ret = strncpy(oo_statistics[i].name, proc_name,	MAX_PROC_NAME_LEN);
		} else {
			return -1;
		}
	}
	// mutex_unlock(&oo_mutex);
	return 0;
}

asmlinkage long sys_add_app(char **proc_name)
{
  char proc[MAX_PROC_NAME_LEN];
	long ret, res;	
	pr_err(KERN_ALERT "\n\n===========================\n"
										"Inside system calls (%s)"
										"\n============================\n\n", __func__);

	if (proc_name != NULL) {
		ret = strncpy_from_user(proc, proc_name[0], MAX_PROC_NAME_LEN);
		res = oo_register_process(proc);
		if (res < 0) {
			pr_err("%s: %d: Error registering the process %s\n", __func__,
				__LINE__, proc_name[0]);
		}
		return res;
	} else {
		pr_err("\n\nEmpty string passed from user space to system call (%s)\n\n",
		__func__);
		return -1;
	}
  return 0;
}

/*
 * This function checks whether a process name provided matches from the list
 *
 */

/*
inline int indexof_tracked_process(const char* proc_name)
{
    unsigned int i;
    for ( i = 0 ; i < MAX_TRACK_SIZE ; i++ ) {
        if (!strncmp(proc_name, process_track_list[i], MAX_PROC_NAME_LEN))
            return i;
    }
    return -1;
}
*/

oo_stats_t* indexof_process_stats(const char* proc_name)
{
    unsigned int i;
    for ( i = 0 ; i < MAX_TRACK_SIZE ; i++ ) {
        if (!strncmp(proc_name, oo_statistics[i].name, MAX_PROC_NAME_LEN))
            return &oo_statistics[i];
    }
		// oo_print("Returning null from %s for process %s\n", __func__, proc_name);
    return NULL;
}

/*
int is_process_being_tracked(const char* proc_name)
{
    unsigned int i;
    for ( i = 0 ; i < MAX_TRACK_SIZE ; i++ ) {
        if (!strncmp(proc_name, process_track_list[i], MAX_PROC_NAME_LEN))
            return 1;
    }

    return 0;
}
*/
