/*
	CSCE 2110.002: Project 3:
	Group 19
	Major Component 4 - Disability Data
	File Name: dis.h
	Author:
*/

// class represents a tuple in the hash table
class DisData
{
    public:
		// constructors
		DisData() {hearing = -1; vision = -1; cognitive = -1; ambulatory = -1; selfCare = -1; independentLiving = -1;}
		DisData(int h, int v, int c, int a, int s, int i) {hearing = h; vision = v; cognitive = c; ambulatory = a; selfCare = s; independentLiving = i;}
		// getters each statement return the corresponding field in the tuple
		int     getHearing() {return hearing;}
		int     getVision() {return vision;}
		int     getCognitive() {return cognitive;}
		int     getAmbulatory() {return ambulatory;}
		int     getSelfCare() {return selfCare;}
		int     getIndependentLiving() {return independentLiving;}
		bool   operator!=(const DisData& source) const //used in deletion query
		{
			if((source.hearing == this->hearing) && 
			   (source.vision == this->vision) && 
			   (source.cognitive == this->cognitive) && 
			   (source.ambulatory == this->ambulatory) &&
			   (source.selfCare == this->selfCare) &&
			   (source.independentLiving == this->independentLiving))
			{
				return false;
			}
			else
			{
				return true;
			}
		}
    private:
		int hearing, vision, cognitive, ambulatory, selfCare, independentLiving;
};

// this the a node of secondary index (linked list stucture)
struct DisNode
{
	pair<string, DisData> *data;
	DisNode *next;
};

// the class refers to the secondary index
class DisList
{
	public:
		//Constructors:
		DisList() {head = NULL; tail = NULL;}
		// functions
		void Display(string, ofstream&);                // display the list
		void AddNode(pair<string, DisData> *item);      // add nodes                
		void DeleteNodes(string, ofstream&);            // deletes noddes
		void SelectNodes(string, ofstream&);            // selects nodes         
		void Write(string, string, string);             // write to list
		void ResetAll();                                // clears secondary index
	private:
		DisNode *head, *tail;
};

// the hash table
class DisTable
{
    public:
		//constructors:
		DisTable() {origdata.clear(); data.clear(); filename = "none"; name = "none"; scheme = "none";}
		//functions:
		void   CreateList(DisList&);            																			// creates table's secondary index
		// hashing
		void   	HashAll();                       																			//hash all entries in table
		int		multiplicativeHash(string key, int tableSize);                                                              // multiplicative string hashing
		int		quadraticProbing(int hashedKey, int tableSize, int i);                                                      // quadratic probing strategy
		bool   	IncreaseSize();                  																			// check if size must be doubled, size is doubled if needed
		void    Tombstone(pair<string, DisData>&);                                                                          // tombstone record
		// operations
		void    Insert(pair<string, DisData>, DisList&, ofstream&);                                                         // insert into 
		void    Update(pair<string, DisData>, DisList&, ofstream&);                                                         // update record 
		void    Select(smatch, string, DisList&, ofstream&);                                                                // select record(s)
		void    Delete(smatch, string, DisList&, ofstream&, DisTable&);                                                                // delete records(s)
		void    Display(ofstream&);                                                                                         // displays table
		void    Write(ofstream&);                                                                                           // write current state of table to file
		// name, scheme, and filename:
		void    SetFilename(string filename) {this->filename = filename;}
		string  GetFilename() {return filename;}
		void    SetName(string name) {this->name = name;}
		string  GetName() {return name;}
		void    SetKeyName(string key) {keyname = key;}
		string  GetKeyName() {return keyname;}
		void    SetScheme(string scheme) {this->scheme = scheme;}
		string  GetScheme() {return scheme;}
		//data (tuples):
		void                    AddData(pair<string, DisData> curpair) {data.push_back(curpair);}
		pair<string, DisData>   GetData(int pos) {return data.at(pos);}
		int                     GetDataSize() {return data.size();}
		//original data:
		void    AddOrigData(string str) {origdata.push_back(str);}
		string  GetOrigData(int pos) {return origdata.at(pos);}
		void    RemoveOrigData(int pos) {origdata.erase(origdata.begin() + pos);}
		int     GetOrigDataSize() {return origdata.size();}
		void    ClearOrigData() {origdata.clear();}                               											//clear original string vector
    private:
		vector<string> origdata;
		vector<pair<string, DisData>> data;																					// this is the primary index
		string filename;
		string name;
		string keyname;
		string scheme;
};



