Project 3: Emergancy Storage
Group 19
Alexander Williams, Jian Ma, Cole Hochstrate

Hashing Algorithms and Collision Strategies:
Alexander Williams (Geography Table) - Modulo Hashing and Linear Probing
Jian Ma (Age Table) - 
Cole Hochstrate (Disability Table) - 

Our project uses a makefile to compile. Place the following files into a folder:
age.h
age.cpp
dis.h
dis.cpp
geo.h
geo.cpp
processage.h
processage.cpp
process_disability.h
process_disability.cpp
processgeo.h
processgeo.cpp
organization.h
organization.cpp
main.h
main.cpp
makefile
(all input files and .csv files)

Type "make" into the terminal to create the executable "storage.exe". Type "make clean" to remove all "*.o" and "storage.exe" files. Type "./storage.exe" into the terminal to execute the program. All input file names are inputted during runtime.

**All output is sent to an output file. If the input file is named "input4.txt", the output file will be named "outputinput4.txt".**

WRITE() queries are sent to output files. If the input file is named "input5.txt", the files made will be named "geoinput5.txt.out", "ageinput5.txt.out", and "disinput5.txt.out". Every WRITE() query will append another ".out" to the end of the output table file's name. If you have 3 WRITE() queries (with input file named "input.txt"), the final table output names will be "geoinput.txt.out.out.out", "ageinput.txt.out.out.out", and "disinput.txt.out.out.out".
