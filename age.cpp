/*
    CSCE 2110.002: Project 3:
    Group 19
    Major Component 3 - Age Data
    File Name: age.cpp
    Author: Jian Ma
*/

#include "age.h"
#include "main.h"

int AgeTable::MidSquareHashing(string key, int size)
{
    //HASH FUNCTION (MID-SQUARE HASHING)
    int index = 0;
    int sum = 0;
    for (unsigned int i = 0; i < key.size(); i++)
    {
        sum += static_cast<int>(key[i]);
    }
    int pos = sum * sum;
    string spos = to_string(pos);
    string mpos = spos.substr((spos.length())/2, (spos.length())/2);
    index = stoi(mpos);
    return index % size;
}

int AgeTable::DoubleHashing(string key, int size)
{
    //DOUBLE HASHING COLLISION STRATEGIES
    int sum = 0;
    for (unsigned int i = 0; i < key.size(); i++)
    {
        sum += static_cast<int>(key[i]);
    }
    return (3 - (sum % 3));
}

bool AgeTable::IncreaseSize()
{
    //RESIZE VECTOR ONLY IF NEEDED
    bool spaceFound = false; //boolean for if space was found (a size increase if not performed)
    for(unsigned int i = 0; i < data.size(); i++)
    {
        if((data[i].first.compare("-1") == 0) || (data[i].first.compare("-99") == 0)) //checking if any space remains in the vector. 0 = new space, -99 = deleted old space
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
            data.push_back(make_pair("-1", AgeData()));
        }
        return true;
    }
    else
    {
        return false;
    }
}

void AgeTable::Tombstone(pair<string, AgeData>& pair1)
{
    //TOMBSTONE PAIR (VALUE THAT IMPLIES THAT THE PAIR HAS BEEN DELETED BY QUERY)
    pair1.first = "-99";
    pair1.second = AgeData();
}

void AgeTable::HashAll()
{
    //CREATE TEMP VECTOR AND FILL OLD VECTOR WITH EMPTY BUCKETS
    vector<pair<string, AgeData>> temp = data;
    data.clear();
    for(unsigned int i = 0; i < temp.size(); i++)
    {
        data.push_back(make_pair("-1", AgeData()));
    }

    //HASH ALL NON 0 AND NON TombstoneD (-99) KEYS
    for(unsigned int i = 0; i < temp.size(); i++)
    {
        
        if((temp[i].first.compare("-1") != 0) && (temp[i].first.compare("-99") != 0))
        {
            //HASH FUNCTION (MID-SQUARE)
            int index = MidSquareHashing(temp[i].first, temp.size());
            //DOUBLE HASHING COLLISION STRATEGY
            int numprobe = 0;
            while((data[index].first.compare("-1") != 0) && (data[index].first.compare("-99") != 0)) //find open bucket
            {
                numprobe ++;
                if(index == (data.size() - 1)) //increase index by one unless at the end of the vector, then reset to 0
                {
                    index = 0;
                }
                else
                {
                    index = (MidSquareHashing(temp[i].first, temp.size()) + DoubleHashing(temp[i].first, temp.size()) * numprobe) % temp.size();
                }
            }
            
            //INSERT NEW PAIR AND "DELETE" OLD PAIR
            data.erase(data.begin() + index);
            data.insert(data.begin() + index, temp[i]);
            temp[i] = make_pair("-1", AgeData());
        }
    }
}

void AgeTable::Insert(pair<string, AgeData> pair1, AgeList& agelist, ofstream& out_stream)
{
    //HASHING TUPLE
    //HASH FUNCTION (MID-SQUARE)
    int index = MidSquareHashing(pair1.first, data.size());
    int numprobe = 0; //number of probe times to check if the table size needs to be increased


    //INSERTING TUPLE IF KEY IS UNIQUE COMPARED TO HASH TABLE
    while(data[index].first.compare("-1") != 0)
    {
        numprobe ++;
        //DOUBLE HASHING COLLISION STRATEGY
        if(numprobe == data.size()) //if number of probed times equals size of hash table
        {
            if(IncreaseSize() == true) //increasing size
            {
                HashAll();    //rehashing due to changed size
                agelist.ResetAll();
                CreateList(agelist);
                numprobe = 0; //resetting probe number
            }
        }
        if(data[index].first.compare(pair1.first) == 0) //if the key is NOT unique (don't add)
        {
            out_stream << "Failed to insert (" << pair1.first << "," << pair1.second.GetUnder5() << "," << pair1.second.GetUnder18() << "," << pair1.second.GetOver65() << ") into " << name << endl;
            index = -1;
            return;
        }
        if(index == (data.size() - 1)) //if index reaches end of hash table
        {
            index = 0; //reset index
        }
        else
        {
            index = (MidSquareHashing(pair1.first, data.size()) + DoubleHashing(pair1.first, data.size()) * numprobe) % data.size();
        }
    }
    
    //INSERT TUPLE (erasing and inserting so the pointer in the linked list does not need to be updated)
    data.erase(data.begin() + index);
    data.insert(data.begin() + index, pair1);
    out_stream << "Inserted (" << pair1.first << "," << pair1.second.GetUnder5() << "," << pair1.second.GetUnder18() << "," << pair1.second.GetOver65() << ") into " << name << endl;
}

void AgeTable::Update(pair<string, AgeData> pair1, AgeList& agelist, ofstream& out_stream)
{
    //HASHING TUPLE
    //HASH FUNCTION (MID-SQUARE)
    int index = MidSquareHashing(pair1.first, data.size());
    int numprobe = 0;

    //UPDATING TUPLE IF KEY IS FOUND IN HASH TABLE
    while(data[index].first != pair1.first)
    {
        //DOUBLE HASHING COLLISION STRATEGY
        numprobe++;
        if((numprobe == data.size()) || (data[index].first.compare("-1") == 0)) //if the key is never found
        {
            out_stream << "Failed to update (" << pair1.first << "," << pair1.second.GetUnder5() << "," << pair1.second.GetUnder18() << "," << pair1.second.GetOver65() << ") in " << name << endl;
            return;
        }
        if(index == (data.size() - 1)) //if index reaches end of hash table
        {
            index = 0; //reset index
        }
        else
        {
            index = (MidSquareHashing(pair1.first, data.size()) + DoubleHashing(pair1.first, data.size()) * numprobe) % data.size();
        }
    }

    //UPDATE TUPLE (erasing and inserting so the pointer in the linked list does not need to be updated)
    data.erase(data.begin() + index);
    data.insert(data.begin() + index, pair1);
    out_stream << "Updated (" <<pair1.first << "," << pair1.second.GetUnder5() << "," << pair1.second.GetUnder18() << "," << pair1.second.GetOver65() << ") in " << name << endl;
}

void AgeTable::Select(smatch matcher, string query, AgeList agelist, ofstream& out_stream)
{
    if(matcher[0].str().find("*") != string::npos) //key is not given
    {
        agelist.SelectNodes(query, out_stream);
    }
    else //key is given
    {
        //HASH FUNCTION (MID-SQUARE)
        int index = MidSquareHashing(matcher[1].str(), data.size());
        int numprobe = 0;

        while(data[index].first.compare(matcher[1].str()) != 0)
        {
            //DOUBLE HASHING COLLISION STRATEGY
            numprobe++;
            if((numprobe == data.size()) || (data[index].first.compare("-1") == 0)) //if the key is never found
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
                index = (MidSquareHashing(matcher[1].str(), data.size()) + DoubleHashing(matcher[1].str(), data.size()) * numprobe) % data.size();
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

void AgeTable::Delete(smatch matcher, string query, AgeList agelist, ofstream& out_stream)
{
    if(matcher[0].str().find("*") != string::npos) //key is not given
    {
        agelist.DeleteNodes(query, out_stream);
    }
    else //key is given
    {
        //HASH FUNCTION (MID-SQUARE)
        int index = MidSquareHashing(matcher[1].str(), data.size());
        int numprobe = 0;
        
        while(data[index].first.compare((matcher[1]).str()) != 0)
        {
            //DOUBLE HASHING COLLISION STRATEGY
            numprobe++;
            if((numprobe == data.size()) || (data[index].first == "-1")) //if the key is never found
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
                index = (MidSquareHashing(matcher[1].str(), data.size()) + DoubleHashing(matcher[1].str(), data.size()) * numprobe) % data.size();
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

void AgeTable::CreateList(AgeList& agelist)
{
    //CREATE INITIAL LINKED LIST
    for(unsigned int i = 0; i < data.size(); i++)
    {
        pair<string, AgeData>* ptr = &(data[i]);
        agelist.AddNode(ptr);
    }
}

//AGE LINKED LIST FUNCTIONS
void AgeList::AddNode(pair<string, AgeData> *item)
{
    //ADD NODE TO LINKED LIST
    AgeNode *temp = new AgeNode;
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

void AgeList::SelectNodes(string query, ofstream& out_stream)
{
    //SELECT NEEDED GROUP
    regex pattern("SELECT\\(\\((.*,.*,.*,.*)\\),age\\)");
    smatch matcher;
    regex_search(query, matcher, pattern);


    //CHANGE GROUP TO BE REGEX READY AND MAKE NEW REGEX PATTERN
    string oldstr = matcher[1].str();
    regex pattern2("\\*");
    string patternstr = regex_replace(oldstr, pattern2, ".*");
    regex selectpat(patternstr);


    //LOOK FOR MATCHING TUPLES
    AgeNode* curr = new AgeNode;
    curr = head;
    int numMatches = 0;
    while(curr != NULL)
    {
        //CREATE ENTRY
        string str = curr->data->first;
	str += ","; 
	str += to_string(curr->data->second.GetUnder5());
        str += ",";
        str += to_string(curr->data->second.GetUnder18());
        str += ",";
        str += to_string(curr->data->second.GetOver65());
        
        
        //FIND MATCHES
        if((regex_match(str, selectpat)) && (curr->data->first.compare("-1") != 0) && (curr->data->first.compare("-99") != 0))
        {
            numMatches++;
            if(numMatches == 1)
            {
                out_stream << "Found entries:\n";
            }
            out_stream << "(" << str << ") in age\n";
        }
        curr = curr->next;
    }
    delete curr;

    if(numMatches == 0)
    {
        out_stream << "No entries match query (" << oldstr << ") in age\n";
    }
}

void AgeList::DeleteNodes(string query, ofstream& out_stream)
{
    //DELETE NEEDED GROUP
    regex pattern("DELETE\\(\\((.*,.*,.*,.*)\\),age\\)");
    smatch matcher;
    regex_search(query, matcher, pattern);


    //CHANGE GROUP TO BE REGEX READY AND MAKE NEW REGEX PATTERN
    string oldstr = matcher[1].str();
    regex pattern2("\\*");
    string patternstr = regex_replace(oldstr, pattern2, ".*");
    regex selectpat(patternstr);


    //LOOK FOR MATCHING TUPLES
    AgeNode* curr = new AgeNode;
    curr = head;
    int numMatches = 0;
    while(curr != NULL)
    {
        string str = curr->data->first;
	str += ",";  
	str += to_string(curr->data->second.GetUnder5());
        str += ",";
        str += to_string(curr->data->second.GetUnder18());
        str += ",";
        str += to_string(curr->data->second.GetOver65());
        
        
        //FIND MATCHES
        if((regex_match(str, selectpat)) && (curr->data->first.compare("-1") != 0) && (curr->data->first.compare("-99") != 0))
        {
            numMatches++;
            if(numMatches == 1)
            {
                out_stream << "Deleted entries:\n";
            }
            out_stream << "(" << str << ") in age\n";
            curr->data->first = "-99";        //tombstone entry
            curr->data->second = AgeData();
        }
        curr = curr->next;
    }
    delete curr;

    if(numMatches == 0)
    {
        out_stream << "No entries match query (" << oldstr << ") in age\n";
    }
}

void AgeList::ResetAll()
{
    //RESET ALL NODES (EMPTY LINKED LIST)
    AgeNode* curr = new AgeNode;
    AgeNode* next;
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

void AgeList::Display(string scheme, ofstream& out_stream)
{
    AgeNode * curr = new AgeNode;
    curr = head;

    int cmpLength;
    string tupleItem;
    vector<string> schemes;
    vector<int> width;
    regex pattern("(.*),(.*),(.*),(.*)");
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
        if((curr->data->first.compare("-1") != 0) && (curr->data->first.compare("-99") != 0))                                                                        // if tuple is not tombstoned (-99) and not empty space (-1)
        {
            cmpLength = curr->data->first.length();
            width[0] = (width[0] < cmpLength ? cmpLength : width[0]);
            tupleItem = curr->data->second.GetUnder5();
            cmpLength = tupleItem.length();
            width[1] = (width[1] < cmpLength ? cmpLength : width[1]);
            tupleItem = curr->data->second.GetUnder18();
            cmpLength = tupleItem.length();
            width[2] = (width[2] < cmpLength ? cmpLength : width[2]);
            tupleItem = curr->data->second.GetOver65();
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
        if((curr->data->first.compare("-1") != 0) && (curr->data->first.compare("-99") != 0))                                                                        // if tuple is not tombstoned (-99) and not empty space (-1)
        {
            out_stream.width(width[0] + 2);
            out_stream << std::left << curr->data->first;
            out_stream.width(width[1] + 2);
            out_stream << std::left << curr->data->second.GetUnder5();
            out_stream.width(width[2] + 2);
            out_stream << std::left << curr->data->second.GetUnder18();
            out_stream.width(width[3] + 2);
            out_stream << std::left << curr->data->second.GetOver65() << endl;
        }
        curr = curr->next;
    }
    delete curr;
}

void AgeList::Write(string keyname, string scheme, string filename)
{
    //CREATE NEW FILENAME
    string newfilename = "age"; //new output file name for geography table   ex: (geoexample.txt.out or geoexample.txt.out.out)
    newfilename += filename;
    ofstream out_stream;
    out_stream.open(newfilename);

    
    //OUTPUT GEOGRAPHY TABLE DATA
    out_stream << keyname << endl;
    out_stream << scheme;
    
    AgeNode* curr = new AgeNode;
    curr = head;
    while(curr != NULL)
    {
        if((curr->data->first.compare("-1") != 0) && (curr->data->first.compare("-99") != 0))
        {
            out_stream << endl;
            out_stream << curr->data->first << ",";
            out_stream << curr->data->second.GetUnder5() << ",";
            out_stream << curr->data->second.GetUnder18() << ",";
            out_stream << curr->data->second.GetOver65();
        }
        curr = curr->next;
    }
    delete curr;
    out_stream.close();
}

