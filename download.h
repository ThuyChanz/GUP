#pragma once
#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <windows.h>

typedef struct {
    unsigned char* data;
    size_t size;
} DownloadResult;

DownloadResult* download_shellcode(const char* url);
void free_download_result(DownloadResult* result);

#endif