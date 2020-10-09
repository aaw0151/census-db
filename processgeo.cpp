/*
	CSCE 2110.002: Project 3:
	Group 19
	Major Component 2 - Creating Geography Table and Developing Related Functions
	File Name: processgeo.cpp
	Author: Alexander Williams
*/

#include "main.h"
#include "geo.h"
#include "processgeo.h"

void ProcessGeoData(ofstream& out_stream, GeoTable& geo, GeoList& geolist, string query)
{
	//INCREASING SIZE AND HASHING ALL DATA
	if(geo.IncreaseSize())
	{
		geo.HashAll();
		
		
		//INITIALIZING LINKED LIST
		geolist.ResetAll();
		geo.CreateList(geolist);
	}


	//INSERT QUERIES
	regex insertpat("INSERT\\(\\(([0-9]+),(\".+\"),([a-z]+),([0-9]+),([0-9]+),([0-9]+),([A-Z0-9]+),([0-9]+)\\),geography\\)");
	if(regex_match(query, insertpat))
	{
		smatch matcher;
		regex_search(query, matcher, insertpat);
		geo.Insert(make_pair(stoi(matcher[1].str()), GeoData(matcher[2].str(), matcher[3].str(), stoi(matcher[4].str()), stoi(matcher[5].str()), stoi(matcher[6].str()), matcher[7].str(), stoi(matcher[8].str()))), geolist, out_stream);
		return;
	}


	//UPDATE QUERIES
	regex updatepat("UPDATE\\(\\(([0-9]+),(\".*\"),([a-z]+),([0-9]+),([0-9]+),([0-9]+),([0-9A-Z]+),([0-9]+)\\),geography\\)");
	if(regex_match(query, updatepat))
	{
		smatch matcher;
		regex_search(query, matcher, updatepat);
		geo.Update(make_pair(stoi(matcher[1].str()), GeoData(matcher[2].str(), matcher[3].str(), stoi(matcher[4].str()), stoi(matcher[5].str()), stoi(matcher[6].str()), matcher[7].str(), stoi(matcher[8].str()))), geolist, out_stream);
		return;
	}


	//SELECT QUERIES
	regex selectpat("SELECT\\(\\((.*),(\".*\"|.*),(.*),(.*),(.*),(.*),(.*),(.*)\\),geography\\)");
	if(regex_match(query, selectpat))
	{
		smatch matcher;
		regex_search(query, matcher, selectpat);
		geo.Select(matcher, query, geolist, out_stream);
		return;
	}


	//DELETE QUERIES
	regex deletepat("DELETE\\(\\((.*),(\".*\"|\\*),(.*),(.*),(.*),(.*),(.*),(.*)\\),geography\\)");
	if(regex_match(query, deletepat))
	{
		smatch matcher;
		regex_search(query, matcher, deletepat);
		geo.Delete(matcher, query, geolist, out_stream);
		return;
	}
}