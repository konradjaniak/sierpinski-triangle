c: trojkaty_c11threads.cpp
	g++ trojkaty_c11threads.cpp -o trojkaty_c11threads.exe -O3 -lpthread

r:
	make c && ./trojkaty_c11threads && eog grafika.ppm