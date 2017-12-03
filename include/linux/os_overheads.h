#ifndef __OS_OVERHEADS_H__
#define __OS_OVERHEADS_H__

#define MAX_PROC_NAME_LEN 50

typedef struct process {
	char name[MAX_PROC_NAME_LEN];
	int pid;
} process_t;



#endif /* __OS_OVERHEADS_H__ */
