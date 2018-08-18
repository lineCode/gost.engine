cd ..
del bin32\plugins\gtiiBMP.gpl

mkdir bin32\plugins\
mkdir bin64\plugins\

cd tools
gost.build.exe ../source/gtiiBMP/gost.build.txt -o -r
gost.build.exe ../source/gtiiBMP/gost.build.txt -o

pause