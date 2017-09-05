git clone https://github.com/glfw/glfw.git glfw_repository
cd glfw_repository
cmake -G "Unix Makefiles"
make
make install
# apt-get install libglfw3-dev
sudo apt-get install libglew-dev

echo ""
echo ""
echo "To compile use:"
echo "g++ main.cpp -std=c++11 -lglfw3 -pthread -lGLEW -lGLU -lGL -lrt -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -ldl"
echo ""
echo ""
