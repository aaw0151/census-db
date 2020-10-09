/*
    CSCE 2110.002: Project 3:
    Group 19
    Major Component 3 - Creating Age Table
    File Name: processage.cpp
    Author: Jian Ma
*/

#include "main.h"
#include "age.h"
#include "processage.h"

void ProcessAge(ofstream& out_stream, AgeTable& age, AgeList& agelist, string query)
{
    //INCREASING SIZE AND HASHING ALL DATA
    if(age.IncreaseSize())
    {
        
        age.HashAll();
        
        //INITIALIZING LINKED LIST
        agelist.ResetAll();
        age.CreateList(agelist);
    }

    //INSERT QUERIES
    regex insertpat("INSERT\\(\\((\".*\"|.*),(.*),(.*),(.*)\\),age\\)");
    if(regex_match(query, insertpat))
    {
        smatch matcher;
        regex_search(query, matcher, insertpat);
        age.Insert(make_pair(matcher[1].str(), AgeData(stoi(matcher[2].str()), stoi(matcher[3].str()), stoi(matcher[4].str()))), agelist, out_stream);
        return;
    }

    //UPDATE QUERIES
    regex updatepat("UPDATE\\(\\((\".*\"|.*),(.*),(.*),(.*)\\),age\\)");
    if(regex_match(query, updatepat))
    {
        smatch matcher;
        regex_search(query, matcher, updatepat);
        age.Update(make_pair(matcher[1].str(), AgeData(stoi(matcher[2].str()), stoi(matcher[3].str()), stoi(matcher[4].str()))), agelist, out_stream);
        return;
    }

    //SELECT QUERIES
    regex selectpat("SELECT\\(\\((\".*\"|.*),(.*),(.*),(.*)\\),age\\)");
    if(regex_match(query, selectpat))
    {
        smatch matcher;
        regex_search(query, matcher, selectpat);
        age.Select(matcher, query, agelist, out_stream);
        return;
    }

    //DELETE QUERIES
    regex deletepat("DELETE\\(\\((\".*\"|.*),(.*),(.*),(.*)\\),age\\)");
    if(regex_match(query, deletepat))
    {
        smatch matcher;
        regex_search(query, matcher, deletepat);
        age.Delete(matcher, query, agelist, out_stream);
        return;
    }
}

