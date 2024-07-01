#include<stdio.h>
#define N 5

int main() {
        int a[N][N];
	for (int i=0;i<N;i++) {
        	for (int j=0;j<N;j++) {
        		a[i][j] = 0;
        	}
	}
	int x1=0;
	int y1=0;
	int x2=N-1;
	int y2=N-1;

	for (int i=1; i<=N*N;) {
		for (int i1=x1; i1<=x2; i1++) {
			a[y1][i1] = i;
			i++;
		}
		y1++;
		for (int i1=y1; i1<=y2; i1++) {
                	a[i1][x2] = i;
                	i++;
        	}
		x2--;
		for (int i1=x2; i1>=x1; i1--) {
                	a[y2][i1] = i;
                	i++;
        	}
		y2--;
        	for (int i1=y2; i1>=y1; i1--) {
                	a[i1][x1] = i;
                	i++;
        	}
		x1++;
	}

        for (int i=0;i<N;i++) {
                for (int j=0;j<N;j++) {
                        printf("%d ",a[i][j]);
                }
        printf("\n");
        }
	return 0;
}
