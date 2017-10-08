// // #pragma warning(disable:4996)
// #include <stdio.h>
// #include <iostream>
// #include <string>
// #include <fstream>
// #include <sstream>
// #include <vector>
// #include <math.h>
// #include <time.h>       /* time */
//
// using namespace std;
//
// // Build:
// // nodemon --exec "g++ -o ./build/tttt.out ./tttt.cpp -std=c++11 -lglfw3 -pthread -lGLEW -lGLU -lGL -lrt -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -ldl && ./build/tttt.out"  -e cpp
//
// void readObjectFile(string filename) {
// 	std::vector<float> pointsVector = {};
//     std::vector<int> indexVector = {};
//
// 	ifstream openedFile(filename.c_str());
// 	if (openedFile.is_open()) {
// 		string line;
//         string strNumber;
//
//     	while(!openedFile.eof()) {
//     		openedFile >> line;
//
// 			if(line.compare("v") == 0) {
//                 openedFile >> strNumber;
//                 pointsVector.push_back( stof(strNumber) );
//                 openedFile >> strNumber;
//                 pointsVector.push_back( stof(strNumber) );
//                 openedFile >> strNumber;
//                 pointsVector.push_back( stof(strNumber) );
// 			} else if(line.compare("f") == 0) {
//
//                 int count = 3;
//                 for (size_t i = 0; i < count; i++) {
//                     openedFile >> strNumber;
//                     int doubleIdx = strNumber.find("//");
//                     if(doubleIdx > 0) {
//                         string fstNumber = strNumber.substr(0, doubleIdx);
//                         string secNumber = strNumber.substr(doubleIdx + 2);
//                         indexVector.push_back( stof(fstNumber) );
//                     } else {
//                         int singleIdx = strNumber.find("/");
//                         if(singleIdx > 0) {
//                             string fstNumber = strNumber.substr(0, singleIdx);
//                             string nextString = strNumber.substr(singleIdx + 1);
//                             int nextIndex = nextString.find("/");
//                             string secNumber = nextString.substr(0, nextIndex);
//                             string trdNumber = nextString.substr(nextIndex + 1);
//
//                             indexVector.push_back( stof(fstNumber) );
//                         } else {
//                             indexVector.push_back( stof(strNumber) );
//                         }
//                     }
//                 }
//
// 			}
//         }
// 		openedFile.close();
// 	}
//
//     // for (size_t i = 0; i < pointsVector.size(); i++) {
//     //     std::cout << pointsVector.at(i) << '\n';
//     // }
//     //
//     // for (size_t i = 0; i < indexVector.size(); i++) {
//     //     std::cout << indexVector.at(i) << '\n';
//     // }
// }
//
// int main() {
//     readObjectFile("./Objects/cube_t.obj");
// }
