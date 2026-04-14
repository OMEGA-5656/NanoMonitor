@echo off
setlocal

if "%JAVA_HOME%"=="" (
  echo JAVA_HOME is not set.
  echo Set JAVA_HOME to your JDK path and run again.
  exit /b 1
)

if not exist build mkdir build
if not exist build\classes mkdir build\classes
if not exist build\native mkdir build\native

javac -h native -d build\classes src\MonitorApp.java
if errorlevel 1 exit /b 1

g++ -shared -O2 -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" native\MonitorApp.cpp -o build\native\stats_bridge.dll
if errorlevel 1 exit /b 1

java -Djava.library.path=build\native -cp build\classes MonitorApp
