#include<stdio.h>
#define N 4

int main() {
	int a[N][N]={   {1 ,2 ,3 ,4},
			{12,13,14,5},
			{11,16,15,6},
			{10,9 ,8 ,7}
		    };
	for (int i=0;i<N;i++) {
		for (int j=0;j<N;j++) {
			printf("%d ",a[i][j]);
		}
	printf("\n");
	}
return 0;
}
