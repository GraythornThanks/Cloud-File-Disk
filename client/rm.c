#include"../include/all.h"

int removeAllFile(const char* filename) {
    int ret = 0;

    // 文件名为空, 删除文件失败返回-1
    int fileNameLen = strlen(filename);
    if (fileNameLen == 0)
        return -1;

    // 拼接路径
    char currWorkDir[50] = { 0 };
    getcwd(currWorkDir, sizeof(currWorkDir));
    char realFilePath[100] = { 0 };
    sprintf(realFilePath, "%s/%s", currWorkDir, filename);

    struct stat statbuf;
    memset(&statbuf, 0, sizeof(statbuf));

    stat(realFilePath, &statbuf);

    if (S_ISDIR(statbuf.st_mode)) { // 所输入的文件类型为目录
        ret = removeDir(realFilePath);
        if (ret == -1)
            printf("Remove %s failed.\n", realFilePath);
    }
    else { // 除目录以外的类型均视为普通文件
        ret = removeFile(realFilePath);
        if (ret == -1)
            printf("Remove %s failed.\n", realFilePath);
    }
    return ret;
}

int removeFile(const char* filepath) {
    int ret = remove(filepath);
    return ret;
}

int removeDir(const char* dirpath) {
    int ret = 0;

    struct dirent* pdirent;
    DIR* pdir = opendir(dirpath);

    while ((pdirent = readdir(pdir)) != NULL) {
        // 忽略隐藏文件
        char* name = pdirent->d_name;
        if (name[0] == '.') continue;

        // 拼接路径
        char filepath[100] = { 0 };
        sprintf(filepath, "%s/%s", dirpath, name);

        // 判断当前文件类型并删除文件
        if (pdirent->d_type == DT_DIR) {
            /* 提示用户该文件类型为目录，是否递归删除该目录
             * 如果是，则执行下列操作
             */
            ret = removeDir(filepath);
            if (ret == -1) {
                printf("Remove %s failed.\n", filepath);
                return ret;
            }
            /* 否，则设置 ret 为指定值，告诉用户删除取消 */
            if (ret == -2)
                printf("Remove %s cancelled.\n", filepath);
        }    
        else {
            ret = removeFile(filepath);
            if (ret == -1) {
                printf("Remove %s failed.\n", filepath);
                return ret;
            }
            else
                printf("Remove %s successfully.\n", filepath);
        }
    }
    // 删除完当前目录下的全部文件后, 删除目录本身
    ret = removeFile(dirpath);
    printf("Remove %s successfully.\n", dirpath);
    return ret;
}
