/*
	CSCE 2110.002: Project 3:
	Group 19
	Major Component 2 - Geography Data
	File Name: geo.cpp
	Author: Alexander Williams
*/

#include "main.h"
#include "geo.h"

//GEOGRAPHY HASH TABLE FUNCTIONS
bool GeoTable::IncreaseSize()
{
	//RESIZE VECTOR ONLY IF NEEDED
	bool spaceFound = false; //boolean for if space was found (a size increase if not performed)
	for(unsigned int i = 0; i < data.size(); i++)
	{
		if((data[i].first == -1) || (data[i].first == -99)) //checking if any space remains in the vector. 0 = new space, -99 = deleted old space
		{
			spaceFound = true;
			return false;
		}
	}
	if(spaceFound == false) //resize needed
	{
		int size = data.size();
		for(unsigned int i = 0; i < size; i++) //doubling size of the vector
		{
			data.push_back(make_pair(-1, GeoData()));
		}
		return true;
	}
}

void GeoTable::Tombstone(pair<int, GeoData>& pair1)
{
	//TOMBSTONE PAIR (VALUE THAT IMPLIES THAT THE PAIR HAS BEEN DELETED BY QUERY)
	pair1.first = -99;
	pair1.second = GeoData();
}

int  GeoTable::HashFunc(int key, int size)
{
	//HASH FUNCTION (MODULO)
	return key % size;
}

void GeoTable::HashAll()
{
	//CREATE TEMP VECTOR AND FILL OLD VECTOR WITH EMPTY BUCKETS
	vector<pair<int, GeoData>> temp = data;
	data.clear();
	for(unsigned int i = 0; i < temp.size(); i++)
	{
		data.push_back(make_pair(-1, GeoData()));
	}


	//HASH ALL NON 0 AND NON TombstoneD (-99) KEYS
	for(unsigned int i = 0; i < temp.size(); i++)
	{
		if((temp[i].first != -1) && (temp[i].first != -99))
		{
			//HASH FUNCTION (MODULO)
			int index = HashFunc(temp[i].first, temp.size());
			//LINEAR PROBING COLLISION STRATEGY
			while((data[index].first != -1) && (data[index].first != -99)) //find open bucket
			{
				if(index == (data.size() - 1)) //increase index by one unless at the end of the vector, then reset to 0
				{
					index = 0;
				}
				else
				{
					index++;
				}
			}

			
			//INSERT NEW PAIR AND "DELETE" OLD PAIR
			data.erase(data.begin() + index);
			data.insert(data.begin() + index, temp[i]);
			temp[i] = make_pair(-1, GeoData());
		}
	}
}

void GeoTable::Insert(pair<int, GeoData> pair1, GeoList& geolist, ofstream& out_stream)
{
	//HASHING TUPLE
	//HASH FUNCTION (MODULO)
	int index = HashFunc(pair1.first, data.size());
	int numprobe = 0; //number of probe times to check if the table size needs to be increased


	//INSERTING TUPLE IF KEY IS UNIQUE COMPARED TO HASH TABLE
	while(data[index].first != -1)
	{
		//LINEAR PROBING COLLISION STRATEGY
		numprobe++;
		if(numprobe == data.size()) //if number of probed times equals size of hash table
		{
			if(IncreaseSize() == true) //increasing size
			{
				HashAll();    //rehashing due to changed size
				geolist.ResetAll();
				CreateList(geolist);
				numprobe = 0; //resetting probe number
			}
		}
		if(data[index].first == pair1.first) //if the key is NOT unique (don't add)
		{
			out_stream << "Failed to insert (" << pair1.first << "," << pair1.second.GetName() << "," << pair1.second.GetState() << "," << pair1.second.GetSumlevel() << "," << pair1.second.GetStatenum() << "," << pair1.second.GetCounty() << "," << pair1.second.GetGeoid() << "," << pair1.second.GetPopulation() << ") into " << name << endl;
			index = -1;
			return;
		}
		if(index == (data.size() - 1)) //if index reaches end of hash table
		{
			index = 0; //reset index
		}
		else
		{
			index++;
		}
	}
	
	//INSERT TUPLE (erasing and inserting so the pointer in the linked list does not need to be updated)
	data.erase(data.begin() + index);
	data.insert(data.begin() + index, pair1);
	out_stream << "Inserted (" << pair1.second.GetName() << "," << pair1.second.GetState() << "," << pair1.second.GetSumlevel() << "," << pair1.second.GetStatenum() << "," << pair1.second.GetCounty() << "," << pair1.second.GetGeoid() << "," << pair1.second.GetPopulation() << ") into " << name << endl;
}

void GeoTable::Update(pair<int, GeoData> pair1, GeoList& geolist, ofstream& out_stream)
{
	//HASHING TUPLE
	//HASH FUNCTION (MODULO)
	int index = HashFunc(pair1.first, data.size());
	int numprobe = 0;


	//UPDATING TUPLE IF KEY IS FOUND IN HASH TABLE
	while((data[index].first != pair1.first) && (data[index].first != -99))
	{
		//LINEAR PROBING COLLISION STRATEGY
		numprobe++;
		if((numprobe == data.size()) || (data[index].first == -1)) //if the key is never found
		{
			out_stream << "Failed to update (" << pair1.second.GetName() << "," << pair1.second.GetState() << "," << pair1.second.GetSumlevel() << "," << pair1.second.GetStatenum() << "," << pair1.second.GetCounty() << "," << pair1.second.GetGeoid() << "," << pair1.second.GetPopulation() << ") in " << name << endl;
			return;
		}
		if(index == (data.size() - 1)) //if index reaches end of hash table
		{
			index = 0; //reset index
		}
		else
		{
			index++;
		}
	}


	//UPDATE TUPLE (erasing and inserting so the pointer in the linked list does not need to be updated)
	data.erase(data.begin() + index);
	data.insert(data.begin() + index, pair1);
	out_stream << "Updated (" <<pair1.first << "," << pair1.second.GetName() << "," << pair1.second.GetState() << "," << pair1.second.GetSumlevel() << "," << pair1.second.GetStatenum() << "," << pair1.second.GetCounty() << "," << pair1.second.GetGeoid() << "," << pair1.second.GetPopulation() << ") in " << name << endl;
}

void GeoTable::Select(smatch matcher, string query, GeoList geolist, ofstream& out_stream)
{
	if(matcher[0].str().find("*") != string::npos) //key is not given
	{
		geolist.SelectNodes(query, out_stream);
	}
	else //key is given
	{
		//HASH FUNCTION (MODULO)
		int index = HashFunc(stoi(matcher[1].str()), data.size());
		int numprobe = 0;
		GeoData temp(matcher[2], matcher[3], stoi(matcher[4]), stoi(matcher[5]), stoi(matcher[6]), matcher[7], stoi(matcher[8]));
		
		while((data[index].first != stoi(matcher[1])) && (data[index].second != temp))
		{
			//LINEAR PROBING COLLISION STRATEGY
			numprobe++;
			if((numprobe == data.size()) || (data[index].first == -1)) //if the key is never found
			{
				out_stream << "No entries match query (";
				for(unsigned int i = 1; i < matcher.size(); i++)
				{
					out_stream << matcher[i].str();
					if(i != (matcher.size() - 1))
					{
						out_stream << ",";
					}
				
				}
				out_stream << ") in " << name << endl;
				return;
			}
			if(index == (data.size() - 1)) //if index reaches end of hash table
			{
				index = 0; //reset index
			}
			else
			{
				index++;
			}
		}
		
		
		//OUTPUT FOUND ENTRY
		out_stream << "Found (";
		for(unsigned int i = 1; i < matcher.size(); i++)
		{
			out_stream << matcher[i].str();
			if(i != (matcher.size() - 1))
			{
				out_stream << ",";
			}
		}
		out_stream << ") in " << name << endl;	
	}
}

void GeoTable::Delete(smatch matcher, string query, GeoList geolist, ofstream& out_stream)
{
	if(matcher[0].str().find("*") != string::npos) //key is not given
	{
		geolist.DeleteNodes(query, out_stream);
	}
	else //key is given
	{
		//HASH FUNCTION (MODULO)
		int index = HashFunc(stoi(matcher[1].str()), data.size());
		int numprobe = 0;
		GeoData temp(matcher[2], matcher[3], stoi(matcher[4]), stoi(matcher[5]), stoi(matcher[6]), matcher[7], stoi(matcher[8]));
		
		while((data[index].first != stoi(matcher[1]))&& (data[index].second != temp))
		{
			//LINEAR PROBING COLLISION STRATEGY
			numprobe++;
			if((numprobe == data.size()) || (data[index].first == -1)) //if the key is never found
			{
				out_stream << "Failed to delete (";
				for(unsigned int i = 1; i < matcher.size(); i++)
				{
					out_stream << matcher[i].str();
					if(i != (matcher.size() - 1))
					{
						out_stream << ",";
					}
				}
				out_stream << ") in " << name << endl;
				return;
			}
			if(index == (data.size() - 1)) //if index reaches end of hash table
			{
				index = 0; //reset index
			}
			else
			{
				index++;
			}
		}

		
		//ERASE FOUND TUPLE
		Tombstone(data[index]);
		
		out_stream << "Deleted (";
		for(unsigned int i = 1; i < matcher.size(); i++)
		{
			out_stream << matcher[i].str();
			if(i != (matcher.size() - 1))
			{
				out_stream << ",";
			}
		}
		out_stream << ") in " << name << endl;	
	}
}

void GeoTable::CreateList(GeoList& geolist)
{
	//CREATE INITIAL LINKED LIST
	for(unsigned int i = 0; i < data.size(); i++)
	{
		pair<int, GeoData>* ptr = &(data[i]);
		geolist.AddNode(ptr);
	}
}


//GGEOGRAPHY LINKED LIST FUNCTIONS
void GeoList::AddNode(pair<int, GeoData> *item)
{
	//ADD NODE TO LINKED LIST
	GeoNode *temp = new GeoNode;
	temp->data = item;
	temp->next = NULL;
	if(head == NULL)
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

void GeoList::SelectNodes(string query, ofstream& out_stream)
{
	//SELECT NEEDED GROUP
	regex pattern("SELECT\\(\\((.*,.*,.*,.*,.*,.*,.*,.*)\\),geography\\)");
	smatch matcher;
	regex_search(query, matcher, pattern);


	//CHANGE GROUP TO BE REGEX READY AND MAKE NEW REGEX PATTERN
	string oldstr = matcher[1].str();
	regex pattern2("\\*");
	string patternstr = regex_replace(oldstr, pattern2, ".*");
	regex selectpat(patternstr);


	//LOOK FOR MATCHING TUPLES
	GeoNode* curr = new GeoNode;
	curr = head;
	int numMatches = 0;
	while(curr != NULL)
	{
		//CREATE ENTRY
		string str = to_string(curr->data->first);
		str += ",";
		str += curr->data->second.GetName();
		str += ",";
		str += curr->data->second.GetState();
		str += ",";
		str += to_string(curr->data->second.GetSumlevel());
		str += ",";
		str += to_string(curr->data->second.GetStatenum());
		str += ",";
		str += to_string(curr->data->second.GetCounty());
		str += ",";
		str += curr->data->second.GetGeoid();
		str += ",";
		str += to_string(curr->data->second.GetPopulation());
		
		
		//FIND MATCHES
		if((regex_match(str, selectpat)) && (curr->data->first != -1) && (curr->data->first != -99))
		{
			numMatches++;
			if(numMatches == 1)
			{
				out_stream << "Found entries:\n";
			}
			out_stream << "(" << str << ") in geography\n";
		}
		curr = curr->next;
	}
	delete curr;

	if(numMatches == 0)
	{
		out_stream << "No entries match query (" << oldstr << ") in geography\n";
	}
}

void GeoList::DeleteNodes(string query, ofstream& out_stream)
{
	//DELETE NEEDED GROUP
	regex pattern("DELETE\\(\\((.*,.*,.*,.*,.*,.*,.*,.*)\\),geography\\)");
	smatch matcher;
	regex_search(query, matcher, pattern);


	//CHANGE GROUP TO BE REGEX READY AND MAKE NEW REGEX PATTERN
	string oldstr = matcher[1].str();
	regex pattern2("\\*");
	string patternstr = regex_replace(oldstr, pattern2, ".*");
	regex selectpat(patternstr);


	//LOOK FOR MATCHING TUPLES
	GeoNode* curr = new GeoNode;
	curr = head;
	int numMatches = 0;
	while(curr != NULL)
	{
		string str = to_string(curr->data->first);
		str += ",";
		str += curr->data->second.GetName();
		str += ",";
		str += curr->data->second.GetState();
		str += ",";
		str += to_string(curr->data->second.GetSumlevel());
		str += ",";
		str += to_string(curr->data->second.GetStatenum());
		str += ",";
		str += to_string(curr->data->second.GetCounty());
		str += ",";
		str += curr->data->second.GetGeoid();
		str += ",";
		str += to_string(curr->data->second.GetPopulation());
		
		
		//FIND MATCHES
		if((regex_match(str, selectpat)) && (curr->data->first != -1) && (curr->data->first != -99))
		{
			numMatches++;
			if(numMatches == 1)
			{
				out_stream << "Deleted entries:\n";
			}
			out_stream << "(" << str << ") in geography\n";
			curr->data->first = -99;        //tombstone entry
			curr->data->second = GeoData();
		}
		curr = curr->next;
	}
	delete curr;

	if(numMatches == 0)
	{
		out_stream << "No entries match query (" << oldstr << ") in geography\n";
	}

}

void GeoList::ResetAll()
{
	//RESET ALL NODES (EMPTY LINKED LIST)
	GeoNode* curr = new GeoNode;
	GeoNode* next;
	curr = head;
	while(curr != NULL)
	{
		next = curr->next;
		curr = NULL;
		curr = next;		
	}
	delete curr;
	head = NULL;
	tail = NULL;
}

void GeoList::Display(string scheme, ofstream& out_stream)
{
	GeoNode* curr = new GeoNode;
	curr = head;
	
	vector<string> schemes;
	regex pattern("(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*)");
	smatch matcher;
	regex_search(scheme, matcher, pattern);
	for(unsigned int i = 1; i < matcher.size(); i++)
	{
		schemes.push_back(matcher[i].str());
	}
	

	//FINDING WIDTHS FOR OUTPUTS
	int widthf = 0, widthn = 0, widths = 0, widthsl = 0, widthsn = 0, widthc = 0, widthg = 0, widthp = 0;
	widthf =  (widthf  < schemes[0].size()) ? schemes[0].size() : 0;
	widthn =  (widthn  < schemes[1].size()) ? schemes[1].size() : 0;
	widths =  (widths  < schemes[2].size()) ? schemes[2].size() : 0;
	widthsl = (widthsl < schemes[3].size()) ? schemes[3].size() : 0;
	widthsn = (widthsn < schemes[4].size()) ? schemes[4].size() : 0;
	widthc =  (widthc < schemes[5].size()) ? schemes[5].size() : 0;
	widthg =  (widthg < schemes[6].size()) ? schemes[6].size() : 0;
	widthp =  (widthp < schemes[7].size()) ? schemes[7].size() : 0;
	while(curr != NULL)
	{
		if((curr->data->first != -1) && (curr->data->first != -99))
		{
			int count = 0;
			int num = curr->data->first;
			while(num)
			{
				num = num / 10;
				count++;
			}
			widthf =  (widthf  < count) ? count : widthf;
			count = 0;
			widthn =  (widthn  < curr->data->second.GetName().size()) ? curr->data->second.GetName().size() : widthn;
			widths =  (widths  < curr->data->second.GetState().size()) ? curr->data->second.GetState().size() : widths;
			num = curr->data->second.GetSumlevel();
			while(num)
			{
				num = num / 10;
				count++;
			}
			widthsl =  (widthsl  < count) ? count : widthsl;
			count = 0;
			num = curr->data->second.GetStatenum();
			while(num)
			{
				num = num / 10;
				count++;
			}
			widthsn =  (widthsn  < count) ? count : widthsn;
			count = 0;
			num = curr->data->second.GetCounty();
			while(num)
			{
				num = num / 10;
				count++;
			}
			widthc =  (widthc  < count) ? count : widthc;
			count = 0;
			widthg =  (widthg  < curr->data->second.GetGeoid().size()) ? curr->data->second.GetGeoid().size() : widthg;
			num = curr->data->second.GetPopulation();
			while(num)
			{
				num = num / 10;
				count++;
			}
			widthp =  (widthp  < count) ? count : widthp;
		}
		curr = curr->next;
	}

	
	//OUTPUT HEADER
	out_stream.width(widthf + 2);
	out_stream << std::left << schemes[0];
	out_stream.width(widthn + 2);
	out_stream << std::left << schemes[1];
	out_stream.width(widths + 2);
	out_stream << std::left << schemes[2];
	out_stream.width(widthsl + 2);
	out_stream << std::left << schemes[3];
	out_stream.width(widthsn + 2);
	out_stream << std::left << schemes[4];
	out_stream.width(widthc + 2);
	out_stream << std::left << schemes[5];
	out_stream.width(widthg + 2);
	out_stream << std::left << schemes[6];
	out_stream.width(widthp + 2);
	out_stream << std::left << schemes[7] << endl;
	
	curr = head;
	while(curr != NULL)
	{
		if((curr->data->first != -99) && (curr->data->first != -1))
		{
			out_stream.width(widthf + 2);
			out_stream << std::left << curr->data->first;
			out_stream.width(widthn + 2);
			out_stream << std::left << curr->data->second.GetName();
			out_stream.width(widths + 2);
			out_stream << std::left << curr->data->second.GetState();
			out_stream.width(widthsl + 2);
			out_stream << std::left << curr->data->second.GetSumlevel();
			out_stream.width(widthsn + 2);
			out_stream << std::left << curr->data->second.GetStatenum();
			out_stream.width(widthc + 2);
			out_stream << std::left << curr->data->second.GetCounty();
			out_stream.width(widthg + 2);
			out_stream << std::left << curr->data->second.GetGeoid();
			out_stream.width(widthp + 2);
			out_stream << std::left << curr->data->second.GetPopulation() << endl;
		}
		curr = curr->next;
	}
	delete curr;
}

void GeoList::Write(string keyname, string scheme, string filename)
{
	//CREATE NEW FILENAME
	string newfilename = "geo"; //new output file name for geography table   ex: (geoexample.txt.out or geoexample.txt.out.out)
	newfilename += filename;
	ofstream out_stream;
	out_stream.open(newfilename);

	
	//OUTPUT GEOGRAPHY TABLE DATA
	out_stream << keyname << endl; 
	out_stream << scheme;
	
	GeoNode* curr = new GeoNode;
	curr = head;
	while(curr != NULL)
	{
		if((curr->data->first != -1) && (curr->data->first != -99))
		{
			out_stream << endl;
			out_stream << curr->data->first << ",";
			out_stream << curr->data->second.GetName() << ",";
			out_stream << curr->data->second.GetState() << ",";
			out_stream << curr->data->second.GetSumlevel() << ",";
			out_stream << curr->data->second.GetStatenum() << ",";
			out_stream << curr->data->second.GetCounty() << ",";
			out_stream << curr->data->second.GetGeoid() << ",";
			out_stream << curr->data->second.GetPopulation();
		}
		curr = curr->next;
	}
	delete curr;
	out_stream.close();
}
