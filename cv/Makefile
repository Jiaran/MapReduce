


main: cv.o main.o
	g++ main.o cv.o -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -o main
main.o : main.cpp
	g++ -g -c main.cpp 
cv.o: cv.cpp
	g++ -g -c cv.cpp
