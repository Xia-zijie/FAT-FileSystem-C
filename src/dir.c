#include "dir.h"

int find_empty_dir_entry(void) {
    for (int i = 0; i < ROOT_ENTRY_CNT; i++) {
        if (g_root[i].name[0] == 0) return i;
    }
    return -1;
}

int add_dir_entry(DirEntry* entry) {
    int idx = find_empty_dir_entry();
    if (idx == -1) return -1;
    memcpy(&g_root[idx], entry, sizeof(DirEntry));
    save_disk();
    return 0;
}

int find_dir_entry(const char* name, const char* ext, DirEntry* out) {
    for (int i = 0; i < ROOT_ENTRY_CNT; i++) {
        if (strncmp(g_root[i].name, name, 8) == 0 && strncmp(g_root[i].ext, ext, 3) == 0) {
            memcpy(out, &g_root[i], sizeof(DirEntry));
            return i;
        }
    }
    return -1;
}

int del_dir_entry(const char* name, const char* ext) {
    DirEntry entry;
    int idx = find_dir_entry(name, ext, &entry);
    if (idx == -1) return -1;
    recycle_cluster_chain(entry.start_clu);
    memset(&g_root[idx], 0, sizeof(DirEntry));
    save_disk();
    return 0;
}