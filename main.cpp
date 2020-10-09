/*
	CSCE 2110.002: Project 3:
	Group 19
	File Name: main.cpp
*/


#include "main.h"
#include "geo.h"
#include "age.h"
#include "dis.h"
#include "organization.h"
#include "processgeo.h"
#include "processage.h"
#include "process_disability.h"

int main()
{
	GeoTable geo;         //geography data table
	GeoList  geolist;     //geography linked list
	AgeTable age;         //age data table
	AgeList  agelist;     //age linked list
	DisTable dis;         //disability data table
	DisList  dislist;     //disability linked list
	vector<string> query; //vector for query instructions
	string filename;      //string for input file name
	ifstream in_stream;   //input file stream for input file
	ofstream out_stream;  //output file stream for output file

	//READING AND CHECKING INPUT FILE, READING AND CHECKING TABLE FILES, AND READING QUERY (Part 1), populating tables
	ReadInputFile(filename, in_stream, out_stream, geo, age, dis, query);
	cout << "Output sent to \"output" << filename << "\"\n";

	//CREATING INITIAL LINKED LISTS creates the secondary index
	geo.CreateList(geolist);
	age.CreateList(agelist);
	dis.CreateList(dislist);
	
	//PROCESSING QUERIES
	for(unsigned int i = 0; i < query.size(); i++)
	{
		string str = query[i];
		regex geopattern(".*\\),geography.*");
		regex agepattern(".*\\),age.*");
		regex dispattern(".*\\),disability.*");
		regex displaypattern("DISPLAY\\(\\)");
		regex writepattern("WRITE\\(\\)");

		if(regex_match(str, geopattern))
		{
			//PROCESS GEOGRAPHY DATA (Part 2)
			ProcessGeoData(out_stream, geo, geolist, query[i]);
		}
		else if(regex_match(str, agepattern))
		{
			//PROCESS AGE DATA (Part 3)
			ProcessAge(out_stream, age, agelist, query[i]);
		}
		else if(regex_match(str, dispattern))
		{
			//PROCESS DISABILITY DATA (Part 4)
			processDisability(out_stream, dis, dislist, query[i]);
		}
		else if(regex_match(str, displaypattern))
		{
			out_stream << "DISPLAY\n";
			geolist.Display(geo.GetScheme(), out_stream);
			out_stream << endl;
			agelist.Display(age.GetScheme(), out_stream);
			out_stream << endl;
			dislist.Display(dis.GetScheme(), out_stream);
			out_stream << endl;
		}
		else if(regex_match(str, writepattern))
		{
			out_stream << "WRITE\n";
			filename += ".out";
			geolist.Write(geo.GetKeyName(), geo.GetScheme(), filename);
			agelist.Write(age.GetKeyName(), age.GetScheme(), filename);
			dislist.Write(dis.GetKeyName(), dis.GetScheme(), filename);
		}
	}

	out_stream << "Processing complete!\n";
	query.clear();
	geolist.ResetAll();
	agelist.ResetAll();
	out_stream.close();

	return 0;
}
