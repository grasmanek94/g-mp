@echo off
cd /d %~dp0
cd ../Common/Common.Version
copy /b Version.cxx +,,
echo touched version.cxx ...

git rev-list HEAD --count > auto_version.h
FOR /f "tokens=1" %%a in ('findstr /R "^[0-9][0-9]*" auto_version.h') do set _BUILD_VERSION=%%a

echo #define BUILD_VERSION %_BUILD_VERSION% > auto_version.h
echo Updated version to %_BUILD_VERSION%
REM exit 0
