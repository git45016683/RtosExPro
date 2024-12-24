@echo off

rem "此处使用绝对路径，寻找UV4.exe的完整路径"
rem "如果找不到，请自行加入本机VS环境下的UV4.exe路径"
rem "说明:UV4.exe 的必要的参数说明:"
rem "说明:UV4.exe [命令] [工程文件uvprojx] [可选命令]"
rem "说明:非必要参数 -0 输出编译信息到build_log文件"

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
echo Init building ... (%prodir%)
echo .>build_log.txt
%uvpath% -b -j0 %prodir% -t "powerclean" -o %cd%\build_log.txt
type build_log.txt | findstr /I error
echo ---------------------------------------------------0.Build Done.
