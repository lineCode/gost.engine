cd ..
del bin32\plugins\gtiDirectInput.gpl

mkdir bin32\plugins\
mkdir bin64\plugins\

cd tools
gost.build.exe ../source/gtiDirectInput/gost.build.txt -o -r
gost.build.exe ../source/gtiDirectInput/gost.build.txt -o

pause