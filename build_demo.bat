del bin32\base_demo.exe
del bin32\Demo_release.exe

cd tools

rmdir /s /q ..\build\objects\base_demo\debug\
rmdir /s /q ..\build\objects\base_demo\release\


mkdir ..\build\objects\base_demo\debug\
mkdir ..\build\objects\base_demo\release\


rmdir /s /q ..\build\objects\Demo\debug\
rmdir /s /q ..\build\objects\Demo\release\

mkdir ..\build\objects\Demo\debug\
mkdir ..\build\objects\Demo\release\

gost.build.exe ../demo/examples/gost.build.txt
gost.build.exe ../demo/gost.build.txt

pause