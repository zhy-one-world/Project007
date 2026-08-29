@echo off
setlocal enabledelayedexpansion

set "PROTO_DIR=E:\Project\Project007\server_shipping\Res\protocol"
set "PROTOC_EXE=E:\Project\dep\vcpkg\installed\x64-windows\tools\protobuf\protoc.exe"
set "CPP_OUT=E:\Project\Project007\server\def\protocol\internet"

if not exist "%PROTO_DIR%" (
    echo [ERROR] Proto source directory not found: %PROTO_DIR%
    pause
    exit /b 1
)

if not exist "%PROTOC_EXE%" (
    echo [ERROR] protoc.exe not found: %PROTOC_EXE%
    pause
    exit /b 1
)

if not exist "%CPP_OUT%" mkdir "%CPP_OUT%"

echo [INFO] Generating C++ protobuf files...
for %%f in ("%PROTO_DIR%\*.proto") do (
    echo [INFO] %%~nxf
    "%PROTOC_EXE%" --proto_path="%PROTO_DIR%" --cpp_out="%CPP_OUT%" "%%~ff"
)

echo [INFO] Done.
pause