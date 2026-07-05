#ifndef FILEOP_H
#define FILEOP_H
#include "dir.h"

int create_file(const char* name, const char* ext);
int create_dir(const char* dir_name);
int copy_file(const char* src_name, const char* src_ext, const char* dst_name, const char* dst_ext);

#endif