[part 1]

	1) Напишем простую программу «init.c», которая будет работать 15 секунд.
		#include <stdio.h>
		#include <unistd.h>

		int main(void)
		{
			printf("Hello!\n");
			sleep(15);
			return 0;
		}

	2) Скомпилируем её с использованием статических библиотек:
		arm-linux-gnueabihf-gcc -static init.c -o init

	3) Проверим, что готовый файл скомпилирован для ARM-архитектуры и имеет статическую сборку:
		file ./init

	4) Упакуем «init»:
		echo init | cpio -o -H newc | gzip >initramfs.cpio.gz

	5) Проверим, что в архиве нет ничего лишнего:
		zcat initramfs.cpio.gz | cpio -t

	6) Запуск ядра через qemu с отключением графики:
		QEMU_AUDIO_DRV=none qemu-system-arm -M vexpress-a9 -kernel zImage -initrd 	initramfs.cpio.gz -dtb vexpress-v2p-ca9.dtb -append "console=ttyAMA0" -nographic

	Результат запуска ядра приведён на скриншоте «img1.png».


[part 2]

	1) Скачиваем busybox с сайта (https://busybox.net/downloads/) для создания файловой системы.

	2) Создаём дефолтный конфиг для arm-архитектуры:
		ARCH=arm make defconfig

	3) Открываем меню конфигурации:
		ARCH=arm make menuconfig

	4) Переходим в раздел «Settings».

	5) Указываем использование статических библиотек:
		(Build static binary (no shared libs))

	6) Указываем префикс кросс-компилятора ( Cross compiler prefix):
		arm-linux-gnueabihf

	7) Возвращаемся в основное меню и переходим в раздел «Networking Utilities».

	8) Отключаем опцию «tc».

	9) Сохраняем изменения и выходим из меню конфигурации.

	10) Проверяем наличие файла «busybox» и проверяем, что он скомпилирован для ARM-архитектуры и имеет статическую сборку:
		file ./busybox

	11) Вызываем команду:
		make install

	12) Проверяем наличия созданных папок в «_install»

	13) Упаковываем файловую систему:
		find . | cpio -o -H newc | gzip > initramfs.cpio.gz

	14) Проверяем, что в архиве нет ничего лишнего:
		zcat initramfs.cpio.gz | cpio -t

	15) Запустим qemu:
		QEMU_AUDIO_DRV=none qemu-system-arm -M vexpress-a9 -kernel zImage -initrd 	initramfs.cpio.gz -dtb vexpress-v2p-ca9.dtb -append "console=ttyAMA0 rdinit=/bin/ash" -nographic

	16) После запуска qemu попробуем выполнить несколько команд в «ash»:
		ls
		mkdir proc
		ls
		mount -t proc proc proc
		ps 

	Результат работы команд приведён на скриншоте «img2.png».