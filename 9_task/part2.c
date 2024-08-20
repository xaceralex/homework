#include <stdlib.h> 
#include <dirent.h> //для работы с директориями
#include <panel.h>
#include <ncurses.h> //консольная графика	
#include <string.h>
#include <malloc.h> 

#include <stdio.h>

#include <inttypes.h>

struct dirent_short {
	unsigned char d_type;
	char d_name[256];
};

int write_arr(struct dirent_short **dir_data, DIR *dir_fl)
{
	int count = 1;
	struct dirent *dir_data0 = NULL;
	struct dirent_short *dir_data_tmp = NULL;

	while (dir_data0 = readdir(dir_fl)) 
	{
		if (count == 1)
			dir_data_tmp = malloc(sizeof(struct dirent_short));
		else
			dir_data_tmp = realloc(*dir_data, count * sizeof(struct dirent_short));

		if (dir_data_tmp == NULL)
		{
			return -1;
		}
		*dir_data = dir_data_tmp;

		strcpy((*dir_data)[count-1].d_name, dir_data0->d_name);
		(*dir_data)[count-1].d_type = dir_data0->d_type;
		count++;
	}
	return count;
}

void print_arr(struct dirent_short **dir_data, int count, bool left)
{
	int xt, yt;
	if (left == TRUE)
	{
		xt = 3;
		yt = 2;
	}
	else
	{
		xt = 31;
		yt = 2;
	}
	if (count > 0) 
	{
		for (int i = 0; i < count; i++, yt++)
		{
			mvprintw(yt, xt, "%s", (*dir_data)[i].d_name);
		}
	}
}

int main(void)
{
	initscr();//инициализация окна

	DIR *dir_fl_left, *dir_fl_right;
	struct dirent_short *dir_data_left = NULL, *dir_data_right = NULL;
	char directory_left[255] = "/home/", directory_right[255] = "/home/";
	int dir_level_left = 0, dir_level_right = 0;
	dir_fl_left = opendir(directory_left);
	dir_fl_right = opendir(directory_right);

	int count_left = 0, count_right = 0;

	count_left = write_arr(&dir_data_left, dir_fl_left);
	count_right = write_arr(&dir_data_right, dir_fl_right);

	print_arr(&dir_data_left, count_left, TRUE);
	print_arr(&dir_data_right, count_right, FALSE);

	keypad(stdscr, 1); // подключение стрелок
	int input_key; // клавиша которую нажал пользователь
	int x = 2,y = 2;

	bool left_win = 1; // текущее окно 1 - левое, 0 - правое
	int current_item = 0;
	do
	{
		switch (input_key)
		{
			case KEY_UP:
				if (y != 2)
				{
					y--;
					current_item--;
					clear();
					mvprintw(0, 0, "d_name = %s", (dir_data_left)[current_item].d_name);
					print_arr(&dir_data_left, count_left, TRUE);
					print_arr(&dir_data_right, count_right, FALSE);
				}
				break;
			case KEY_DOWN:
				y++;
				current_item++;
				clear();
				mvprintw(0, 0, "d_name = %s", (dir_data_left)[current_item].d_name);
				print_arr(&dir_data_left, count_left, TRUE);
				print_arr(&dir_data_right, count_right, FALSE);
				break;
			case 9:
				if (left_win == 1)
				{
					x=x+28;
					left_win = 0;
				}
				else 
				{
					x=x-28;
					left_win = 1;
				}
				break;
			case '\n': //нажатие ENTER
				if ((left_win == 1) && ((dir_data_left)[current_item].d_type == 4)&&(strcmp ((dir_data_left)[current_item].d_name, ".") != 0))
				{
					if (strcmp ((dir_data_left)[current_item].d_name, "..") == 0)
					{
						if (dir_level_left > 0)
						{
							for (int i = 255, count_slash = 0, j = 1; i > 0 && count_slash < 1; i--, j++)
							{
								if (directory_left[i] == '/')
								{
									count_slash++;
								}
								if (count_slash != 2)
									directory_left[i] = '\0';
							}
							dir_level_left--;
						}
					}
					else
					{
						char* buff = calloc(strlen(directory_left) + strlen((dir_data_left)[current_item].d_name) + 2, 1);
						strcat(buff, directory_left);
						strcat(buff, (dir_data_left)[current_item].d_name);
						strcat(buff, "/");
						strcpy(directory_left, buff);
						free(buff);
						dir_level_left++;
					}
					dir_fl_left = opendir(directory_left);
					count_left = write_arr(&dir_data_left, dir_fl_left);
					clear();
					print_arr(&dir_data_left, count_left, TRUE);
				}
				else //правое окно
				if ((left_win == 0) && ((dir_data_right)[current_item].d_type == 4)&&(strcmp ((dir_data_right)[current_item].d_name, ".") != 0))
				{
					if (strcmp ((dir_data_right)[current_item].d_name, "..") == 0)
					{
						if (dir_level_right > 0)
						{
							for (int i = 255, count_slash = 0, j = 1; i > 0 && count_slash < 1; i--, j++)
							{
								if (directory_right[i] == '/')
								{
									count_slash++;
								}
								if (count_slash != 2)
									directory_right[i] = '\0';
							}
							dir_level_right--;
						}
					}
					else
					{
						char* buff = calloc(strlen(directory_right) + strlen((dir_data_right)[current_item].d_name) + 2, 1);
						strcat(buff, directory_right);
						strcat(buff, (dir_data_right)[current_item].d_name);
						strcat(buff, "/");
						strcpy(directory_right, buff);
						free(buff);
						dir_level_right++;
					}
					dir_fl_right = opendir(directory_right);
					count_right = write_arr(&dir_data_right, dir_fl_right);
					clear();
					print_arr(&dir_data_right, count_right, TRUE);
				}
				clear();
				print_arr(&dir_data_left, count_left, TRUE);
				print_arr(&dir_data_right, count_right, FALSE);
				break;
		}
		mvaddch(y, x, '_');
	}
	while((input_key = getch()) != 27);

	free(dir_data_left);//высвобождение памяти
	free(dir_data_right);//высвобождение памяти
	closedir(dir_fl_left);//закрытие потока
	closedir(dir_fl_right);//закрытие потока
	endwin();//закрытие окна
	exit(EXIT_SUCCESS);
}