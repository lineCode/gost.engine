cd ..
del bin32\plugins\gtsLua.gpl

mkdir bin32\plugins\
mkdir bin64\plugins\

cd tools
gost.build.exe ../source/gtsLua/gost.build.txt -o -r
gost.build.exe ../source/gtsLua/gost.build.txt -o

pause