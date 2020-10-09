/*
	CSCE 2110.002: Project 3:
	Group 19
	Major Component 2 - Geography Data
	File Name: geo.h
	Author: Alexander Williams
*/

class GeoData //class for geo table entry
{
	public:
		//Constructors:
		GeoData() {name = "none"; state = "none"; geoid = "none"; sumlevel = -1; statenum = -1; county = -1; population = -1;}
		GeoData(string n, string s, int sl, int sn, int c, string g, int p) {name = n; state = s; sumlevel = sl; statenum = sn; county = c; geoid = g; population = p;}
		//Gets:
		string GetName() {return name;}
		string GetState() {return state;}
		string GetGeoid() {return geoid;}
		int    GetSumlevel() {return sumlevel;}
		int    GetStatenum() {return statenum;}
		int    GetCounty() {return county;}
		int    GetPopulation() {return population;}
		//Operator Overloads:
		bool   operator==(const GeoData& source) const //used in deletion query
		{
			if((source.name == this->name) && 
			   (source.state == this->state) && 
			   (source.geoid == this->geoid) && 
			   (source.sumlevel == this->sumlevel) &&
			   (source.statenum == this->statenum) &&
			   (source.county == this->county) &&
			   (source.population == this->population))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool   operator!=(const GeoData& source) const //used in deletion query
		{
			if((source.name == this->name) && 
			   (source.state == this->state) && 
			   (source.geoid == this->geoid) && 
			   (source.sumlevel == this->sumlevel) &&
			   (source.statenum == this->statenum) &&
			   (source.county == this->county) &&
			   (source.population == this->population))
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	private:
		string name, state, geoid;                  //string for name, state, and geo id
		int sumlevel, statenum, county, population; //integer for sumlevel, state number, county number, and population
};

struct GeoNode //struct for linked list node
{
	pair<int, GeoData> *data;
	GeoNode *next;
};

class GeoList //class for linked list (secondary index)
{
	public:
		//Constructors:
		GeoList() {head = NULL; tail = NULL;}
		//Functions:
		void AddNode(pair<int, GeoData> *item);
		void SelectNodes(string, ofstream&);
		void DeleteNodes(string, ofstream&);
		void ResetAll();
		void Display(string, ofstream&);
		void Write(string, string, string);
	private:
		GeoNode *head, *tail;
};

class GeoTable
{
	public:
		//Constructors:
		GeoTable() {origdata.clear(); data.clear(); filename = "none"; name = "none"; scheme = "none";}
		//Hashing Functions:
		void   HashAll();                       //hash all entries in table
		bool   IncreaseSize();                  //check if size must be doubled, size is doubled if needed
		void   Tombstone(pair<int, GeoData>&); //tombstone current pair
		int    HashFunc(int, int);              //hashing function (modulo hashing)
		void   CreateList(GeoList&);            //create initial linked list
		//Query Functions:
		void   Insert(pair<int, GeoData>, GeoList&, ofstream&); //geography insert queries
		void   Update(pair<int, GeoData>, GeoList&, ofstream&); //geography update queries
		void   Select(smatch, string, GeoList, ofstream&);      //geography select queries
		void   Delete(smatch, string, GeoList, ofstream&);      //geography delete queries
		//Name, Scheme, and Filename:
		void   SetName(string name) {this->name = name;}
		string GetName() {return name;}
		void   SetKeyName(string key) {keyname = key;}
		string GetKeyName() {return keyname;}
		void   SetScheme(string scheme) {this->scheme = scheme;}
		string GetScheme() {return scheme;}
		void   SetFilename(string filename) {this->filename = filename;}
		string GetFilename() {return filename;}
		//Data Entries (used for initialization):
		void   AddData(pair<int, GeoData> curpair) {data.push_back(curpair);} //adding tuple to vector
		pair<int, GeoData> GetData(int pos) {return data.at(pos);}            //getting pair from vector
		int    GetDataSize() {return data.size();}                            //getting size of vector
		//Original Data (used for initialization):
		void   AddOrigData(string str) {origdata.push_back(str);}                //add original string entry
		string GetOrigData(int pos) {return origdata.at(pos);}                   //get original string entry
		void   RemoveOrigData(int pos) {origdata.erase(origdata.begin() + pos);} //remove original string entry
		int    GetOrigDataSize() {return origdata.size();}                       //get size of original string vector
		void   ClearOrigData() {origdata.clear();}                               //clear original string vector
	private:
		vector<string> origdata;         //vector of strings for origininal data
		vector<pair<int, GeoData>> data; //vector of pairs of ints and GeoData objects for hash table
		string filename; //string for file name of table file
		string name;     //string for name of table
		string keyname;  //string for key
		string scheme;   //string for scheme
};