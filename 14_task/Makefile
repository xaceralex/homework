# Определение компилятора и необходимых флагов
CC = gcc
CFLAGS = -Wall -Werror

# Флаги линковки
LDFLAGS_C = -lncursesw -lpthread
LDFLAGS_S = -lpthread

# Исходные файлы
SOURCES = part1_sv_s.c part1_sv_c.c part1_px_s.c part1_px_c.c part2_s.c part2_c.c

# Исполняемые файлы
EXECUTABLES = part1_sv_s part1_sv_c part1_px_s part1_px_c part2_s part2_c

# Правила сборки исполняемых файлов
all: $(EXECUTABLES)

# Правила для исполняемых файлов, используя шаблоны
%: %.c
		$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS_S)

part2_c: part2_c.c
		$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS_C)

part2_s: part2_s.c
		$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS_S)

# Очистка
clean:
		rm -f $(EXECUTABLES)