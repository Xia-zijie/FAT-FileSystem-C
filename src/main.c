#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileop.h"

// 打印磁盘簇状态可视化
void print_disk_status() {
    printf("\n===== 磁盘簇状态可视化 =====\n");
    for(int i = 2; i < TOTAL_CLUSTER; i++){
        if(g_fat[i] == 0)
            printf("□"); // 空闲
        else
            printf("■"); // 已占用
        if(i % 32 == 0) printf("\n");
    }
    printf("\n□空闲簇 ■已占用簇\n");
}

// 打印目录列表
void print_dir() {
    printf("\n===== 当前根目录文件列表 =====\n");
    for(int i = 0; i < ROOT_ENTRY_CNT; i++){
        if(g_root[i].name[0] == 0) continue;
        printf("文件名:%.8s.%.3s | 起始簇:%d | 大小:%d | 属性:%s\n",
            g_root[i].name,
            g_root[i].ext,
            g_root[i].start_clu,
            g_root[i].size,
            g_root[i].attr == 0x10 ? "目录" : "文件");
    }
}

void menu() {
    printf("\n========= 操作系统FAT文件系统模拟 =========\n");
    printf("1. 新建文件 TEST.TXT\n");
    printf("2. 新建目录 MYDIR\n");
    printf("3. 删除文件 TEST.TXT\n");
    printf("4. 复制 TEST.TXT -> COPY.TXT\n");
    printf("5. 刷新磁盘与目录显示\n");
    printf("0. 退出程序\n");
    printf("==========================================\n");
    printf("请输入操作序号：");
}

int main() {
    if(load_disk() == -1){
        printf("首次运行，初始化虚拟磁盘！\n");
        format_disk();
    }

    int op;
    while(1) {
        menu();
        scanf("%d", &op);
        switch(op) {
            case 1:
                create_file("TEST", "TXT");
                printf("✅ 创建文件成功\n");
                break;
            case 2:
                create_dir("MYDIR");
                printf("✅ 创建目录成功\n");
                break;
            case 3:
                del_dir_entry("TEST", "TXT");
                printf("✅ 删除成功\n");
                break;
            case 4:
                copy_file("TEST", "TXT", "COPY", "TXT");
                printf("✅ 复制文件成功\n");
                break;
            case 5:
                break;
            case 0:
                printf("程序退出\n");
                return 0;
            default:
                printf("输入错误！\n");
        }
        print_disk_status();
        print_dir();
    }
    return 0;
}