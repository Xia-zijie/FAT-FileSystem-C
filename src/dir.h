#ifndef DIR_H
#define DIR_H
#include "fat.h"

int find_empty_dir_entry(void);
int add_dir_entry(DirEntry* entry);
int find_dir_entry(const char* name, const char* ext, DirEntry* out);
int del_dir_entry(const char* name, const char* ext);

#endif