cd ..
del bin32\gost.dll

mkdir bin32\plugins\
mkdir bin64\plugins\

cd tools
gost.build.exe ../source/gost/gost.build.txt -o -r
gost.build.exe ../source/gost/gost.build.txt -o

pause