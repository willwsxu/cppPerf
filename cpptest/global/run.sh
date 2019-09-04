g++ -std=c++14 -c -fpic static.cpp
ar rcs libstatic.a static.o
g++ -std=c++14 -c -fpic dynamic.cpp
g++ -shared -o libdynamic.so dynamic.o -L./ -lstatic
g++ -std=c++14 -c main.cpp
g++ -L./ -Wl,-rpath=./ main.o -lstatic -ldynamic