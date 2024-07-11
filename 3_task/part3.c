#include<stdio.h>
#define N 10

int main () {
	int a[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int *ptr = a;
	for (int i=0; i<10; i++) {
		printf("a[%d] = %d\n", i, *ptr);
		ptr+=1;
	}
	return 0;
}
