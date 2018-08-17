rmdir /s /q bin32\plugins

mkdir ..\build\objects\gtiiBMP\debug\
mkdir ..\build\objects\gtiiBMP\release\
gost.build.exe ../source/gtiiBMP/gost.build.txt

mkdir ..\build\objects\gtpBullet\debug\
mkdir ..\build\objects\gtpBullet\release\
gost.build.exe ../source/gtpBullet/gost.build.txt

mkdir ..\build\objects\gtrD3D11\debug\
mkdir ..\build\objects\gtrD3D11\release\
gost.build.exe ../source/gtrD3D11/gost.build.txt

mkdir ..\build\objects\gtimOBJ\debug\
mkdir ..\build\objects\gtimOBJ\release\
gost.build.exe ../source/gtimOBJ/gost.build.txt

mkdir ..\build\objects\gtiiPNG\debug\
mkdir ..\build\objects\gtiiPNG\release\
gost.build.exe ../source/gtiiPNG/gost.build.txt

pause