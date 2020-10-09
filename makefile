storage.exe : main.o geo.o age.o dis.o organization.o processgeo.o process_disability.o processage.o
	g++ main.o geo.o age.o dis.o organization.o processgeo.o  processage.o process_disability.o -o storage.exe

main.o : main.cpp
	g++ -c main.cpp

geo.o : geo.cpp
	g++ -c geo.cpp

age.o : age.cpp
	g++ -c age.cpp

dis.o : dis.cpp
	g++ -c dis.cpp

organization.o : organization.cpp
	g++ -c organization.cpp

processgeo.o : processgeo.cpp
	g++ -c processgeo.cpp

processage.o : processage.cpp
	g++ -c processage.cpp

process_disability.o : process_disability.cpp
	g++ -c process_disability.cpp

clean :
	rm *.o storage.exe
