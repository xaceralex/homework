
#include <stdio.h>
int main () {
	int x = 0;
	printf("Wrinte number X:");
	scanf("%d",&x);

	printf("Binary number X:");
        for (int i=(sizeof(x)*8)-1; i>=0; i--) { 
		printf("%d",(x>>i)&1);  
        }
	printf("\n");

	x = ~x;
        printf("Inver. number X:");
	for (int i=(sizeof(x)*8)-1; i>=0; i--) {
		printf("%d",(x>>i)&1);	
	}
	printf("\n");

	x=x+1;
	printf("Number plus 1 X:");
        for (int i=(sizeof(x)*8)-1; i>=0; i--) {
                printf("%d",(x>>i)&1);  
        }
	printf("\n");

	x=x|2147483648;
	printf("Num. check 1_ X:");
        for (int i=(sizeof(x)*8)-1; i>=0; i--) {
                printf("%d",(x>>i)&1);  
        }
        printf("\n");


	return 0;
}
