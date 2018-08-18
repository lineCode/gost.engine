cd ..
del bin32\base_demo.exe
del bin32\Demo_release.exe

mkdir bin32\plugins\
mkdir bin64\plugins\

cd tools

gost.build.exe ../demo/examples/gost.build.txt -r -o
gost.build.exe ../demo/examples/gost.build.txt -o
gost.build.exe ../demo/gost.build.txt -r -o
gost.build.exe ../demo/gost.build.txt -o

pause