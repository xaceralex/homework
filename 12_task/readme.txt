1) part1.c Программа, которая создает неименованный канал и
	порождает процесс. Родительский процесс закрывает дескриптор для
	чтения из канала и записывает в канал строку “Hi!”, затем ожидает
	завершения дочернего процесса. Дочерний процесс закрывает
	дескриптор для записи в канал, считывает строку из канала и выводит на
	экран.

2) part2_1.c, part2_2.c. Две программы, первая сервер, вторая клиент.
	Сервер создает именованный канал и открывает его только на запись,
	записывает строку “Hi!” и завершается. Клиент открывает созданный
	канал сервером только на чтение, считывает строку из канала и выводит
	на экран. Удаляет канал.