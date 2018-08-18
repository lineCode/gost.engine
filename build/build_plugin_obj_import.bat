cd ..
del bin32\plugins\gtimOBJ.gpl

mkdir bin32\plugins\
mkdir bin64\plugins\

cd tools
gost.build.exe ../source/gtimOBJ/gost.build.txt -o -r
gost.build.exe ../source/gtimOBJ/gost.build.txt -o

pause