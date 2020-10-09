/*
	CSCE 2110.002: Project 3:
	Group 19
	Major Component 1 - Organizing Initial Data Storage
	File Name: organization.h
	Author: Alexander Williams
*/

void OpenInputFile(string&, ifstream&, ofstream&, int argc, char* argv[]); //open input file function
void ReadInputFile(string&, ifstream&, ofstream&, GeoTable&, AgeTable&, DisTable&, vector<string>&); //read input file function
template<class TableType> //template to prevent 3 seperate functions for reading data file
void ReadDataFile(TableType&, ofstream&); //read data file function (table file)
void CreateTuples(ofstream&, GeoTable&, AgeTable&, DisTable&); //create tuples (entries) function
