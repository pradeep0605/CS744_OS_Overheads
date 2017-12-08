#ifndef __OS_OVERHEADS_H__
#define __OS_OVERHEADS_H__

#include <linux/time.h>
#include <linux/mmzone.h>

#define MAX_TRACK_SIZE 100
#define MAX_PROC_NAME_LEN 50

// #define ENABLE_MSG 1

#define BILLION (1000000000)
#define MILLION (1000000)
#define THOUSANDS (1000)

#define CTXT_SWTCH_TIME (1500)
#define OO_MAX_ORDER 21

enum oo_register_type {EMPTY = 0,
	FOR_STATISTICS = 1};

typedef enum event_type {
	OO_MMAP_EVENT,
	OO_MREMAP_EVENT, 
	OO_PGFAULT_EVENT,
	OO_PGFAULT_MINOR_EVENT,
	OO_PGFAULT_MAJOR_EVENT,
	OO_ALLOC_ORDER_EVENT,
	OO_ALLOC_REQ_FROM_USR_SPACE,
	OO_MAX_EVENT
}  oo_event_t;


typedef struct os_overhead_statistics {
	char name[MAX_PROC_NAME_LEN]; /* Name of the process */
	int pid;
	unsigned long start_time;  /* Note start of an event */
	unsigned long end_time;  /* Note the end of an event*/
	unsigned long allocation_time; /* Amount of time spent for allocation */
	unsigned long kernel_time;  /* Total Amount of time spent in kernel
																 for allocation and handling page faults */
	unsigned long kernel_entry; /* Number of time we entered kernel for allocation 
																 kernel_entry = mmap_entry_count +
																	pagefault_entry_count */

	/* Coutners for all the events */
	unsigned long counters[OO_MAX_EVENT];
	unsigned long timers[OO_MAX_EVENT];
	unsigned long orders[OO_MAX_ORDER + 1];
	unsigned long user_mem_req;
} oo_stats_t;


extern char process_track_list[MAX_TRACK_SIZE][MAX_PROC_NAME_LEN];

extern unsigned char enable_dump_stack;
extern unsigned char enable_prints;
extern unsigned char enable_stats;
extern unsigned char enable_alloc_overhead_stats;
extern unsigned char disable_stats_on_child;

int is_process_being_tracked(const char* proc_name);
inline unsigned long get_current_time(void);
inline void record_start_event(oo_stats_t *application, oo_event_t event);
inline void record_end_event(oo_stats_t *application, oo_event_t event);
inline void incr_pgfault_count(oo_stats_t *application);
inline void incr_mmap_count(oo_stats_t *application);
int oo_register_process(const char *proc_name);
oo_stats_t* indexof_process_stats(const char* proc_name);
inline void record_alloc_event(oo_stats_t *application, oo_event_t event,
		unsigned long order);
inline void dec_event_counter(oo_stats_t *application, oo_event_t event);

#define oo_print(...) if (enable_prints) \
	pr_err(__VA_ARGS__)

#define oo_dump_stack() if (enable_dump_stack) \
	dump_stack()

/* All three macros assume that time is give in nano seconds */
#define IN_SECONDS(time)  ((double(time)) / (double(BILLION)))

#define IN_MICROSECS(time)  ((double(time)) / (double(MILLION)))

#define IN_MICROSECS(time)  ((double(time)) / (double(MILLION)))

#endif /* __OS_OVERHEADS_H__ */




