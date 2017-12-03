#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

int main()
{
	int n = 100;
	int *ptr = mmap(NULL, n, PROT_READ | PROT_WRITE, MAP_PRIVATE |
		MAP_ANONYMOUS, -1, 0);

	printf("Returned address %p\n", ptr);
	return 0;
}
