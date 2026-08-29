@echo off

echo "begin server pb"
..\..\..\dep\vcpkg\installed\x64-windows\tools\protobuf\protoc  -I=.\ --cpp_out=..\..\..\Project007\server\def\protocol\internet\    .\*.proto

pause