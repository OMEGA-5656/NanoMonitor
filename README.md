# NanoMonitor

NanoMonitor is a very small desktop monitor app made for learning Java and C++ together.
It uses Java Swing for the window and JNI to call native C++ code for system stats.

Right now this version is focused on Windows only, to keep the code simple and easy to explain.

## What this project does

- Shows CPU usage and RAM usage
- Updates stats automatically every 1000ms
- Reads real system data from Windows APIs using C++

## How it works in simple steps

1. Java starts the Swing window.
2. Java loads a native library named stats_bridge.dll.
3. Timer and button both call one native method: getStats().
4. C++ reads CPU and memory values from Windows.
5. C++ returns one formatted text line.
6. Java shows that text on the button.

This is purposely kept tiny so interview discussion becomes easy.

## Tech stack

- Java (Swing)
- C++ (Windows API)
- JNI bridge between Java and C++

## Project structure

- src/MonitorApp.java: Swing UI and native method call
- native/MonitorApp.cpp: CPU and RAM collection logic
- build-and-run-windows.bat: one command build and run script

## Run on Windows

Prerequisites:

- JDK installed
- g++ available in PATH

Then run:

```bat
build-and-run-windows.bat
```

If JAVA_HOME is not set, set it first in PowerShell:

```powershell
$env:JAVA_HOME = "C:\Program Files\Java\jdk-25.0.2"
$env:Path = "$env:JAVA_HOME\bin;C:\msys64\ucrt64\bin;$env:Path"
```

```cmd prompt
set JAVA_HOME = "C:\Program Files\Java\jdk-25.0.2"
set Path = "$env:JAVA_HOME\bin;C:\msys64\ucrt64\bin;$env:Path"
```

## Why this project is useful

- Good for understanding Java to native integration
- Good first JNI project because code size is small
- Useful for learning basic system level monitoring

## Potential applications

- Lightweight desktop widget for quick health check
- Learning base for bigger monitor app with graphs later
- Starter architecture for diagnostics tools in embedded or automotive style software
- Can be used in load balancing applications
