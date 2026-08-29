svn update
cd ..\..\document\策划文档\数值维护
svn update
lua_server.py
cd .\数据导出表\UTF8导出结果
copy *.csv ..\..\..\..\..\server_shipping\res\csv
copy *.csv ..\..\..\..\..\tools_shipping\res\csv
pause  