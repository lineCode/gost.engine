cd ..
rmdir /s /q bin32\plugins

mkdir bin32\plugins\
mkdir bin64\plugins\

cd tools

gost.build.exe ../source/gtaXAudio2/gost.build.txt -o -r
gost.build.exe ../source/gtaXAudio2/gost.build.txt -o

gost.build.exe ../source/gtiiBMP/gost.build.txt -o -r
gost.build.exe ../source/gtiiBMP/gost.build.txt -o

gost.build.exe ../source/gtpBullet/gost.build.txt -o -r
gost.build.exe ../source/gtpBullet/gost.build.txt -o

gost.build.exe ../source/gtrD3D11/gost.build.txt -o -r
gost.build.exe ../source/gtrD3D11/gost.build.txt -o

gost.build.exe ../source/gtimOBJ/gost.build.txt -o -r
gost.build.exe ../source/gtimOBJ/gost.build.txt -o

gost.build.exe ../source/gtiiPNG/gost.build.txt -o -r
gost.build.exe ../source/gtiiPNG/gost.build.txt -o

pause