
chcp 65001

tskill rgamews
tskill rgamecs
tskill rgamefep
tskill rgamedp

echo 防止进程没有退干净，延迟3秒再copy进程 
timeout /t 3 /nobreak

COPY /y ws.exe rgamews.exe
COPY /y cs.exe rgamecs.exe
COPY /y fep.exe rgamefep.exe
COPY /y dp.exe rgamedp.exe

START rgamews.exe
START rgamefep.exe
START rgamedp.exe
START rgamecs.exe

exit