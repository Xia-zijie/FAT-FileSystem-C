#include "fat.h"

BPB g_bpb;
unsigned short g_fat[TOTAL_CLUSTER] = {0};
DirEntry g_root[ROOT_ENTRY_CNT] = {0};
unsigned char g_disk_buf[TOTAL_CLUSTER * CLUSTER_SIZE] = {0};

void format_disk(void) {
    memset(&g_bpb, 0, sizeof(BPB));
    memset(g_fat, 0, sizeof(g_fat));
    memset(g_root, 0, sizeof(g_root));
    memset(g_disk_buf, 0, sizeof(g_disk_buf));

    g_bpb.bytes_per_sec = SECTOR_SIZE;
    g_bpb.sec_per_clu = CLUSTER_SEC;
    g_bpb.fat_sec_start = 1;
    g_bpb.root_sec_start = 3;
    g_bpb.data_sec_start = 5;

    g_fat[0] = 0xFFF;
    g_fat[1] = 0xFFF;
    save_disk();
}

int load_disk(void) {
    FILE* fp = fopen(DISK_IMG_NAME, "rb");
    if (!fp) return -1;
    fread(&g_bpb, sizeof(BPB), 1, fp);
    fread(g_fat, sizeof(g_fat), 1, fp);
    fread(g_root, sizeof(g_root), 1, fp);
    fread(g_disk_buf, sizeof(g_disk_buf), 1, fp);
    fclose(fp);
    return 0;
}

void save_disk(void) {
    FILE* fp = fopen(DISK_IMG_NAME, "wb");
    if (!fp) return;
    fwrite(&g_bpb, sizeof(BPB), 1, fp);
    fwrite(g_fat, sizeof(g_fat), 1, fp);
    fwrite(g_root, sizeof(g_root), 1, fp);
    fwrite(g_disk_buf, sizeof(g_disk_buf), 1, fp);
    fclose(fp);
}

int get_free_cluster(void) {
    for (int i = 2; i < TOTAL_CLUSTER; i++) {
        if (g_fat[i] == 0) return i;
    }
    return -1;
}

void recycle_cluster_chain(int start_clu) {
    int cur = start_clu;
    while (cur != 0xFFF && cur < TOTAL_CLUSTER) {
        int next = g_fat[cur];
        g_fat[cur] = 0;
        cur = next;
    }
}