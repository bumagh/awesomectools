#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <windows.h>  // 需要用到 Windows API 函数来处理文件路径

void rename_files_in_directory(const char *directory_path) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    // 拼接目录路径和通配符（*）以列出所有文件
    char searchPath[MAX_PATH];
    snprintf(searchPath, sizeof(searchPath), "%s\\*", directory_path);

    // 使用 FindFirstFile 和 FindNextFile 遍历文件
    hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Unable to open directory: %s\n", directory_path);
        return;
    }

    do {
        // 跳过 "." 和 ".."
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0) {
            continue;
        }

        // 获取当前文件的完整路径
        char old_name[MAX_PATH];
        snprintf(old_name, sizeof(old_name), "%s\\%s", directory_path, findFileData.cFileName);

        // 如果文件名已经以 .png 结尾，跳过
        if (strstr(findFileData.cFileName, ".png") != NULL) {
            continue;
        }

        // 构造新的文件名，添加 .png 后缀
        char new_name[MAX_PATH];
        snprintf(new_name, sizeof(new_name), "%s\\%s.png", directory_path, findFileData.cFileName);

        // 重命名文件
        if (rename(old_name, new_name) == 0) {
            printf("Renamed: %s -> %s\n", old_name, new_name);
        } else {
            perror("rename");
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    // 关闭句柄
    FindClose(hFind);
}

int main() {
    const char *directory_path = "D:\\2work\\3gamedev\\inbox\\gongyi\\mini"; // 修改为你自己的路径
    rename_files_in_directory(directory_path);
    return 0;
}
