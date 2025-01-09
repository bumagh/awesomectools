#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_PATH_LENGTH 1024

// 判断文件是否是图片文件
int is_image(const char *filename) {
    const char *image_extensions[] = {".jpg", ".jpeg", ".png", ".bmp", ".gif", ".tiff", ".webp"};
    size_t num_extensions = sizeof(image_extensions) / sizeof(image_extensions[0]);
    
    for (size_t i = 0; i < num_extensions; ++i) {
        if (strstr(filename, image_extensions[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

// 复制文件到指定目标路径
int copy_file(const char *source, const char *destination) {
    FILE *src_file = fopen(source, "rb");
    if (!src_file) {
        perror("open src error");
        return -1;
    }
    
    FILE *dest_file = fopen(destination, "wb");
    if (!dest_file) {
        perror("open dist error");
        fclose(src_file);
        return -1;
    }
    
    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
        fwrite(buffer, 1, bytes, dest_file);
    }

    fclose(src_file);
    fclose(dest_file);
    
    return 0;
}

// 递归遍历文件夹并提取图片文件
void extract_images(const char *folder_path, const char *target_folder) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    // 组合路径，添加通配符来查找所有文件和目录
    char search_path[MAX_PATH_LENGTH];
    snprintf(search_path, MAX_PATH_LENGTH, "%s\\*", folder_path);

    // 查找第一个文件
    hFind = FindFirstFile(search_path, &findFileData);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("无法打开文件夹 %s\n", folder_path);
        return;
    }

    do {
        const char *file_name = findFileData.cFileName;
        if (strcmp(file_name, ".") == 0 || strcmp(file_name, "..") == 0) {
            continue;  // 忽略当前目录和上级目录
        }

        char current_path[MAX_PATH_LENGTH];
        snprintf(current_path, MAX_PATH_LENGTH, "%s\\%s", folder_path, file_name);

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // 如果是目录，递归遍历
            extract_images(current_path, target_folder);
        } else if (is_image(file_name)) {
            // 如果是文件且是图片文件，复制到目标目录
            char target_path[MAX_PATH_LENGTH];
            snprintf(target_path, MAX_PATH_LENGTH, "%s\\%s", target_folder, file_name);
            
            if (copy_file(current_path, target_path) == 0) {
                printf("已复制文件: %s -> %s\n", current_path, target_path);
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    // 关闭查找句柄
    FindClose(hFind);
}

int main() {
    char source_folder[MAX_PATH_LENGTH];
    char target_folder[MAX_PATH_LENGTH];
    
    // 输入源文件夹路径
    printf("Please input src path: ");
    fgets(source_folder, MAX_PATH_LENGTH, stdin);
    source_folder[strcspn(source_folder, "\n")] = '\0';  // 去掉换行符
    
    // 输入目标文件夹路径
    printf("Please input dist path: ");
    fgets(target_folder, MAX_PATH_LENGTH, stdin);
    target_folder[strcspn(target_folder, "\n")] = '\0';  // 去掉换行符

    // 递归提取图片文件
    extract_images(source_folder, target_folder);

    return 0;
}
