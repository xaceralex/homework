#include <stdio.h>
int main() {
	int x = 0;
	printf("Write number X:");
	scanf("%d",&x);
        printf("Binary number X:");

	for (int i=(sizeof(x)*8)-1; i>=0; i--){
		printf("%d",(x>>i)&1);
	}
        printf("\n");
	return 0;
}
