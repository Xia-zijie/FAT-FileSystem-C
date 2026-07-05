#ifndef FAT_H
#define FAT_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SECTOR_SIZE    512
#define CLUSTER_SEC    1
#define CLUSTER_SIZE   (SECTOR_SIZE * CLUSTER_SEC)
#define TOTAL_CLUSTER  128
#define ROOT_ENTRY_CNT 16
#define DISK_IMG_NAME  "disk.img"

typedef struct {
    unsigned short bytes_per_sec;
    unsigned char sec_per_clu;
    unsigned short fat_sec_start;
    unsigned short root_sec_start;
    unsigned short data_sec_start;
} BPB;

typedef struct {
    char name[8];
    char ext[3];
    unsigned char attr;
    unsigned short start_clu;
    unsigned int size;
} DirEntry;

extern BPB g_bpb;
extern unsigned short g_fat[TOTAL_CLUSTER];
extern DirEntry g_root[ROOT_ENTRY_CNT];
extern unsigned char g_disk_buf[TOTAL_CLUSTER * CLUSTER_SIZE];

void format_disk(void);
int load_disk(void);
void save_disk(void);
int get_free_cluster(void);
void recycle_cluster_chain(int start_clu);

#endif