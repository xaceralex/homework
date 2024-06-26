#include <stdio.h>

int main() {
        int x,y;
	printf("Write number X:");
        scanf("%d",&x);
        printf("Binary number X:");

        for (int i=((sizeof(x))*8)-1; i>=0; i--){
		printf("%d",(x>>i)&1);
		 if (i%8==0) {
                        printf(" ");
                }
        }
        printf("\n");

	printf("Write number Y:");
        scanf("%d",&y);
	x = (y<<16) + (x%65536) + ((x/16777216)<<24); 	
	printf("Bynary number Y:");

	 for (int i=((sizeof(y))*8)-1; i>=0; i--){
                printf("%d",(y>>i)&1);
                 if (i%8==0) {
                        printf(" ");
                }

        }
        printf("\n");

	printf("Final result:");
        for (int i=((sizeof(x))*8)-1; i>=0; i--){
		printf("%d",(x>>i)&1);
                 if (i%8==0) {
                        printf(" ");
                }
        }
        printf("\n");
	return 0;
}
