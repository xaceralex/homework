1) Установка кросс-компилятора:
	sudo apt-get install gcc-arm-linux-gnueabihf

2) Проверили, что компилятор доступен:
	arm-linux-gnueabihf-gcc --version

3) Создали дофолтный конфиг для arm-архитектуры
	ARCH=arm make defconfig

4) Собрали ядро:
	ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make -j8 zImage

5) Собрали все dtb-файлы для архитектуры arm:
	ARCH=arm make dtbs

6) Скопировали файл «vexpress-v2p-ca9.dtb» из папки «arch/arm/boot/dts» в папку с собранным ядром.

7) Установили эмулятор qemu:
	sudo apt install qemu-system-arm

8) Запустили ядро:
	QEMU_AUDIO_DRV=none qemu-system-arm -M vexpress-a9 -kernel zImage -dtb vexpress-v2p-ca9.dtb -append "console=ttyAMA0" -nographic

	img1.png - Скриншот с ошибкой "Kernel panic"