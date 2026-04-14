#include <jni.h>
#include <windows.h>
#include <cstdio>

namespace {
ULONGLONG fileTimeToUInt64(const FILETIME& ft) {
    ULARGE_INTEGER value;
    value.LowPart = ft.dwLowDateTime;
    value.HighPart = ft.dwHighDateTime;
    return value.QuadPart;
}

double readCpuPercent() {
    static bool hasPrevious = false;
    static ULONGLONG prevIdle = 0;
    static ULONGLONG prevKernel = 0;
    static ULONGLONG prevUser = 0;

    FILETIME idleTime;
    FILETIME kernelTime;
    FILETIME userTime;
    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        return -1.0;
    }

    ULONGLONG idle = fileTimeToUInt64(idleTime);
    ULONGLONG kernel = fileTimeToUInt64(kernelTime);
    ULONGLONG user = fileTimeToUInt64(userTime);

    if (!hasPrevious) {
        hasPrevious = true;
        prevIdle = idle;
        prevKernel = kernel;
        prevUser = user;
        return 0.0;
    }

    ULONGLONG idleDelta = idle - prevIdle;
    ULONGLONG kernelDelta = kernel - prevKernel;
    ULONGLONG userDelta = user - prevUser;
    ULONGLONG totalDelta = kernelDelta + userDelta;

    prevIdle = idle;
    prevKernel = kernel;
    prevUser = user;

    if (totalDelta == 0) {
        return 0.0;
    }

    double usage = 100.0 * (1.0 - static_cast<double>(idleDelta) / static_cast<double>(totalDelta));
    if (usage < 0.0) {
        usage = 0.0;
    }
    if (usage > 100.0) {
        usage = 100.0;
    }
    return usage;
}

bool readMemory(double& usedGb, double& totalGb) {
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);
    if (!GlobalMemoryStatusEx(&memoryStatus)) {
        return false;
    }

    const double bytesToGb = 1024.0 * 1024.0 * 1024.0;
    totalGb = static_cast<double>(memoryStatus.ullTotalPhys) / bytesToGb;
    double freeGb = static_cast<double>(memoryStatus.ullAvailPhys) / bytesToGb;
    usedGb = totalGb - freeGb;
    return true;
}
}

extern "C" JNIEXPORT jstring JNICALL Java_MonitorApp_getStats(JNIEnv* env, jobject /*obj*/) {
    double cpuPercent = readCpuPercent();
    if (cpuPercent < 0.0) {
        return env->NewStringUTF("Error: unable to read CPU");
    }

    double usedGb = 0.0;
    double totalGb = 0.0;
    if (!readMemory(usedGb, totalGb)) {
        return env->NewStringUTF("Error: unable to read RAM");
    }

    char buffer[128];
    std::snprintf(buffer, sizeof(buffer), "CPU: %.1f%% | RAM: %.1f / %.1f GB", cpuPercent, usedGb, totalGb);
    return env->NewStringUTF(buffer);
}
