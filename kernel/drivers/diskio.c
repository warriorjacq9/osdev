/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include <drivers/ff.h>			/* Obtains integer types */
#include <drivers/diskio.h>		/* Declarations of disk functions */
#include <stdio.h>

/* Definitions of physical drive number for each drive */
#define DEV_HDD		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

#define ATA_PRIMARY_DATA         0x1F0
#define ATA_PRIMARY_ERR          0x1F1
#define ATA_PRIMARY_SECCOUNT     0x1F2
#define ATA_PRIMARY_LBA_LO       0x1F3
#define ATA_PRIMARY_LBA_MID      0x1F4
#define ATA_PRIMARY_LBA_HI       0x1F5
#define ATA_PRIMARY_DRIVE_HEAD   0x1F6
#define ATA_PRIMARY_COMM_REGSTAT 0x1F7
#define ATA_PRIMARY_ALTSTAT_DCR  0x3F6


#define STAT_ERR  (1 << 0) // Indicates an error occurred. Send a new command to clear it
#define STAT_DRQ  (1 << 3) // Set when the drive has PIO data to transfer, or is ready to accept PIO data.
#define STAT_SRV  (1 << 4) // Overlapped Mode Service Request.
#define STAT_DF   (1 << 5) // Drive Fault Error (does not set ERR).
#define STAT_RDY  (1 << 6) // Bit is clear when drive is spun down, or after an error. Set otherwise.
#define STAT_BSY  (1 << 7) // Indicates the drive is preparing to send/receive data (wait for it to clear).
                           // In case of 'hang' (it never clears), do a software reset.
uint32_t num_sectors;

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_HDD :
		//result = RAM_disk_status();

		// translate the reslut code here

		return RES_OK;

	case DEV_MMC :
		//result = MMC_disk_status();

		// translate the reslut code here

		return stat;

	case DEV_USB :
		//result = USB_disk_status();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_HDD :
		inb(ATA_PRIMARY_COMM_REGSTAT);
        outb(ATA_PRIMARY_DRIVE_HEAD, 0xA0);
        inb(ATA_PRIMARY_COMM_REGSTAT);
        outb(ATA_PRIMARY_SECCOUNT, 0);
        inb(ATA_PRIMARY_COMM_REGSTAT);
        outb(ATA_PRIMARY_LBA_LO, 0);
        inb(ATA_PRIMARY_COMM_REGSTAT);
        outb(ATA_PRIMARY_LBA_MID, 0);
        inb(ATA_PRIMARY_COMM_REGSTAT);
        outb(ATA_PRIMARY_LBA_HI, 0);
        inb(ATA_PRIMARY_COMM_REGSTAT);
        outb(ATA_PRIMARY_COMM_REGSTAT, 0xEC);
        outb(ATA_PRIMARY_COMM_REGSTAT, 0xE7);

        // Read the status port. If it's zero, the drive does not exist.
        uint8_t status = inb(ATA_PRIMARY_COMM_REGSTAT);

        printf("[ATA] Waiting for status.\n");
        while(status & STAT_BSY) {
            uint32_t i = 0;
            for(i = 0; i < 0x0FFFFFFF; i++) {}
            printf("[ATA] Checking regstat.\n");
            status = inb(ATA_PRIMARY_COMM_REGSTAT);
        }
        
        if(status == 0) return STA_NOINIT;

        printf("[ATA] Status indicates presence of a drive. Polling while STAT_BSY... ");
        while(status & STAT_BSY) {
        printf("\n[ATA] inb(ATA_PRIMARY_COMM_REGSTAT);... ");
        status = inb(ATA_PRIMARY_COMM_REGSTAT);
        }
        printf("Done.\n");

        uint8_t mid = inb(ATA_PRIMARY_LBA_MID);
        uint8_t hi = inb(ATA_PRIMARY_LBA_HI);
        if(mid || hi) {
            // The drive is not ATA. (Who knows what it is.)
            return STA_NOINIT;
        }

        printf("[ATA] Waiting for ERR or DRQ.\n");
        // Wait for ERR or DRQ
        while(!(status & (STAT_ERR | STAT_DRQ))) {
            status = inb(ATA_PRIMARY_COMM_REGSTAT);
        }

        if(status & STAT_ERR) {
            // There was an error on the drive. Forget about it.
            return STA_NOINIT;
        }

        printf("[ATA] Reading IDENTIFY structure.\n");
        uint8_t buff[256*2];
        insw(ATA_PRIMARY_DATA, buff, 256);
        num_sectors = ((uint32_t) buff[120]) << 24 |
                                    ((uint32_t) buff[121]) << 16 |
                                    ((uint32_t) buff[122]) << 8 |
                                    ((uint32_t) buff[123]);
        printf("[ATA] Number of sectors: %d\n", num_sectors);
        printf("[ATA] Success. Disk is ready to go.\n");
        // We read it!
        return RES_OK;

	case DEV_MMC :
		//result = MMC_disk_initialize();

		// translate the reslut code here

		return stat;

	case DEV_USB :
		//result = USB_disk_initialize();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_HDD :
		// Send drive and bit 24 - 27 of LBA
        outb(0x01F6, (sector >> 24) | 0xE0);

        // Send number of sectors
        outb(0x01F2, 1);

        // Send bit 0 - 7 of LBA
        outb(0x1F3, (uint8_t)(sector & 0xFF));

        // Send bit 8 - 15 of LBA
        outb(0x1F4, (uint8_t)((sector >> 8) & 0xFF));

        // Send bit 16 - 23 of LBA
        outb(0x1F5, (uint8_t)((sector >> 16) & 0xFF));

        // Command port
        outb(0x1F7, 0x20); // Write with retry.

        // Wait until the sector buffer is ready
        uint8_t status;
        do {
            status = inb(0x1F7);
            if(status & STAT_ERR) return RES_ERROR;
        } while (status & STAT_BSY); // Test if the sector buffer requires servicing
        insw(0x1F0, buff, count * 512); // in
        return RES_OK;

	case DEV_MMC :
		// translate the arguments here

		//result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

		//result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_HDD :
		// Send drive and bit 24 - 27 of LBA
        outb(0x01F6, (sector >> 24) | 0xE0);

        // Send number of sectors
        outb(0x01F2, 1);

        // Send bit 0 - 7 of LBA
        outb(0x1F3, (uint8_t)(sector & 0xFF));

        // Send bit 8 - 15 of LBA
        outb(0x1F4, (uint8_t)((sector >> 8) & 0xFF));

        // Send bit 16 - 23 of LBA
        outb(0x1F5, (uint8_t)((sector >> 16) & 0xFF));

        // Command port
        outb(0x1F7, 0x30); // Write with retry.

        // Wait until the sector buffer is ready
        uint8_t status;
        do {
            status = inb(0x1F7);
            if(status & STAT_ERR) return RES_ERROR;
        } while (status & STAT_BSY); // Test if the sector buffer requires servicing
        outsw(0x1F0, buff, count * 512); // out
        return RES_OK;

	case DEV_MMC :
		// translate the arguments here

		//result = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

		//result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_HDD :
        switch(cmd){
            case GET_SECTOR_COUNT:
                *(LBA_t*)buff = num_sectors;
                break;
        }
		// Process of the command for the RAM drive

        return 0;

	case DEV_MMC :

		// Process of the command for the MMC/SD card

		return res;

	case DEV_USB :

		// Process of the command the USB drive

		return res;
	}

	return RES_PARERR;
}
