all:
	g++ -I eigen/ *.cpp -O3 -o Raytracer -std=c++11