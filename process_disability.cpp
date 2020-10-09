/*
	CSCE 2110.002: Project 3:
	Group 19
	Major Component 2 - Creating Disability Table
	File Name: process_disability.cpp
	Author: Cole Hochstrate
*/

#include "main.h"
#include "dis.h"
#include "process_disability.h"

// this funcation handels the calling of insert select delete and update using regex matching
void processDisability(ofstream& out_stream, DisTable& dis, DisList& dislist, string query)
{
	
	
	//INCREASING SIZE AND HASHING ALL DATA
	if(dis.IncreaseSize())
	{
		dis.HashAll();                  // reshash table
		dislist.ResetAll();             // reset secondary index
		dis.CreateList(dislist);        // recreate secondary index
	}

    // each on of the blacks will use regex to deciver what function (insert, update, select, and delete) to call

	//INSERT QUERIES
	regex insertpat("INSERT\\(\\((\".*\"|.*),(.*),(.*),(.*),(.*),(.*),(.*)\\),disability\\)");
	if(regex_match(query, insertpat))
	{
        smatch matcher;
		regex_search(query, matcher, insertpat);
		dis.Insert(make_pair(matcher[1].str(), DisData(stoi(matcher[2].str()), stoi(matcher[3].str()), stoi(matcher[4].str()), stoi(matcher[5].str()), stoi(matcher[6].str()), stoi(matcher[7].str()))), dislist, out_stream);
		return;
	}


	//UPDATE QUERIES
	regex updatepat("UPDATE\\(\\((\".*\"|.*),(.*),(.*),(.*),(.*),(.*),(.*)\\),disability\\)");
	if(regex_match(query, updatepat))
	{
        smatch matcher;
		regex_search(query, matcher, updatepat);
		dis.Update(make_pair(matcher[1].str(), DisData(stoi(matcher[2].str()), stoi(matcher[3].str()), stoi(matcher[4].str()), stoi(matcher[5].str()), stoi(matcher[6].str()), stoi(matcher[7].str()))), dislist, out_stream);
		return;
	}

	//SELECT QUERIES
	regex selectpat("SELECT\\(\\((\".*\"|.*),(.*),(.*),(.*),(.*),(.*),(.*)\\),disability\\)");
	if(regex_match(query, selectpat))
	{
        smatch matcher;
		regex_search(query, matcher, selectpat);
		dis.Select(matcher, query, dislist, out_stream);
		return;
	}


	//DELETE QUERIES
	regex deletepat("DELETE\\(\\((\".*\"|.*),(.*),(.*),(.*),(.*),(.*),(.*)\\),disability\\)");
	if(regex_match(query, deletepat))
	{
        smatch matcher;
		regex_search(query, matcher, deletepat);
		dis.Delete(matcher, query, dislist, out_stream, dis);
		return;
	}
}