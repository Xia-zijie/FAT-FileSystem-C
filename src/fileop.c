#include "fileop.h"

int create_file(const char* name, const char* ext) {
    int clu = get_free_cluster();
    if (clu == -1) return -1;

    DirEntry new_file = {0};
    strncpy(new_file.name, name, 8);
    strncpy(new_file.ext, ext, 3);
    new_file.attr = 0x00;
    new_file.start_clu = clu;
    new_file.size = 0;
    g_fat[clu] = 0xFFF;

    return add_dir_entry(&new_file);
}

int create_dir(const char* dir_name) {
    int clu = get_free_cluster();
    if (clu == -1) return -1;

    DirEntry new_dir = {0};
    strncpy(new_dir.name, dir_name, 8);
    memset(new_dir.ext, ' ', 3);
    new_dir.attr = 0x10;
    new_dir.start_clu = clu;
    new_dir.size = 0;
    g_fat[clu] = 0xFFF;

    unsigned char* clu_buf = g_disk_buf + clu * CLUSTER_SIZE;
    DirEntry self = {0};
    self.name[0] = '.';
    self.attr = 0x10;
    self.start_clu = clu;
    memcpy(clu_buf, &self, sizeof(DirEntry));

    DirEntry parent = {0};
    parent.name[0] = '.'; parent.name[1] = '.';
    parent.attr = 0x10;
    parent.start_clu = 1;
    memcpy(clu_buf + sizeof(DirEntry), &parent, sizeof(DirEntry));

    return add_dir_entry(&new_dir);
}

int copy_file(const char* src_name, const char* src_ext, const char* dst_name, const char* dst_ext) {
    DirEntry src_entry;
    if (find_dir_entry(src_name, src_ext, &src_entry) == -1) return -1;

    int new_clu = get_free_cluster();
    if (new_clu == -1) return -1;
    g_fat[new_clu] = 0xFFF;

    unsigned char* src_data = g_disk_buf + src_entry.start_clu * CLUSTER_SIZE;
    unsigned char* dst_data = g_disk_buf + new_clu * CLUSTER_SIZE;
    memcpy(dst_data, src_data, CLUSTER_SIZE);

    DirEntry dst_entry = {0};
    strncpy(dst_entry.name, dst_name, 8);
    strncpy(dst_entry.ext, dst_ext, 3);
    dst_entry.attr = 0x00;
    dst_entry.start_clu = new_clu;
    dst_entry.size = src_entry.size;

    return add_dir_entry(&dst_entry);
}