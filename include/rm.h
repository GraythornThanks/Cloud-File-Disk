#pragma once


// 删除指定文件/目录
// 成功返回0, 失败返回-1
int removeAllFile(const char* filepath);

// 删除除目录以外的所有文件
// 成功返回0, 失败返回-1
int removeFile(const char* filepath);

// 递归删除目录及其内容
// 成功返回0, 失败返回-1
int removeDir(const char* dirpath);
