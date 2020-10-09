/*
	CSCE 2110.002: Project 3:
	Group 19
	Major Component 4 - Disability Data
	File Name: dis.cpp
	Author: Cole Hochstrate
*/
#include "main.h"
#include "dis.h"

// MULTIPLICATIVE STRING HASHING FUNCTION
int DisTable::multiplicativeHash(string key, int tableSize)
{
	unsigned long long pos = 0;                                         // using and unsigned long long because this postion nubmer is multipled by itself many times
	int secretPrime = 2;                                                // constant secret prime number
	for(int i = 0; i < key.length(); i++)                               // for each character in the sting
		pos = (pos * secretPrime) + int(key[i]);                        // pos is equal to itself times secret prime + ascii value of current iteration of key
	
    return pos % tableSize; 
}

// QUADRATIC PROBING
int DisTable::quadraticProbing(int hashedKey, int tableSize, int i)
{
	int val = hashedKey + pow(i, 2);                                    // hashed key * i (ie number of attemped probes) squared                             
	return val % tableSize;                                             
} 

// CREATE LINKED LIST
void DisTable::CreateList(DisList& disList)
{
	//CREATE INITIAL LINKED LIST
	for(int i = 0; i < data.size(); i++)
	{
		pair<string, DisData>* ptr = &(data[i]);						// pointing to memory location of the primary index in hash table						
		disList.AddNode(ptr);                                           // add node to linked list
	}
}

// DISABILITY LINKED LIST FUNCTIONS
void DisList::AddNode(pair<string, DisData> *item)
{
	//ADD NODE TO LINKED LIST
	DisNode *temp = new DisNode;                                        // assign an empty node
	temp->data = item;                                                  // points to the passed item
	temp->next = NULL;                                                  // next on the linked list is poiting to null
	if(head == NULL)                                                    // checking for null at the head of the linked list
	{
		head = temp;
		tail = temp;
		temp = NULL;
	}
	else
	{
		tail->next = temp;
		tail = temp;
	}
}

// clears the secondary index
void DisList::ResetAll()
{
	//RESET ALL NODES (EMPTY LINKED LIST)
	DisNode* curr = new DisNode;
	DisNode* next;
	curr = head;
	while(curr != NULL)                                                 // iterating through the linked list
	{
		next = curr->next;
		curr = NULL;
		curr = next;		
	}
	delete curr;                                                        // delete the node
	head = NULL;                                                        // set head of linked list to null
	tail = NULL;                                                        // set tail of linked list to null
}

// this function will return true and increase the hash table size, if the hash table is more than 60% full
bool DisTable::IncreaseSize()
{

	float ctEmpty = 0.0;
	float ctOccupied = 0.0;

	for(int i = 0; i < data.size(); i++)
	{
		if(data[i].first.compare("-1") == 0)                                                    // if tuple is empty
			ctEmpty += 1;                                                                       // then increment the empty variable
		else
			ctOccupied += 1;
	}

	if(ctOccupied >= data.size() * 0.6)                                                        // if the table is greater than 60% full
	{
		// double the tablesize and return true
		int size = data.size();
		for( int i = 0; i < size; i++) 												
			data.push_back(make_pair("-1", DisData()));
		
		return true;                                                                            // reuturn as true (table was increased)
	}
	else
	{
		return false;                                                                           // return false (table does not need to be increased)
	}
}

// this function hashes all records in the disability table
void DisTable::HashAll()
{
	//CREATE TEMP VECTOR AND FILL OLD VECTOR WITH EMPTY BUCKETS
	vector<pair<string, DisData>> temp = data;
	data.clear();
	for(int i = 0; i < temp.size(); i++)
		data.push_back(make_pair("-1", DisData()));

	//HASH ALL NON 0 AND NON TombstoneD (-99) KEYS
	for(int i = 0; i < temp.size(); i++)
	{
		int numprobe = 0;
        if((temp[i].first.compare("-1") != 0) && (temp[i].first.compare("-99") != 0))
		{
			// QUADRATIC PROBING STRATEGY
            int index = quadraticProbing(multiplicativeHash(temp[i].first, data.size()), data.size(), numprobe);         // resetting index using quadratic probing
			numprobe ++;

			while((data[index].first.compare("-1") != 0) && (data[index].first.compare("-99") != 0))                    // find open bucket
			{
                // QUADRATIC PROBING STRATEGY
                index = quadraticProbing(multiplicativeHash(data[i].first, data.size()), data.size(), numprobe);         // resetting index using quadratic probing
                numprobe ++;                                                                                             // increment number of attempted inserts (ie probing)
			}
			
			//INSERT NEW PAIR AND "DELETE" OLD PAIR
			data.erase(data.begin() + index);
			data.insert(data.begin() + index, temp[i]);
			temp[i] = make_pair("-1", DisData());
		}
	}
}

// this function tomb stones passed record
void DisTable::Tombstone(pair<string, DisData>& pair1)
{
	// TOMBSTONE PAIR (VALUE THAT IMPLIES THAT THE PAIR HAS BEEN DELETED BY QUERY)
	pair1.first = "-99";
	pair1.second = DisData();
}

// ===========================
// DISPLAY AND WRITE FUNCTIONS
// ===========================

// function responsable for DISPLAY() call in the file
void DisList::Display(string scheme, ofstream& out_stream)
{
	DisNode * curr = new DisNode;
	curr = head;

	int cmpLength;
	string tupleItem;
	vector<string> schemes;
	vector<int> width;
	regex pattern("(.*),(.*),(.*),(.*),(.*),(.*),(.*)");
	smatch matcher;
	regex_search(scheme, matcher, pattern);
	
	// pushing comma delimited string scheme into schemes vector
	for(unsigned int i = 1; i < matcher.size(); i++)
		schemes.push_back(matcher[i].str());
	

	//FINDING WIDTHS FOR OUTPUTS
	for(int i = 0; i < schemes.size(); i ++)
		width.push_back(schemes[i].size());

	
	// walking the linked list
	while(curr != NULL)
	{
		if((curr->data->first.compare("-1") != 0) && (curr->data->first.compare("-99") != 0))               // if tuple is not tombstoned (-99) and not empty space (-1)
		{
			// THE BELOW BLOCK OF CODE IS JUST FOR GETTING MAX WIDTH OF EACH TABLE COLUMN
            cmpLength = curr->data->first.length();
			width[0] = (width[0] < cmpLength ? cmpLength : width[0]);
			tupleItem = curr->data->second.getHearing();
			cmpLength = tupleItem.length();
			width[1] = (width[1] < cmpLength ? cmpLength : width[1]);
			tupleItem = curr->data->second.getVision();
			cmpLength = tupleItem.length();
			width[2] = (width[2] < cmpLength ? cmpLength : width[2]);
			tupleItem = curr->data->second.getCognitive();
			cmpLength = tupleItem.length();
			width[3] = (width[3] < cmpLength ? cmpLength : width[3]);
			tupleItem = curr->data->second.getAmbulatory();
			cmpLength = tupleItem.length();
			width[4] = (width[4] < cmpLength ? cmpLength : width[4]);
			tupleItem = curr->data->second.getSelfCare();
			cmpLength = tupleItem.length();
			width[5] = (width[5] < cmpLength ? cmpLength : width[5]);
			tupleItem = curr->data->second.getIndependentLiving();
			cmpLength = tupleItem.length();
			width[6] = (width[6] < cmpLength ? cmpLength : width[6]);
		}
		curr = curr->next;
	}

	//OUTPUT HEADERS
	for(int i = 0; i < schemes.size(); i ++)
	{	
			out_stream.width(width[i] + 2);
			out_stream << std::left << schemes[i];
	}	
	out_stream << endl;

	curr = head;
	while(curr != NULL)
	{
		if((curr->data->first.compare("-1") != 0) && (curr->data->first.compare("-99") != 0))																		// if tuple is not tombstoned (-99) and not empty space (-1)
		{
			// THE BELOW BLOCK OF CODE IS FOR OUTPUTTING THE DATA TO FILE USING CORRECT COLUMN WIDTH
            out_stream.width(width[0] + 2);
			out_stream << std::left << curr->data->first;
			out_stream.width(width[1] + 2);
			out_stream << std::left << curr->data->second.getHearing();
			out_stream.width(width[2] + 2);
			out_stream << std::left << curr->data->second.getVision();
			out_stream.width(width[3] + 2);
			out_stream << std::left << curr->data->second.getCognitive();
			out_stream.width(width[4] + 2);
			out_stream << std::left << curr->data->second.getAmbulatory();
			out_stream.width(width[5] + 2);
			out_stream << std::left << curr->data->second.getSelfCare();
			out_stream.width(width[6] + 2);
			out_stream << std::left << curr->data->second.getIndependentLiving() << endl;
		}
		curr = curr->next;
	}
	delete curr;
}

// function responsable for WRITE() call in the file
void DisList::Write(string keyname, string scheme, string filename)
{
	//CREATE NEW FILENAME
	string newfilename = "dis";                                                                         // new output file name for geography table   ex: (geoexample.txt.out or geoexample.txt.out.out)
	newfilename += filename;
	ofstream out_stream;
	out_stream.open(newfilename);
	
	//OUTPUT GEOGRAPHY TABLE DATA
	out_stream << keyname << endl; 
	out_stream << scheme;
	
	DisNode* curr = new DisNode;
	curr = head;
	while(curr != NULL)                                                                                 // walking the linked list
	{
		if((curr->data->first.compare("-1") != 0) && (curr->data->first.compare("-99") != 0))           // if tuple is not tombstoned (-99) and not empty space (-1)
		{
			// OUTPUT THE TABLE TO TABLE'S RESPECTED FILE
            out_stream << endl;                                                                        
			out_stream << curr->data->first << ",";
			out_stream << curr->data->second.getHearing() << ",";
			out_stream << curr->data->second.getVision() << ",";
			out_stream << curr->data->second.getCognitive() << ",";
			out_stream << curr->data->second.getAmbulatory() << ",";
			out_stream << curr->data->second.getSelfCare() << ",";
			out_stream << curr->data->second.getIndependentLiving();
		}
		curr = curr->next;
	}
	delete curr;
	out_stream.close();
}

// ============================================
// SELECT, UPDATE, INSERT, AND DELETE FUNCTIONS
// ============================================

void DisTable::Select(smatch matcher, string query, DisList& dislist, ofstream& out_stream)
{
	if(matcher[0].str().find("*") != string::npos)                                                                      // key is not given
	{
		// SELECT ALL RECORDS
        dislist.SelectNodes(query, out_stream);
	}
	else                                                                                                                // key is given
	{
        // SELECT A SINGLE RECORD
        // HASHING INDEX USING MULTIPLICATIVE STRING HASHING
        int index = multiplicativeHash(matcher[1].str(), data.size());
        int numprobe = 1;

		while(data[index].first.compare(matcher[1].str()) != 0)											                // if bucket is full and the key
		{
			if((numprobe == data.size()) || (data[index].first.compare("-1") == 0)) 									// if the key is never found
			{
				// below for loop generates message for no records found
                out_stream << "No entries match query (";
				for(unsigned int i = 1; i < matcher.size(); i++)
				{
					out_stream << matcher[i].str();
					if(i != (matcher.size() - 1))
						out_stream << ",";				
				}
				out_stream << ") in " << name << endl;
				return;                                                                                                 // exits the function
			}
            // QUADRATIC PROBING STRATEGY
            index = quadraticProbing(multiplicativeHash(matcher[1].str(), data.size()), data.size(), numprobe);         // resetting index using quadratic probing
            numprobe ++;                                                                                                // increment number of attempted inserts (ie probing)
		}
		
		//OUTPUT FOUND ENTRY
		out_stream << "Found (";
		for(int i = 1; i < matcher.size(); i++)
		{
			out_stream << matcher[i].str();
			if(i != (matcher.size() - 1))
				out_stream << ",";
		}
		out_stream << ") in " << name << endl;	
	}
}

// this function will delete one or more records by tombstoning the record(s) in the disability table
void DisTable::Delete(smatch matcher, string query, DisList& dislist, ofstream& out_stream, DisTable& dis)
{
	if(matcher[0].str().find("*") != string::npos)                                                                      // if key is not given from query
	{
		// DELTETION OF MULTIPLE RECORDS
        dislist.DeleteNodes(query, out_stream);                                                                         // function to handle deletion of all nodes
	}
	else                                                                                                                // key is given from query
	{
		// REFRESH TABLE
		dis.HashAll();                  // reshash table
		dislist.ResetAll();             // reset secondary index
		dis.CreateList(dislist);        // recreate secondary index
		
		
		// DELETION OF A SINGLE RECORD
        // HASHING INDEX USING MULTIPLICATIVE STRING HASHING
		int index = multiplicativeHash(matcher[1].str(), data.size());
		int numprobe = 1;                                                                                               // number of probe times to check if the table size needs to be increased
		DisData temp(stoi(matcher[2]), stoi(matcher[3]), stoi(matcher[4]), stoi(matcher[5]), stoi(matcher[6]), stoi(matcher[7]));
		

		// checking for the specfic case of key value mismatch
		if(data[index].second != temp && data[index].first.compare((matcher[1]).str()) == 0)
		{
			// below for loop generates fail to delete message which sent is sent to output file
			out_stream << "Failed to delete (";
			for(int i = 1; i < matcher.size(); i++)
			{
				out_stream << matcher[i].str();
				if(i != (matcher.size() - 1))
					out_stream << ",";
			}
			out_stream << ") in " << name << endl;
			return;     
		}
		
		
		while(data[index].first.compare((matcher[1]).str()) != 0)                      									// while key is not equal to current index
		{
			if((numprobe == data.size()) || (data[index].first.compare("-1") == 0))                                     // if the key is never found (ie finds empty space of hashed index or number of probing = table size)
			{
				
				// below for loop generates fail to delete message which sent is sent to output file
                out_stream << "Failed to delete (";
				for(int i = 1; i < matcher.size(); i++)
				{
					out_stream << matcher[i].str();
					if(i != (matcher.size() - 1))
						out_stream << ",";
				}
				out_stream << ") in " << name << endl;
				return;                                                                                                 // exits the function
			}
            // QUADRATIC PROBING STRATEGY
            index = quadraticProbing(multiplicativeHash(matcher[1].str(), data.size()), data.size(), numprobe);         // resetting index using quadratic probing
            numprobe ++;                                                                                                // increment number of attempted inserts (ie probing)
		}
		
		//ERASE FOUND TUPLE BY TOMBSTONING
		Tombstone(data[index]);                                                                                         // this mark the record as tombstoned by setting the key to"-99"
		
        // below for loop generates success delete message which sent is sent to output file
		out_stream << "Deleted (";
		for(int i = 1; i < matcher.size(); i++)
		{
			out_stream << matcher[i].str();
			if(i != (matcher.size() - 1))
				out_stream << ",";
		}
		out_stream << ") in " << name << endl;	
	}
}

// this function will update a single record
void DisTable::Update(pair<string, DisData> pair1, DisList& dislist, ofstream& out_stream)
{
    // HASHING INDEX USING MULTIPLICATIVE STRING HASHING
	int index = multiplicativeHash(pair1.first, data.size());
	int numprobe = 1;

	// UPDATING TUPLE IF KEY IS FOUND IN HASH TABLE
	while((data[index].first != pair1.first))                                                                       // while key is not equal to the current index
	{
		if((numprobe == data.size()) || (data[index].first.compare("-1") == 0))                                     // if the key is never found
		{
			// write failed update message to output file
			out_stream << "Failed to update (" << pair1.first << "," 
                       << pair1.second.getHearing() << "," 
                       << pair1.second.getVision() << "," 
                       << pair1.second.getCognitive() << "," 
                       << pair1.second.getAmbulatory() << "," 
                       << pair1.second.getSelfCare() << "," 
                       << pair1.second.getIndependentLiving() 
                       << ") in " << name << endl;
			return;                                                                                                 // exit the function
		}
        // QUADRATIC PROBING STRATEGY
        index = quadraticProbing(multiplicativeHash(pair1.first, data.size()), data.size(), numprobe);              // resetting index using quadratic probing
        numprobe ++;                                                                                                // increment number of attempted inserts (ie probing)
	}

	//UPDATE TUPLE (erasing and inserting so the pointer in the linked list does not need to be updated)
	data.erase(data.begin() + index);
	data.insert(data.begin() + index, pair1);
	// write update success to output file
	out_stream << "Updated (" << pair1.first << "," 
               << pair1.second.getHearing() << "," 
               << pair1.second.getVision() << "," 
               << pair1.second.getCognitive() << "," 
               << pair1.second.getAmbulatory() << "," 
               << pair1.second.getSelfCare() << "," 
               << pair1.second.getIndependentLiving() 
               << ") in " << name << endl;
}

// this function will insert a single record into the disability table
void DisTable::Insert(pair<string, DisData> pair1, DisList& dislist, ofstream& out_stream)
{
	// HASHING INDEX USING MULTIPLICATIVE STRING HASHING
	int index = multiplicativeHash(pair1.first, data.size());                                   
	int numprobe = 1;                                                                                           // number of probe times to check if the table size needs to be increased

	//INSERTING TUPLE IF KEY IS UNIQUE COMPARED TO HASH TABLE
	while(data[index].first.compare("-1") != 0 && data[index].first.compare("-99") != 0)                        // while index position is not deleted or empty in hash table
	{
        if(data[index].first.compare(pair1.first) == 0)                                                         // if the key is NOT unique then dont add to hash table
		{
			out_stream << "Failed to insert (" << pair1.first << "," 
                       << pair1.second.getHearing() << "," 
                       << pair1.second.getVision() << "," 
                       << pair1.second.getCognitive() << "," 
                       << pair1.second.getAmbulatory() << "," 
                       << pair1.second.getSelfCare() << "," 
                       << pair1.second.getIndependentLiving() << ") into " 
                       << name << endl;                                                                         // output fail to insert message to output file
			return;                                                                                             // exits the function
		}

		// if number of probes equals table size then increase and re hash the table
		if(numprobe == data.size())                                                                             // if number of probed times equals size of hash table
		{
			if(IncreaseSize() == true)                                                                          // increasing size if needed
			{
                HashAll();                                                                                      // rehashing due to changed size
                dislist.ResetAll();                                                                             // clearing the secondary index
				CreateList(dislist);                                                                            // recreating the secondary index                                                               
				numprobe = 1;                                                                                   // resetting probe number
			}
		}
        // QUADRATIC PROBING STRATEGY
		index = quadraticProbing(multiplicativeHash(pair1.first, data.size()), data.size(), numprobe);
        numprobe++;                                                                                             // increment number of attempted inserts (ie probing)
    }
	
	// INSERT TUPLE (erasing and inserting so the pointer in the linked list does not need to be updated)
	data.erase(data.begin() + index);
	data.insert(data.begin() + index, pair1);
    out_stream << "Inserted (" << pair1.first << "," 
                               << pair1.second.getHearing() << "," 
                               << pair1.second.getVision() << "," 
                               << pair1.second.getCognitive() << "," 
                               << pair1.second.getAmbulatory() << "," 
                               << pair1.second.getSelfCare() << "," 
                               << pair1.second.getIndependentLiving() 
                               << ") into " << name << endl;                                                    // ouput success message to outputfile
} 




// this function will select all values that are not tombstoned or empty from the disability table
void DisList::SelectNodes(string query, ofstream& out_stream)
{
	//SELECT NEEDED GROUP
	regex pattern("SELECT\\(\\((.*,.*,.*,.*,.*,.*,.*)\\),disability\\)");
	smatch matcher;
	regex_search(query, matcher, pattern);

	//CHANGE GROUP TO BE REGEX READY AND MAKE NEW REGEX PATTERN
	string oldstr = matcher[1].str();
	regex pattern2("\\*");
	string patternstr = regex_replace(oldstr, pattern2, ".*");
	regex selectpat(patternstr);

	//LOOK FOR MATCHING TUPLES
	DisNode* curr = new DisNode;
	curr = head;
	int numMatches = 0;
	while(curr != NULL)
	{
		if((curr->data->first.compare("-1") != 0) && (curr->data->first.compare("-99") != 0))                                       // if not tombstoned or empty	
		{
			//CREATE ENTRY
			string str = curr->data->first;
			str += ",";
			str += to_string(curr->data->second.getHearing());
			str += ",";
			str += to_string(curr->data->second.getVision());
			str += ",";
			str += to_string(curr->data->second.getCognitive());
			str += ",";
			str += to_string(curr->data->second.getAmbulatory());
			str += ",";
			str += to_string(curr->data->second.getSelfCare());
			str += ",";
			str += to_string(curr->data->second.getIndependentLiving());
			
			
			//FIND MATCHES
			if(regex_match(str, selectpat))
			{
				numMatches++;
				// OUTPUT MATCHES TO FILE
                if(numMatches == 1)
				{
					out_stream << "Found entries:\n";
				}
				out_stream << "(" << str << ") in disability\n";
			}
		}
		curr = curr->next;
	}
	delete curr;

	if(numMatches == 0)
	{
		out_stream << "No entries match query (" << oldstr << ") in disability\n";
	}
}


// this function will delete all records from the disability table by tombstoning
void DisList::DeleteNodes(string query, ofstream& out_stream)
{
	//DELETE NEEDED GROUP
	regex pattern("DELETE\\(\\((.*,.*,.*,.*,.*,.*,.*)\\),disability\\)");
	smatch matcher;
	regex_search(query, matcher, pattern);

	//CHANGE GROUP TO BE REGEX READY AND MAKE NEW REGEX PATTERN
	string oldstr = matcher[1].str();
	regex pattern2("\\*");
	string patternstr = regex_replace(oldstr, pattern2, ".*");
	regex selectpat(patternstr);

	//LOOK FOR MATCHING TUPLES
	DisNode* curr = new DisNode;
	curr = head;
	int numMatches = 0;
	while(curr != NULL)
	{
		if((curr->data->first.compare("-1") != 0) && (curr->data->first.compare("-99") != 0))                                           // if not tombstoned or empty
		{
			//CREATE ENTRY
			string str = curr->data->first; 
			str += ",";
			str += to_string(curr->data->second.getHearing());
			str += ",";
			str += to_string(curr->data->second.getVision());
			str += ",";
			str += to_string(curr->data->second.getCognitive());
			str += ",";
			str += to_string(curr->data->second.getAmbulatory());
			str += ",";
			str += to_string(curr->data->second.getSelfCare());
			str += ",";
			str += to_string(curr->data->second.getIndependentLiving());
			
			//FIND MATCHES
			if(regex_match(str, selectpat))
			{
				numMatches++;
				// OUTPUT MATCHES TO FILE
                if(numMatches == 1)
				{
					out_stream << "Deleted entries:\n";
				}
				out_stream << "(" << str << ") in disability\n";
				curr->data->first = "-99";        // tombstone entry
				curr->data->second = DisData();
			}
		}
		curr = curr->next;
	}
	delete curr;


	if(numMatches == 0)
	{
		out_stream << "No entries match query (" << oldstr << ") in disability\n";
	}

}
