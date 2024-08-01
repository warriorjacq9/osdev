#include <stdint.h>

typedef struct fat_bpb // The first table in a FAT header. Contains info about the device and the "geometry" (sectors per track, cluster size, etc.) of the device
{
    unsigned char   bootjmp[3]; // Boot jump section, usually EB 3C 90, which dissasembles to JMP SHORT 3C NOP.
    unsigned char   oem_name[8]; // The version of DOS being used. Normally MSWIN4.1
    uint16_t        sector_size; // The number of bytes per sector
    uint8_t         cluster_size; // The number of sectors per cluster
    uint16_t        reserved_size; // The number of reserved sectors, including boot record sectors
    uint8_t         num_tables; // The number of File Allocation Tables on disk
    uint16_t        root_dirs; // Number of root directory entries
    uint16_t        total_sectors; // Number of total secttors in the logical volume. If this value is 0, then there are more than 65,535 sectors and the value is stored in total_sectors_large at offset 0x20
    /* What type of media the device is. 
     * | Value | Type |
     * |:------|:----:|
     * | 0xE5  | 8-inch floppy, 77 track, 26 sector per track, 128 bytes per sector |
     * | 0xED  | 5.25-inch 2-sided floppy, 80 track, 9 sector |
     * | 0xEE  | Non-standard custom partition, not recognized |
     * | 0xEF  | Non-standard superfloppy, not recognized |
     * | 0xF0  | Other |
     * | 0xF4  | Double density |
     * | 0xF5  | Fixed 4-sided disk, 12 sectors per track |
     * | 0xF8  | Partition or removable media |
     * | 0xF9  | 3.5-inch 2-sided floppy, 80 track, 9 sector |
     * | 0xFA  | ROM disk |
     * | 0xFB  | 3.5 and 5.25-inch 2 sided floppy, 80 track, 8 sector |
     * | 0xFC  | 5.25-inch floppy, 40 track, 9 sector |
     * | 0xFD  | 8-inch 2-sided floppy, 77 track, 26 sector, 128 byte |
     * | 0xFE  | 5.25-inch floppy, 40 track, 8 sector |
     * | 0xFF  | 5.25-inch 2-sided floppy, 40 track, 8 sector |
    */
    unsigned char   media_desc;
    uint16_t        fat_size; // The number of sectors per File Allocation Table. FAT12/FAT16 only
    uint16_t        track_size; // The number of sectors per track
    uint16_t        num_sides; // The number of heads or sides on the device
    uint32_t        num_hidden; // The number of hidden sectors
    uint32_t        total_sectors_large; // The large sector count. Is set only if the sector count is more than 65,535
};

typedef struct fat32_bpb_ext // The second table in a FAT32 header, contains filesystem-specific information
{
    uint32_t        fat_size; // The number of sectors
    unsigned char   flags[2]; // Flags. Bits 0-3 are the number of active FATs if bit 7 is set
    uint8_t         version[2]; // The FAT version number. The high byte is the major version, and the low byte is the minor version
    uint32_t        root_cluster; // The cluster number of the root directory. Often set to 2
    uint16_t        fsinfo_sector; // The sector number of the FSInfo structure
    uint16_t        bkup_sector; // The sector number of the backup boot sector
    unsigned char   reserved[12]; // These bytes are reserved. Should be set to 0 when formatting
    uint8_t         drive_num; // The number of the drive. Identical to the return of INT 0x13, floppy drives start at 0 and hard disks start at 128. CD/DVD is number 224
    unsigned char   nt_flags; // Flags for Windows NT. Reserved otherwise
    unsigned char   signature; // Disk signature. Must de either 0x28 or 0x29
    uint32_t        volume_id; // The volume ID, used for tracking volumes. Can be ignored
    unsigned char   label[11]; // The volume label string. It is padded with spaces
    unsigned char   sys_id[8]; // The system identifier string, always "FAT32   ". Do not trust for any use
    unsigned char   boot_code[420]; // The boot code.
    uint16_t        boot_sig[2]; // Bootable partition signature. 0xAA55 if bootable
};

typedef struct fat32_fsinfo
{
    uint32_t        signature_lead; // Lead signature, must be 0x41615252
    unsigned char   reserved[480]; // Reserved bytes, these should not be used
    uint32_t        signature; // Second signature, must be 0x61417272
    uint32_t        free_clusters; // Last known free cluster count. If it is 0xFFFFFFFF, it must be computed. However, it may be incorrect and should be range checked
    uint32_t        available_clusters; // Contains the cluster number at which the driver should look for available clusters. If it is 0xFFFFFFFF, the the driver should start at 2. Typically set to the last allocated cluster, and should be range checked
    unsigned char   reserved_more[12]; // More reserved bytes
    uint32_t        signature_trail; // Trail signature, must be 0xAA550000
};

#define FAT_TYPE_READONLY   0x01
#define FAT_TYPE_HIDDEN     0x02
#define FAT_TYPE_SYSTEM     0x04
#define FAT_TYPE_VOLID      0x08
#define FAT_TYPE_DIRECTORY  0x10
#define FAT_TYPE_ARCHIVE    0x20
#define FAT_TYPE_DEVICE     0x40
#define FAT_TYPE_RESERVED   0x80

typedef struct fat_entry // An 8.3 file entry. Goes in the FAT
{
    unsigned char   name[11]; // 8.3 File name. The first 8 characters are the name, and the last 3 are the file extension. If the first byte is 0xE5, the file has been erased, and if it is 0x05, the character is actually 0xE5
    /*
     * The file type flag.
     * | Value | File type |
     * |:-----:|:---------:|
     * | 0x01  | Read-only |
     * | 0x02  | Hidden    |
     * | 0x04  | System    |
     * | 0x08  | Volume ID |
     * | 0x10  | Directory |
     * | 0x20  | Archive   |
     * | 0x40  | Device    |
     * | 0x80  | Reserved
    */
    unsigned char   type;
    unsigned char   reserved; // Reserved for Windows NT
    uint8_t         creation_ds; // The creation time of the file in hundreths of a second (deciseconds)
    /*
     * The time that the file was created. Multiply seconds by 2
     * |      |        |
     * |:----:|:-------:|
     * | Hour | 5 bits |
     * | Minutes | 6 bits |
     * | Seconds | 5 bits |
    */
    uint16_t        creation_time;
    /*
     * The date that the file was creates on.
     * |      |        |
     * |:----:|:------:|
     * | Year | 7 bits |
     * | Month| 4 bits |
     * | Day  | 5 bits |
    */
   uint16_t         creation_date;
   uint16_t         access_date; // Last accessed date. Same format as creation date
   uint16_t         cluster_high; // High 16 bits of the entry's first cluster number. Always 0 for fat12/16
   uint16_t         mod_time; // Last modification time. Same format as creation time
   uint16_t         mod_date; // Last modification date. Same format as creation date
   uint16_t         cluster_low; // Low 16 bits of the entry's first cluster number
   uint32_t         file_size; // The size of the file in bytes
};

typedef struct fat_entry_lfn // The long filename entry. Always placed immediately before the 8.3 entry
{
    unsigned char   seq_number; // The order of the entry in the sequence of LFN (long filename) entries.
    uint16_t        name_first[5]; // The first 5 UTF-16 characters of the filename
    unsigned char   type; // The entry type flag. Always equals 0x0F, the long filename attribute
    unsigned char   checksum; // The checksum generated of the corresponding short filename
    uint16_t        name[6]; // The next 6 UTF-16 characters of the filename
    unsigned char   reserved[2]; // Reserved bytes, always 0
    uint16_t        name_last[2]; // The final 2 UTF-16 characters of the filename
};

typedef struct fat32 // The FAT 32 filesystem structure
{
    struct fat_bpb bpb;
    struct fat32_fsinfo fsinfo;
    
};