#include<stdio.h>
#include<string.h>
#include<stdbool.h>

#define N1 41
#define N2 5

int main () {
	char str1[N1] = "";
	char str2[N2] = "";
        bool flag = false;
        int i = 0;
        int j = 0;

	printf("Введите начальную строку:");
	scanf("%s",str1);
	printf("Введите искомую строку:");
	scanf("%s",str2);

	printf("Начальная строка:%s\n",str1);
	printf("Искомоя строка  :%s\n",str2);

	int len1 = strlen(str1);
	int len2 = strlen(str2);

	for (;i<len1; i++) {
		j=0;
		for (;j<len2; j++) {
			if (str1[i+j] != str2[j]) {
				break;
			}
		}
		if (j==len2) {
			flag = true;
			break;
		}
	}

	if (flag == true) {
		char* ptr1 = str1;
		ptr1 += i;
		printf("ptr1= %p \n",ptr1);
	} else
		printf("NULL");

	return 0;
}
