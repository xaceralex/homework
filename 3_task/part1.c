#include<stdio.h>

int main () {
	int x;
	int y;
	char *ptr_char;

	printf("Введите число:");
	scanf("%d",&x);
	printf("Введите 3-й байт:");
	scanf("%d",&y);
	ptr_char = (char*)&x;
	ptr_char += 2;
	*ptr_char = (char) y;

	printf("Y=%d\n",y);
	printf("X=%d\n",x);

return 0;
}
