#include <stdio.h>
int main() {
        int x;
        printf("Write number X:");
        scanf("%d",&x);
        printf("Binary number X:");

	int count = 0;
        for (int i=(sizeof(x)*8)-1; i>=0; i--){
                printf("%d",(x>>i)&1);
		if (((x>>i)&1)==1) {
			count++;
		}
        }

	printf("\n Count of 1:%d\n",count);
        return 0;
}
