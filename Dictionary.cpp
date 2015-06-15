#include "Dictionary.h"
#include <string>
#include <list>
#include <cctype>
#include <vector>
#include <algorithm>
using namespace std;

void removeNonLetters(string& string);
void generateNextPermutation(string& permutation);

string order(string thing)
{
    sort(thing.begin(), thing.end());
    return thing;
}

struct Porter
{
    string sorted;
    vector<string> anagrams;
    
};

// This class does the real work of the implementation.

class DictionaryImpl
{
public:
    DictionaryImpl():h_dict(49999) {}
    ~DictionaryImpl() {}
    void insert(string str);
    void lookup(string crate, void callback(string)) const;
    int Key(const string& x) const;
private:
    vector <list<Porter> > h_dict;
    
};




void removeNonLetters(string& s)
{
    string::iterator to = s.begin();
    for (string::const_iterator from = s.begin(); from != s.end(); from++)
    {
        if (isalpha(*from))
        {
            *to = tolower(*from);
            to++;
        }
    }
    s.erase(to, s.end());  // chop off everything from "to" to end.
}



int DictionaryImpl::Key(const string& x) const  //HASH FUNCTION
{
    int h = 0;
    for (int i = 0; i < x.size(); i++)
    {
        //cube of askii value
        h+= (x[i]*x[i]*x[i]);
    }
    //mod by # of bucket
    return h % 49999;
}




void DictionaryImpl::lookup(string crate, void callback(string)) const
{
    //check if theres a function
    if (callback == NULL)
        return;
    
    //remove whatever isn't a letter
    removeNonLetters(crate);
    
    //if empty then return
    if (crate.empty())
        return;
    //call the hash function
    int x = Key(crate);
    string wow = order(crate);
    //loops through the list
    list<Porter>:: const_iterator j = h_dict[x].begin();
    
    while (j != h_dict[x].end()) {
        //if an anagram is found
        if (j->sorted == wow)
        {
            //callback each anagram
            for (vector<string>::const_iterator i = j->anagrams.begin(); i!= j->anagrams.end(); i++)
            {
                callback(*i);
            }
        }
    
        j++;
        
    }
    

        
    
    }


void DictionaryImpl::insert(string str)
{
    //remove non letters
    removeNonLetters(str);
    
    if (!str.empty())
    {
        
        //sort the word and then store into temporary variable
        string t = order(str);
        
        //put the hash key into the x variable
        int x = Key(str);
        
        //while     loop goes through hash table to see if there are anagrams in the dictionary
        list<Porter>::iterator i = h_dict[x].begin();
        
        while (i != h_dict[x].end()) {
            
            if (i->sorted == t)
            {
                i->anagrams.push_back(str);
                return;
            }
            i++;
        }
        
        //if it doesn't exist, create a new one and push it back into the dictionary
        Porter yay;
        yay.sorted = t;
        
        yay.anagrams.push_back(str);
        h_dict[x].push_back(yay);
    }
}


void generateNextPermutation(string& permutation)
{
    string::iterator last = permutation.end() - 1;
    string::iterator p;
    
    for (p = last; p != permutation.begin()  &&  *p <= *(p-1); p--)
        ;
    if (p != permutation.begin())
    {
        string::iterator q;
        for (q = p+1; q <= last  &&  *q > *(p-1); q++)
            ;
        swap(*(p-1), *(q-1));
    }
    for ( ; p < last; p++, last--)
        swap(*p, *last);
}


//******************** Dictionary functions **********************************

// These functions simply delegate to DictionaryImpl's functions.
// You probably don't want to change any of this code.

Dictionary::Dictionary()
{
    m_impl = new DictionaryImpl;
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const
{
    m_impl->lookup(letters, callback);
}