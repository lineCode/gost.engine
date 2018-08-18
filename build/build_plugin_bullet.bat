cd ..
del bin32\plugins\gtpBullet.gpl

mkdir bin32\plugins\
mkdir bin64\plugins\

cd tools
gost.build.exe ../source/gtpBullet/gost.build.txt -o -r
gost.build.exe ../source/gtpBullet/gost.build.txt -o

pause