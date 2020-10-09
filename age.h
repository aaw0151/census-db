/*
    CSCE 2110.002: Project 3:
    Group 19
    Major Component 3 - Age Data
    File Name: age.h
    Author: Jian Ma
*/

#include <fstream>
#include <regex>

using namespace std;

class AgeData
{
    public:
        //Constructors:
        AgeData() {under5 = -1; under18 = -1; over65 = -1;}
        AgeData(int under5, int under18, int over65) {this->under5 = under5; this->under18 = under18; this->over65 = over65;}
        //Gets:
        int GetUnder5() {return under5;}
        int GetUnder18() {return under18;}
        int GetOver65() {return over65;}
        bool   operator==(const AgeData& source) const //used in deletion query
        {
            if((source.under5 == this->under5) &&
               (source.under18 == this->under18) &&
               (source.over65 == this->over65))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    private:
        int under5, under18, over65;        //integer for under5, under18, over65
};

struct AgeNode //struct for linked list node
{
    pair<string, AgeData> *data;
    AgeNode *next;
};

class AgeList //class for linked list (secondary index)
{
public:
    //Constructors:
    AgeList() {head = NULL; tail = NULL;}
    //Functions:
    void AddNode(pair<string, AgeData> *item);
    void SelectNodes(string, ofstream&);
    void DeleteNodes(string, ofstream&);
    void ResetAll();
    void Display(string, ofstream&);
    void Write(string, string, string);
private:
    AgeNode *head, *tail;
};

class AgeTable
{
    public:
        //constructors:
        AgeTable() {origdata.clear(); data.clear(); filename = "none"; name = "none"; scheme = "none";}
        //Hashing functions:
        void   HashAll();                            //hash all entries in table
        bool   IncreaseSize();                       //check if size must be doubled, size is doubled if needed
        void   Tombstone(pair<string, AgeData>&);    //tombstone current pair
        int    MidSquareHashing(string, int);        //hashing function (mid-square hashing)
        int    DoubleHashing(string, int);           //double hashing collision strategies
        void   CreateList(AgeList&);                 //create initial linked list
        //functions:
        void   Insert(pair<string, AgeData>, AgeList&, ofstream&);
        void   Update(pair<string, AgeData>, AgeList&, ofstream&);
        void   Select(smatch, string, AgeList, ofstream&);
        void   Delete(smatch, string, AgeList, ofstream&);
        //name, scheme, and filename:
        void   SetFilename(string filename) {this->filename = filename;}
        string GetFilename() {return filename;}
        void   SetName(string name) {this->name = name;}
        string GetName() {return name;}
        void   SetKeyName(string key) {keyname = key;}
        string GetKeyName() {return keyname;}
        void   SetScheme(string scheme) {this->scheme = scheme;}
        string GetScheme() {return scheme;}
        //data (tuples):
        void   AddData(pair<string, AgeData> curpair) {data.push_back(curpair);}    //adding tuple to vector
        pair<string, AgeData> GetData(int pos) {return data.at(pos);}               //getting pair from vector
        int    GetDataSize() {return data.size();}                                  //getting size of vector
        //original data:
        void   AddOrigData(string str) {origdata.push_back(str);}                   //add original string entry
        string GetOrigData(int pos) {return origdata.at(pos);}                      //get original string entry
        void   RemoveOrigData(int pos) {origdata.erase(origdata.begin() + pos);}    //remove original string entry
        int    GetOrigDataSize() {return origdata.size();}                          //get size of original string vector
        void   ClearOrigData() {origdata.clear();}                                  //clear original string vector
    private:
        vector<string> origdata;         //vector of strings for origininal data
        vector<pair<string, AgeData>> data; //vector of pairs of ints and AgeData objects for hash table
        string filename; //string for file name of table file
        string name;     //string for name of table
        string keyname;  //string for key
        string scheme;   //string for scheme
};

