#ifndef _ASSEM_H_INLCUDED_
#define _ASSEM_H_INLCUDED_

#include<map>
#include<string>
#include<list>
#include<vector>

using namespace std;

// map with instruction and number of param in order
typedef map<int, std::pair<string,size_t> > Maptype;

size_t split(const string &txt, vector<string,string> & Assignreg);

void checkForAssignation(string regester, map<string,string> &T_table , map<string, string> &D_table);

void ToAssembly(list<string>& s_list);

void assign(map<string,string>& m, Maptype parm, map<string, int> &keywords);

#endif
