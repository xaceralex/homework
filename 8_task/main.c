#include <stdio.h>
#include <stdlib.h>
#include "func.h"

int main(void) {
	int chose;
	int result;
	while (chose != 5) {
		printf("Выберите пункт\n");
		printf("1 - Сложение\n2 - Вычитание\n");
		printf("3 - Умножение\n4 - Деление\n");
		printf("5 - Выход\n");
		scanf("%d",&chose);
		switch (chose) {
			case 1:
				result = add();
                                printf("%d\n",result);
			break;
                	case 2:
                        	result = sub();
                                printf("%d\n",result);
	                break;
			case 3:
                        	result = mul();
                                printf("%d\n",result);
	                break;
        	        case 4:
                        	result = div1();
                                printf("%d\n",result);
                	break;
	                case 5:
        	                //exit
                	break;
			default:
			        printf("Неправильный ввод\n");
		}
	}
	exit(EXIT_SUCCESS);
}
