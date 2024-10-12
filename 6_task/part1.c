#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

struct abonent {
        char name[10];
        char second_name[10];
        char tel[10];
};

void abon_add(struct abonent **ptr, int count) {
	printf("Введите имя абонента:\n");
	scanf("%s", (*ptr+count)->name);
        printf("Введите фамилию абонента:\n");
        scanf("%s", (*ptr+count)->second_name);
        printf("Введите номер абонента:\n");
        scanf("%s", (*ptr+count)->tel);
}

void abon_print(struct abonent **ptr, int count) {
	for(int i= 0;i<count; i++) {
                printf("Запись №%d\n",i);
		printf("Имя:%s\n",(*ptr+i)->name);
		printf("Фам:%s\n",(*ptr+i)->second_name);
		printf("Тел:%s\n",(*ptr+i)->tel);
	}
}

void abon_delete(struct abonent **ptr, int count) {
        int select_abon = -1;
        printf("Выберете номер записи для удаления\n");
        abon_print(ptr, count);
        scanf("%d",&select_abon);
        for (int i=select_abon; i<=count-2; i++) {
                strcpy((*ptr+i)->name, (*ptr+i+1)->name);
                strcpy((*ptr+i)->second_name, (*ptr+i+1)->second_name);
                strcpy((*ptr+i)->tel, (*ptr+i+1)->tel);
        }
        strcpy((*ptr+count-1)->name, "0");
        strcpy((*ptr+count-1)->second_name, "0");
        strcpy((*ptr+count-1)->tel, "0");
}

void abon_search (struct abonent **ptr, int count) {
	char name[10] = "0";
	printf("Введите имя абонента\n");
	scanf("%s", name);
	for (int i=0; i<count; i++) {
		if (strcmp((*ptr+i)->name,name)==0) {
			printf("Запись №%d\n",i);
			printf("%s\n",(*ptr+i)->name);
			printf("%s\n",(*ptr+i)->second_name);
			printf("%s\n\n",(*ptr+i)->tel);
		}
	}
}

void main () {
	struct abonent *phonebook;
	struct abonent **ptr = &phonebook;

	int count = 0;
	int chose = -1;
	int select = -1;

	while (chose != 5){
                printf("Выберете пункт\n1) Добавить абонента\n");
                printf("2) Удалить абонента\n3) Поиск абонентов по имени\n");
                printf("4) Вывод всех записей\n5) Выход\n");
                scanf("%d",&chose);
                switch (chose) {
			case 1:
                                if (count == 0) {
                                        phonebook = malloc(sizeof(struct abonent));
                                        abon_add(ptr, count);
                                        count++;
                                } else {
                                        if ((count < 100) && (count > 0)) {
                                                phonebook = realloc(phonebook,(sizeof(struct abonent))*count);
                                                abon_add(ptr, count);
                                                count++;
                                        } else
                                                printf("Достигнут лимит в 100 записей\n");
                                }
                                break;
                        case 2:
	        		abon_delete(ptr, count);
                                count--;
		                phonebook = realloc(phonebook,(sizeof(struct abonent))*count);
				break;
                        case 3:
				abon_search(ptr, count);
        	                break;
                        case 4:
				abon_print(ptr, count);
				break;
		}
	}
	free(phonebook);
}
