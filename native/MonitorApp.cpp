#include <jni.h>
#include <windows.h>
#include <cstdio>

// Tiny helper to convert Windows time formats to simple numbers
unsigned long long ft2ull(FILETIME f) {
    return ((unsigned long long)f.dwHighDateTime << 32) | f.dwLowDateTime;
}

extern "C" JNIEXPORT jstring JNICALL Java_MonitorApp_getStats(JNIEnv* env, jobject obj) {
    // 1. Calculate CPU
    static unsigned long long prevIdle = 0, prevKernel = 0, prevUser = 0;
    static bool hasPrevious = false;
    FILETIME idle, kernel, user;
    if (!GetSystemTimes(&idle, &kernel, &user)) {
        return env->NewStringUTF("Error: unable to read CPU");
    }

    unsigned long long cIdle = ft2ull(idle), cKernel = ft2ull(kernel), cUser = ft2ull(user);
    
    double cpu = 0.0;
    if (hasPrevious) {
        double total = (cKernel - prevKernel) + (cUser - prevUser);
        if (total != 0.0) {
            cpu = 100.0 * (1.0 - (double)(cIdle - prevIdle) / total);
        }
    }
    
    prevIdle = cIdle; prevKernel = cKernel; prevUser = cUser;
    hasPrevious = true;

    // 2. Calculate RAM
    MEMORYSTATUSEX mem;
    mem.dwLength = sizeof(mem);
    if (!GlobalMemoryStatusEx(&mem)) {
        return env->NewStringUTF("Error: unable to read RAM");
    }
    
    double totalGb = mem.ullTotalPhys / 1073741824.0;
    double usedGb = totalGb - (mem.ullAvailPhys / 1073741824.0);

    // 3. Return String
    char buffer[100];
    std::snprintf(buffer, sizeof(buffer), "CPU: %.0f%% | RAM: %.1f/%.1f GB", cpu, usedGb, totalGb);
    return env->NewStringUTF(buffer);
}