@echo off

rem "此处使用绝对路径，寻找UV4.exe的完整路径"
rem "如果找不到，请自行加入本机Keil环境下的UV4.exe路径"
rem "说明:UV4.exe 的必要的参数说明:"
rem "说明:UV4.exe [命令] [工程文件uvprojx] [可选命令]"
rem "说明:非必要参数 -0 输出编译信息到flash_log文件"

if not exist "C:\Keil_v5\UV4\UV4.exe" goto PATH1
set "uvpath="C:\Keil_v5\UV4\UV4.exe""
goto EXECUTION

:PATH1
if not exist "C:\Keil_v5\UV4\UV4.exe" goto PATH2
set "uvpath="C:\Keil_v5\UV4\UV4.exe""
goto EXECUTION

:PATH2
if not exist "C:\Keil_v5\UV4\UV4.exe" goto PATH3
set "uvpath="C:\Keil_v5\UV4\UV4.exe""
goto EXECUTION

:PATH3
if not exist "C:\Keil_v5\UV4\UV4.exe" goto PATHEND
set "uvpath="C:\Keil_v5\UV4\UV4.exe""
goto EXECUTION

:PATHEND
set "uvpath="C:\Keil_v5\UV4\UV4.exe""
goto EXECUTION

:EXECUTION
set prodir=%~dp0powerclean.uvprojx
echo Init flashing ... (%prodir%)
echo .>flash_log.txt
%uvpath% -j0 -f %prodir% -o %cd%\flash_log.txt
type flash_log.txt
echo ---------------------------------------------------0.Flash Done.
del %~dp0flash_log.txt
del /a/f/q %~dp0DebugConfig
rd /s/q %~dp0DebugConfig
del /a/f/q %~dp0RTE
rd /s/q %~dp0RTE
pause.