mkdir build || true
g++ -o ./build/app.out ./*.cpp ./vendor/*.cpp ./utils/*.cpp ./objects/*.cpp -std=c++11 -lglfw3 -pthread -lGLEW -lGLU -lGL -lrt -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -ldl -g
