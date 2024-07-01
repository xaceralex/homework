#include<stdio.h>
#define N 10

int main() {
        int a[N];

	for (int i=0;i<N;i++) {
		scanf("%d",&a[i]);
 	}

	for (int i=0;i<N/2;i++) {
                a[i]=a[i]*a[N-i-1];
		a[N-i-1]=a[i]/a[N-i-1];
		a[i]=a[i]/a[N-i-1];
        }

	printf("\n");

	for (int i=0;i<N;i++) {
        	printf("%d ",a[i]);
        }
return 0;
}
