# Makefile for extracting image files from directories

# 编译器设置
CC = gcc
CFLAGS = -Wall -Wextra -g

# 源文件和目标文件
SRC = extract_images.c
OBJ = extract_images.o
TARGET = extract_images

# 默认目标
all: $(TARGET)

# 链接目标文件生成可执行文件
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# 编译源文件为目标文件
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

# 清理目标
clean:
	rm -f $(OBJ) $(TARGET)

# 执行程序
run: $(TARGET)
	./$(TARGET)
