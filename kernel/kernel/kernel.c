#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/idt.h>
#include <kernel/gdt.h>
#include <kernel/pic.h>
#include <drivers/timer.h>
#include <drivers/pcspk.h>
#include <drivers/ata_pio.h>
#include <drivers/ff.h>

void kernel_main(void) {
	terminal_initialize();

	pic_remap(0x20, 0x28);
	pic_disable();
	irq_clear_mask(0); // Timer
	irq_clear_mask(1); // Keyboard
	timer_set(1);
	gdt_init();
	idt_init();

	/*play_sound(622);
	timer_wait(17);
	play_sound(311);
	timer_wait(8);
	play_sound(466);
	timer_wait(25);
	play_sound(414);
	timer_wait(38);
	play_sound(622);
	timer_wait(25);
	play_sound(466);
	timer_wait(50);
	nosound();*/
	printf("Hello, kernel World!\n");
	FATFS FatFs;
	FRESULT fres; // Result after operations
	FIL fp;
	// Mount the filesystem
	fres = f_mount(&FatFs, "", 1);
	if (fres != FR_OK) {
		printf("f_mount error (%i)\n", fres);
		while (1);
	}
	char buf[32];
	unsigned int bytes;
	fres = f_open(&fp, "test.txt", FA_READ | FA_OPEN_ALWAYS);
	if(fres == FR_OK) {
		fres = f_read(&fp, buf, sizeof(buf) - 1, &bytes);
		if(fres == FR_OK && bytes > 0){
			buf[bytes] = '\0';
			printf("Read %d bytes: %s\n", bytes, buf);
		} else {
			printf("f_read error (%i) or no bytes read\n", fres);
		}
	}

	// Unmount the filesystem
	f_mount(NULL, "", 1);


	while(1) asm ("hlt");
}
