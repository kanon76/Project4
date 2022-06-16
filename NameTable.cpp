//
//  NameTable.cpp
//  Project 4
//
//  Created by Mari Mori on 6/7/22.
//

#include "NameTable.h"
#include <string>
#include <vector>
#include <iostream>
#include <list>
#include <cstdlib>
using namespace std;


const int NUM_BUCK = 19997;


//==================

struct ID // represents items in buckets
{
    ID():name(""), lineNum(0), scopeNum(0){}
    ID(string naming, int liner, int scope):name(naming), lineNum(liner), scopeNum(scope){}
    ~ID() {}
    
    string name;
    int lineNum;
    int scopeNum;
};


class NameTableImpl
{
  public:
    NameTableImpl();
    ~NameTableImpl();
    void enterScope();
    bool exitScope();
    bool declare(const string& id, int lineNum);
    int find(const string& id) const;
    
    int mapFunc(string id) const //the algorithm to convert the name ID into an int
    {
        //break up the string into many chars
        //multiple up the chars
        //then %NUM_BUCK and return that
        
//.        hash<char> hash_string;
        int hashKey = 1;
        
        for (int i=0; i<id.size(); i++)
        {
            hashKey = hashKey*id[i];
        }
        
        hashKey = abs(hashKey);
        return hashKey%NUM_BUCK;
    }
    
private:
    int currScope; // keeps track of which scope we are in
    list<ID> arrID [NUM_BUCK];
    vector <list<ID>> scoper;
};

NameTableImpl::NameTableImpl()
{
    currScope = 0;
    list<ID> li;
    scoper.push_back(li);
}

NameTableImpl::~NameTableImpl()
{
    
}

void NameTableImpl::enterScope()
{
    currScope++;
    list<ID> li;
    scoper.push_back(li);
}

bool NameTableImpl::exitScope()
{
    if (currScope <= 0)
    {
        return false;
    }
    
    //first goes thru each element from the front to end of the the linked list inside the scope vector at index scoper[currScope]. then for each element, find the corresponding node in the linked list in the arrID array and pop front from the linked list from the array at arrID[bucket]. after all of the nodes in the current scope has been popped off, pop off from the scope from the scoper vector
    if(!scoper[currScope].empty())
    {
        for(list<ID>::iterator p=scoper[currScope].begin(); p!=scoper[currScope].end(); p++)
        {
            int bucket = mapFunc(p->name); //get the hash key
            bucket = abs(bucket);

            if (!arrID[bucket].empty())
            {
                arrID[bucket].pop_front(); //remove from the linked list of the array
            }
        }
        scoper.pop_back();
    }
    
    currScope--;
    return true;
}

bool NameTableImpl::declare(const string& id, int lineNum)
{
    if (id.empty())
    {
        return false;
    }
    
    //returns false if this id has already been declared in the same scope
    int bucket = mapFunc(id); //get the hash key
    bucket = abs(bucket);

    if(!arrID[bucket].empty())
    {
        list <ID> temp = arrID[bucket];
        for (list<ID>::iterator p = temp.begin(); p!=temp.end(); p++)
        {
            if(p->name == id && p->scopeNum == currScope)
            {
                return false;
            }
        }
    }
    
    ID newID{id, lineNum, currScope}; //create the new ID object
    arrID[bucket].push_front(newID); //push it to the front of the linked list of the array
    scoper[currScope].push_front(newID); //push it to the front of the linked list of the vector
    
    return true;
}

int NameTableImpl::find(const string& id) const
{
    int bucket = mapFunc(id); //get the hash key
    bucket = abs(bucket);
    
    if(!arrID[bucket].empty())
    {
        list <ID> temp = arrID[bucket];
        for (list<ID>::iterator p = temp.begin(); p!=temp.end(); p++)
        {
            if(p->name == id)
            {
                return p->lineNum;
            }
        }
    }
    
    return -1; // not declared
}

//*********** NameTable functions **************

// For the most part, these functions simply delegate to NameTableImpl's
// functions.

NameTable::NameTable()
{
    m_impl = new NameTableImpl;
}

NameTable::~NameTable()
{
    delete m_impl;
}

void NameTable::enterScope()
{
    m_impl->enterScope();
}

bool NameTable::exitScope()
{
    return m_impl->exitScope();
}

bool NameTable::declare(const string& id, int lineNum)
{
    return m_impl->declare(id, lineNum);
}

int NameTable::find(const string& id) const
{
    return m_impl->find(id);
}
