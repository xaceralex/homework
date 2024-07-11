#include <stdio.h>
#include <string.h>

struct abonent {
	char name[10];
	char second_name[10];
	char tel[10];
};

void main () {
	struct abonent phonebook[100];
	int current = 0;
	int  chose = -1;

	for (int i=0; i<99; i++) {
		strcpy(phonebook[i].name, "0");
		strcpy(phonebook[i].second_name, "0");
		strcpy(phonebook[i].tel, "0");
	}

	while (chose != 5){
		printf("Выберете пункт\n1) Добавить абонента\n");
		printf("2) Удалить абонента\n3) Поиск абонентов по имени\n");
		printf("4) Вывод всех записей\n5) Выход\n");

		scanf("%d",&chose);

		switch (chose) {
			case 1:
				printf("Введите имя абонента \n");
				scanf("%s", phonebook[current].name);
				printf("Введите фамилию абонента \n");
                                scanf("%s", phonebook[current].second_name);
				printf("Введите телефон абонента \n");
                                scanf("%s", phonebook[current].tel);
				current++;
				break;
			case 2:
				if (current>0) {
					printf("Введите номер записи для удаления\n");
					for(int i=0; i<current; i++){
						printf("Абонент номер %d\n",i);
			        	        printf("%s\n",phonebook[i].name);
						printf("%s\n",phonebook[i].second_name);
						printf("%s\n\n",phonebook[i].tel);
					}
					int num;
					scanf("%d",&num);
					for (int i = num; i<current-1; i++) {
	                                        strcpy(phonebook[i].name, phonebook[i+1].name);
	                                        strcpy(phonebook[i].second_name, phonebook[i+1].second_name);
        	                                strcpy(phonebook[i].tel, phonebook[i+1].tel);

					}
					strcpy(phonebook[current].name, "0");
			                strcpy(phonebook[current].second_name, "0");
                			strcpy(phonebook[current].tel, "0");
					current--;
				}
                                break;
			case 3:
                                printf("Введите имя абонента\n");
				char name1[10] = "0";
				scanf("%s", name1);
				for (int i=0; i<current; i++) {
					if (strcmp(phonebook[i].name,name1)==0) {
			                	printf("Абонент номер %d\n",i);
	                                        printf("%s\n",phonebook[i].name);
        	                                printf("%s\n",phonebook[i].second_name);
                	                        printf("%s\n\n",phonebook[i].tel);

					}
				}
				break;
			case 4:
                                for(int i=0; i<current; i++){
	               			printf("Абонент номер %d\n",i);
					printf("%s\n",phonebook[i].name);
                                        printf("%s\n",phonebook[i].second_name);
					printf("%s\n\n",phonebook[i].tel);
			        }
				break;
		}
	}
}
