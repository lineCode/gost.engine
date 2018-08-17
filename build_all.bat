rmdir /s /q bin32
rmdir /s /q bin64
cd build
rmdir /s /q objects
mkdir objects
cd ..
mkdir bin32\plugins\
mkdir bin64\plugins\

cd tools

mkdir ..\build\objects\gost\debug\
mkdir ..\build\objects\gost\release\
gost.build.exe ../source/gost/gost.build.txt -o

mkdir ..\build\objects\base_demo\debug\
mkdir ..\build\objects\base_demo\release\
mkdir ..\build\objects\Demo\debug\
mkdir ..\build\objects\Demo\release\
gost.build.exe ../demo/examples/gost.build.txt -o
gost.build.exe ../demo/gost.build.txt -o

mkdir ..\build\objects\gtiiBMP\debug\
mkdir ..\build\objects\gtiiBMP\release\
gost.build.exe ../source/gtiiBMP/gost.build.txt -o

mkdir ..\build\objects\gtpBullet\debug\
mkdir ..\build\objects\gtpBullet\release\
gost.build.exe ../source/gtpBullet/gost.build.txt -o

mkdir ..\build\objects\gtrD3D11\debug\
mkdir ..\build\objects\gtrD3D11\release\
gost.build.exe ../source/gtrD3D11/gost.build.txt -o

mkdir ..\build\objects\gtimOBJ\debug\
mkdir ..\build\objects\gtimOBJ\release\
gost.build.exe ../source/gtimOBJ/gost.build.txt -o

mkdir ..\build\objects\gtiiPNG\debug\
mkdir ..\build\objects\gtiiPNG\release\
gost.build.exe ../source/gtiiPNG/gost.build.txt -o

pause