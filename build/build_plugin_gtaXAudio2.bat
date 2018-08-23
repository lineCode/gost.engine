cd ..
del bin32\plugins\gtaXAudio2.gpl

mkdir bin32\plugins\
mkdir bin64\plugins\

cd tools
gost.build.exe ../source/gtaXAudio2/gost.build.txt -o -r
gost.build.exe ../source/gtaXAudio2/gost.build.txt -o

pause