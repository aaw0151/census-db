/*
	CSCE 2110.002: Project 3:
	Group 19
	Major Component 1 - Organizing Initial Data Storage
	File Name: organization.cpp
	Author: Alexander Williams
*/


#include "main.h"
#include "geo.h"
#include "age.h"
#include "dis.h"
#include "organization.h"

void OpenInputFile(string &filename, ifstream &in_stream, ofstream& out_stream)
{
	//OPENING INPUT FILE
	cout << "Enter the input file's name: ";
        cin  >> filename;

        in_stream.open(filename.c_str());


	//CHECKING IF FILE OPENED CORRECTLY
        while(in_stream.fail())
        {
                cout << "Input file \"" << filename << "\" does not exist. Enter input file's name (or -1 to close the program): ";
                cin  >> filename;
                if(filename == "-1")
                {
                        exit(EXIT_FAILURE);
                }
                in_stream.open(filename.c_str());
        }
	string outputname = "output" + filename;
	out_stream.open(outputname.c_str());
	out_stream << "Please enter the mainfile's name:" << filename << endl;
}

void ReadInputFile(string& filename, ifstream& in_stream, ofstream& out_stream, GeoTable& geo, AgeTable& age, DisTable& dis, vector<string>& query)
{
	//OPENING INPUT FILE
	OpenInputFile(filename, in_stream, out_stream);


	//READING INPUT FILE AND OUTPUT TABLE INFORMATION
	string current, tablename;

	for(int i = 0; i < 3; i++) //setting filenames for each table
	{
		in_stream >> tablename;
		in_stream >> current;

		if(current == "age")
		{
			age.SetFilename(tablename);
			age.SetName(current);
			out_stream << age.GetFilename() << " " << age.GetName() << endl;
			out_stream << "Processing " << age.GetName() << " table." << endl;
			ReadDataFile(age, out_stream);
		}
		else if (current == "geography")
		{
			geo.SetFilename(tablename);
			geo.SetName(current);
			out_stream << geo.GetFilename() << " " << geo.GetName() << endl;
			out_stream << "Processing " << geo.GetName() << " table." << endl;
			ReadDataFile(geo, out_stream);
		}
		else
		{
			dis.SetFilename(tablename);
			dis.SetName(current);
			out_stream << dis.GetFilename() << " " << dis.GetName() << endl;
			out_stream << "Processing " << dis.GetName() << " table." << endl;
			ReadDataFile(dis, out_stream);
		}
	}


	//ADDING INSTRUCTIONS TO QUERY
	while(!in_stream.eof())
	{
		getline(in_stream, current);
		if((current != "\0") && (current != "\n"))
		{
			query.push_back(current);
		}	
	}


	//CLOSING INPUT FILE
	in_stream.close();

	
	//BREAK UP DATA IN TABLES (using regex)
	CreateTuples(out_stream, geo, age, dis);
}

template<class TableType> //using templates to refrain from using three different functions
void ReadDataFile(TableType& table, ofstream& out_stream)
{
	//OPENING DATA FLE
	ifstream in_stream;
	string current;

	in_stream.open((table.GetFilename()).c_str());


	//READING TABLE NAME AND SCHEME
	in_stream >> current;
	table.SetKeyName(current);
	in_stream >> current;
	table.SetScheme(current);
	in_stream.ignore();


	//READING TABLE DATA
	while(!in_stream.eof())
	{
		getline(in_stream, current);
		if(current != "\0")
		{
			table.AddOrigData(current);
		}
	}


	//REMOVING POSSIBLE DUPLICATES
	for(int i = 0; i < table.GetOrigDataSize(); i++)
	{
		for(int j = 0; j < table.GetOrigDataSize(); j++)
		{
			if(i != j)
			{
				if(table.GetOrigData(i) == table.GetOrigData(j))
				{
					table.RemoveOrigData(j);
					j--;
				}
			}
		}
	}


	//OUTPUTING TABLE INITIAL TABLE INFORMATION
	for(int i = 0; i < table.GetOrigDataSize(); i++)
	{
		out_stream << "Inserted (" << table.GetOrigData(i) << ") into "  << table.GetName() << endl;
	}

	
	//CLOSING DATA FILE
	in_stream.close();
}

void CreateTuples(ofstream& out_stream, GeoTable& geo, AgeTable& age, DisTable& dis)
{
	//NOTE: The insertions of tuples at this stage is just for initial data insertion. These tuples will hashed for the hash tables later and the associated linked list will be defined later as well.//

	//FIND TUPLES FOR GEO DATA
	smatch matcher;
	regex pattern("([0-9]+),(\".+\"),([a-zA-Z]+),([0-9]+),([0-9]+),([0-9]+),([0-9A-Za-z]+),([0-9]+)");
	//pattern for strings similar to: 123,"test, test",test,123,123,123,TEST123TEST,123
	for(int i = 0; i < geo.GetOrigDataSize(); i++)
	{
		string str = geo.GetOrigData(i);
		regex_search(str, matcher, pattern);
		geo.AddData(make_pair(stoi(matcher[1].str()), GeoData(matcher[2].str(), matcher[3].str(), stoi(matcher[4].str()), stoi(matcher[5].str()), stoi(matcher[6].str()), matcher[7].str(), stoi(matcher[8].str())))); //creating tuples
	}
	geo.ClearOrigData();

	
	//FIND TUPLES FOR AGE DATA
	regex pattern2("([0-9a-zA-Z]+),([0-9]+),([0-9]+),([0-9]+)");
	//pattern for strings similar to: TEST123TEST,123,123,123
	for(int i = 0; i < age.GetOrigDataSize(); i++)
	{
		string str = age.GetOrigData(i);
		regex_search(str, matcher, pattern2);
		age.AddData(make_pair(matcher[1].str(), AgeData(stoi(matcher[2].str()), stoi(matcher[3].str()), stoi(matcher[4].str())))); //creating tuples
	}
	age.ClearOrigData();


	//FIND TUPLES FOR DISABILITY DATA
	regex pattern3("(\".+\"),([0-9]+),([0-9]+),([0-9]+),([0-9]+),([0-9]+),([0-9]+)");
	//pattern for strings similar to: "test",123,123,123,123,123,123
	for(int i = 0; i < dis.GetOrigDataSize(); i++)
	{
		string str = dis.GetOrigData(i);
		regex_search(str, matcher, pattern3);
		dis.AddData(make_pair(matcher[1].str(), DisData(stoi(matcher[2].str()), stoi(matcher[3].str()), stoi(matcher[4].str()), stoi(matcher[5].str()), stoi(matcher[6].str()), stoi(matcher[7].str())))); //creating tuples
	}
	dis.ClearOrigData();
}
