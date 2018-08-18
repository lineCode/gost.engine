cd ..
del bin32\plugins\gtrD3D11.gpl

mkdir bin32\plugins\
mkdir bin64\plugins\

cd tools
gost.build.exe ../source/gtrD3D11/gost.build.txt -o -r
gost.build.exe ../source/gtrD3D11/gost.build.txt -o

pause