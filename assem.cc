#include<iostream>
#include<sstream>
#include<algorithm>
#include<utility>
#include<queue>
#include<stack>
#include<cassert>
#include "assem.hh"

/*** use -std=c++11 to compile **/

//list<string> s_list = {
//"PUSH","PUSH","PUSH","PUSH","PUSH","JSR FUNC_id_main_L","HALT","LABEL FUNC_id_add_L","LINK 2","ADDF $3 $2 !T0","STOREF !T0 $-1","STOREF $-1 $8","UNLINK","RET","LABEL FUNC_id_main_L","LINK 6","WRITES id_intro","WRITES id_first","READF $-1","WRITES id_second","READF $-2","WRITES id_third","READF $-3","PUSH","PUSHREG","PUSH $-1","PUSH $-2","JSR FUNC_id_multiply_L","POP","POP","POPREG","POP !T2","STOREF !T2 $-5","PUSH","PUSHREG","PUSH $-5","PUSH $-3","JSR FUNC_id_add_L","POP","POP","POPREG","POP !T3","STOREF !T3 $-4","WRITEF $-1","WRITES id_star","WRITEF $-2","WRITES id_plus","WRITEF $-3","WRITES id_equal","WRITEF $-4","WRITES id_eol","STOREI 0 !T4","STOREI !T4 $6","UNLINK","RET","LABEL FUNC_id_multiply_L","LINK 2","MULTF $3 $2 !T1","STOREF !T1 $-1","STOREF $-1 $8","UNLINK","RET"
//
////"IR","code","PUSH","PUSHREGS","JSR FUNC_main","HALT","LABEL FUNC_main","LINK 1","STOREI 0 !T0","STOREI !T0 !T1","STOREI !T1 i","STOREI 0 !T2","STOREI !T2 !T3", "STOREI !T3 a","STOREI 0 !T4","STOREI !T4 !T5","STOREI !T5 b", "LABEL WHILE_START_1","STOREI 10 !T6","EQI i !T6 WHILE_END_2", "READI p","STOREI 10 !T7","LEI p !T7 ELSE_3","STOREI 1 !T9","ADDI a !T9 !T8","STOREI !T8 !T10","STOREI !T10 a","JUMP END_IF_ELSE4","LABEL ELSE_3","STOREI 1 !T12","ADDI b !T12 !T11","STOREI !T11 !T13","STOREI !T13 b","LABEL END_IF_ELSE4","STOREI 1 !T15","ADDI i !T15 !T14","STOREI !T14 !T16","STOREI !T16 i","JUMP WHILE_START_1","LABEL WHILE_END_2","WRITEI a","WRITEI b","RET"
//
////"IR code","PUSH","PUSHREGS","JSR FUNC_maini","HALT","LABEL FUNC_main","LINK 1","STOREI 1 !T0","STOREI !T0 !T1","STOREI !T1 b","STOREI 1 !T2","STOREI !T2 !T3","STOREI !T3 a","LABEL WHILE_START_1","STOREI 120 !T4","GTI a !T4 WHILE_END_2","STOREI 1 !T5","NEI b !T5 ELSE_3","STOREI 1 !T7","MULI !T7 a !T6","STOREI !T6 !T8","STOREI !T8 g","WRITEI g","JUMP END_IF_ELSE4","LABEL ELSE_3","STOREI 2 !T9","NEI b !T9 ELSE_5","STOREI 2 !T11","MULI !T11 a !T10","STOREI !T10 !T12","STOREI !T12 p","WRITEI p","JUMP END_IF_ELSE6","LABEL ELSE_5","STOREI 3 !T13","NEI b !T13 ELSE_7","STOREI 3 !T15","MULI !T15 a !T14","STOREI !T14 !T16","STOREI !T16 k","WRITEI k","JUMP END_IF_ELSE8","LABEL ELSE_7","STOREI 4 !T17","NEI b !T17 ELSE_9","STOREI 4 !T19","MULI !T19 a !T18","STOREI !T18 !T20","STOREI !T20 u","WRITEI u","JUMP END_IF_ELSE10","LABEL ELSE_9","LABEL END_IF_ELSE10","LABEL END_IF_ELSE8","LABEL END_IF_ELSE6","LABEL END_IF_ELSE4","STOREI 1 !T22","ADDI b !T22 !T21","STOREI !T21 !T23","STOREI !T23 b","STOREI 20 !T25","ADDI a !T25 !T24","STOREI !T24 !T26","STOREI !T26 a","JUMP WHILE_START_1","LABEL WHILE_END_2","RET"
//
////"IR code","PUSH","PUSHREGS","JSR FUNC_main","HALT","LABEL FUNC_main","LINK 1","STOREF 0.000100 !T0","STOREF !T0 !T1","STOREF !T1 tolerance","READF num","STOREF num !T2","STOREF !T2 approx","STOREI 0 !T3","STOREI !T3 !T4","STOREI !T4 count","STOREF 0.000000 !T5","STOREF !T5 !T6","STOREF !T6 diff","STOREI 0 !T7","STOREI !T7 !T8","STOREI !T8 enough","LABEL WHILE_START_1","STOREI 1 !T9","EQI enough !T9 WHILE_END_2","STOREI 1 !T11","ADDI count !T11 !T10","STOREI !T10 !T12","STOREI !T12 count","DIVF num approx !T13","ADDF approx !T13 !T14","STOREF 0.500000 !T16","MULF !T16 !T14 !T15","STOREF !T15 !T17","STOREF !T17 newapprox","SUBF approx newapprox !T18","STOREF !T18 !T19","STOREF !T19 diff","STOREF 0.000000 !T20","LEF diff !T20 ELSE_3","STOREF tolerance !T21","GEF diff !T21 ELSE_5","STOREI 1 !T22","STOREI !T22 !T23","STOREI !T23 enough","JUMP END_IF_ELSE6","LABEL ELSE_5","LABEL END_IF_ELSE6","JUMP END_IF_ELSE4","LABEL ELSE_3","STOREF 0.000000 !T25","SUBF !T25 tolerance !T24","STOREF !T24 !T26","LEF diff !T26 ELSE_7","STOREI 1 !T27","STOREI !T27 !T28","STOREI !T28 enough","JUMP END_IF_ELSE8","LABEL ELSE_7","LABEL END_IF_ELSE8","LABEL END_IF_ELSE4","STOREF newapprox !T29","STOREF !T29 approx","JUMP WHILE_START_1","LABEL WHILE_END_2","WRITEF approx","WRITES space","WRITEI count","WRITES eol","RET"
//
////"IR code","PUSH","PUSHREGS","JSR FUNC_main","HALT","LABEL FUNC_main","LINK 1","READF num","STOREI 1 !T0","STOREI !T0 !T1","STOREI !T1 j","STOREF num !T2","STOREF !T2 approx","STOREI 100 !T3","STOREI !T3 !T4","STOREI !T4 i","LABEL WHILE_START_1","STOREI 0 !T5","EQI i !T5 WHILE_END_2","DIVF num approx !T6","ADDF approx !T6 !T7","STOREF 0.500000 !T9","MULF !T9 !T7 !T8","STOREF !T8 !T10","STOREF !T10 newapprox","STOREF newapprox !T11","STOREF !T11 approx","STOREI 1 !T13","SUBI i !T13 !T12","STOREI !T12 !T14","STOREI !T14 i","JUMP WHILE_START_1","LABEL WHILE_END_2","WRITEF approx","WRITES eol","RET"
//
////"IR code","PUSH","PUSHREGS","JSR FUNC_main","HALT","LABEL FUNC_main","LINK 1","STOREI 7 !T0","STOREI !T0 !T1","STOREI !T1 num","STOREI 2 !T2","STOREI !T2 !T3","STOREI !T3 i","STOREI 42 !T4","STOREI !T4 !T5","STOREI !T5 a","STOREI 5 !T6","GEI i !T6 ELSE_1","ADDI num a !T7","STOREI !T7 !T8","STOREI !T8 num","STOREI 3 !T9","GEI i !T9 ELSE_3","ADDI num a !T10","STOREI !T10 !T11","STOREI !T11 num","JUMP END_IF_ELSE4","LABEL ELSE_3","LABEL END_IF_ELSE4","JUMP END_IF_ELSE2","LABEL ELSE_1","STOREI 5 !T12","LTI i !T12 ELSE_5","SUBI num a !T13","STOREI !T13 !T14","STOREI !T14 num","JUMP END_IF_ELSE6","LABEL ELSE_5","LABEL END_IF_ELSE6","LABEL END_IF_ELSE2","WRITEI num","RET"
//
////"PUSH","PUSH","PUSH","PUSH","PUSH","JSR FUNC_id_main_L","HALT","LABEL FUNC_id_factorial_L","LINK 3","STOREI 1 !T0"," NE $2 !T0 IF_ELSE_L0"
////," STOREI 1 !T1"," STOREI !T1 $7"," UNLINK"," RET"," JUMP IF_END_L0"," LABEL IF_ELSE_L0"," STOREI 1 !T2"," SUBI $2 !T2 !T3"," PUSH"," PUSHREG"," PUSH !T3"," JSR FUNC_id_factorial_L","POP"," POPREG"," POP !T4"," STOREI !T4 $-2"," MULTI $-2 $2 !T5"," STOREI !T5 $-1"," LABEL IF_END_L0"," STOREI $-1 $7"," UNLINK"," RET"," LABEL FUNC_id_main_L"," LINK 3"," WRITES id_input"," READI $-1"," STOREI 1 !T6"," NE $-1 !T6 IF_ELSE_L2"," STOREI 1 !T7"," STOREI !T7 $-2"," JUMP IF_END_L2"," LABEL IF_ELSE_L2"," STOREI 1 !T8"," LE $-1 !T8 IF_ELSE_L1"," PUSH"," PUSHREG"," PUSH $-1"," JSR FUNC_id_factorial_L"," POP"," POPREG"," POP !T9"," STOREI !T9 $-2"," JUMP IF_END_L1"," LABEL IF_ELSE_L1"," STOREI 0 !T10"," STOREI !T10 $-2"," LABEL IF_END_L1"," LABEL IF_END_L2"," WRITEI $-2"," WRITES id_eol"," STOREI 0 !T11"," STOREI !T11 $6"," UNLINK"," RET"
//};

size_t split(const string &txt, vector<string> &strs, char ch){
	size_t pos = txt.find(ch);
	size_t initialPos = 0;
	strs.clear();
	// cout <<txt <<endl;

	// Decompose statement
	while(pos != string::npos){
		strs.push_back(txt.substr(initialPos, pos - initialPos));
		initialPos = pos +1;
		pos = txt.find(ch, initialPos);
	}

	// Add the last one
	strs.push_back(txt.substr(initialPos, min(pos, txt.size()) - initialPos + 1));
	return strs.size();
}

////void assign(vector<string>& parm , map<string,string>& m);
//int main(){
//
//	ToAssembly(s_list);
//
//return 0;
//}

// fucntion definition
void ToAssembly(list<string>& s_list){
	int numIR = 0;

	map<string,string> m;
    //map for key words
    map<string, int> keywords;

    m.insert(make_pair(string("STOREI"),string("move")));
	m.insert(make_pair(string("STOREF"),string("move")));
    m.insert(make_pair(string("MULI"),string("muli")));
	m.insert(make_pair(string("DIVI"),string("divi")));
	m.insert(make_pair(string("ADDI"),string("addi")));
	m.insert(make_pair(string("SUBI"),string("subi")));
	m.insert(make_pair(string("CMPI"),string("cmpi")));
    m.insert(make_pair(string("MULF"),string("mulr")));

    m.insert(make_pair(string("EQI"),string("jeq")));
    m.insert(make_pair(string("GTI"),string("jgt")));
    m.insert(make_pair(string("LTI"),string("jlt")));
    m.insert(make_pair(string("LEI"),string("jle")));
    m.insert(make_pair(string("LEF"),string("jle")));
    m.insert(make_pair(string("GEF"),string("jge")));
    m.insert(make_pair(string("GEI"),string("jge")));
	m.insert(make_pair(string("NEI"),string("jne")));
	m.insert(make_pair(string("NE"),string("jne")));

	m.insert(make_pair(string("MULTF"),string("mulr")));
	m.insert(make_pair(string("POP"),string("pop")));

	m.insert(make_pair(string("DIVF"),string("divr")));
	m.insert(make_pair(string("ADDF"),string("addr")));
	m.insert(make_pair(string("SUBF"),string("subr")));
	m.insert(make_pair(string("CMPF"),string("cmpr")));
	m.insert(make_pair(string("INCI"),string("inci")));
	m.insert(make_pair(string("DECI"),string("deci")));
	m.insert(make_pair(string("WRITEI"),string("sys writei")));
	m.insert(make_pair(string("WRITEF"),string("sys writer")));
	m.insert(make_pair(string("WRITES"),string("sys writes")));
	m.insert(make_pair(string("READI"),string("sys readi")));
	m.insert(make_pair(string("READF"),string("sys readr")));
	m.insert(make_pair(string("READS"),string("sys reads")));
	m.insert(make_pair(string("RET"),string("ret")));

	//additions
	m.insert(make_pair(string("JSR"),string("jsr")));
	m.insert(make_pair(string("LINK"),string("link")));
	m.insert(make_pair(string("UNLINK"),string("unlnk")));
	m.insert(make_pair(string("LABEL"),string("label")));
	m.insert(make_pair(string("HALT"),string("sys halt")));
	m.insert(make_pair(string("PUSH"),string("push")));
	m.insert(make_pair(string("JUMP"),string("jmp")));

	keywords.insert(make_pair(string("IR"), int(1)));
	keywords.insert(make_pair(string("code"),int(2)));
	keywords.insert(make_pair(string("PUSHREG"),int(3)));
	keywords.insert(make_pair(string("POPREG"),int(3)));
	keywords.insert(make_pair(string("PUSHREGS"),int(3)));
	keywords.insert(make_pair(string("POPREGS"),int(3)));


	string check;
	list<string>::iterator it;

	Maptype parn;

	for(it=s_list.begin(); it != s_list.end(); it++){

		//cout << *it<< endl;
		check = (string) *it;
		size_t n = count(check.begin(), check.end(),' ');
		parn.insert(make_pair(numIR, make_pair(string(check),size_t(n))));
		numIR++;
	}
	assign(m, parn, keywords);
}

string stackregvalue(map<string,string> &trackreg, list<string>&arguments){

	list<string>::iterator it;
	map<string,string>::iterator mpit;
	string reg = "$-";

	if (arguments.size() ==2 ) {
		string s1 = arguments.front();
		arguments.pop_front();
		string s2 = arguments.front();
		arguments.pop_front();

		//cout << s1 << "  " << s2 << endl;

		if(s1.substr(0,1) == "r" && s2.substr(0,2) == reg){
			mpit = trackreg.find(s2);
			if(mpit == trackreg.end()){
				trackreg.insert(make_pair(string(s2),string(s1)));
			}
			else if (mpit != trackreg.end()) {
				mpit->second = s1;
			}
		}


	}

	//for(it = arguments.begin(); it != arguments.end(); it++){
	//	i//cmp (*it, *(it+1))
	//	//cout << "this is the is in the list " << *it << endl;
	//	//if((*it).substr(0,1) == "r" || (*it++).substr(2,3) == reg){
	//	//	stackreg.insert(make_pair(string((*it++)),string((*it))));
	//	//}
	//	//else if((*it).substr(0,1) == reg || (*it++).substr(2,3) == "r"){
	//	//	stackreg.insert(make_pair(string((*it)),string((*it++))));
	//	//}
	//}
	//for(mpit = stackreg.begin(); mpit != stackreg.end(); mpit++){

	//	cout<< "Here is the mapping " << (*mpit).first << " " << (*mpit).second << endl ;
	//}

	//else if(it==Assignreg.end() && regester.substr(0,2) == reg){

	//	int regnum = stoi(regName.substr(2,3)) - 1;

	//	stringstream ss;
	//	ss << regnum;
	//	string regName = "r" + ss.str();

	//	// assigning register for $- values
	//	Assignreg.insert(make_pair(string(regester),string(regName)));
	//}
	return "pased";
}

void checkForAssignation(string regester, map<string,string> &T_table , map<string, string> &D_table){
	// STORE, Read,
	// number
	// counst
	//
	// !T0
	// $-
	//

	// while flushing it have to reset
	static int count = 0;

	string Tname = "!";
	string Dname = "$-";

	map<string,string>::iterator Tit;
	map<string,string>::iterator Dit;

	string regName;
	stringstream ss;
	ss << count;

	if(regester == "LINK")
		D_table.clear();

	//adding the count variable
	Tit = T_table.find(regester);
	Dit = D_table.find(regester);

	// checking for the temporaries
	if (regester.substr(0,1) == Tname) {
		if (Tit == T_table.end()) {
			regName = "r" + ss.str();
			// regName = regName.replace(0,3,"r" );
			T_table.insert(make_pair(string(regester),string(regName)));
			count ++;
		}
	}
	else if  (regester.substr(0,2) == Dname) {
		if (Dit == D_table.end()) {
			regName = "r" + ss.str();
			// regName = regName.replace(0,3,"r" );
			D_table.insert(make_pair(string(regester),string(regName)));
			count ++;
		}

	}

	//if(regester == "LINK") cout << "Here is the link statement "<< regester <<endl;

	/** printing out all the assigned registers **/
	//for(it=Assignreg.begin(); it!=Assignreg.end(); it++){
	//	//cout<<"name " <<(*it).first <<" assigned name " << (*it).second << endl;
	//}
}

//void regallocation(string reg_in, int count, map<int, string> &regallocated, map<string, string> &Assignreg, map<string,string>& m){
//	int num;
//	stringstream ss;
//	ss << count;
//	string regName = "r" +ss.str();
//
//	// making iterator for both Assignreg
//	map<string,string>::iterator reg_it;
//	map<string,string>::iterator ins_it;
//
//	reg_it = Assignreg.find(reg_in);
//	ins_it = m.find(reg_in);
//
//
//	regallocated.insert(make_pair(int(count),string(regName)));
//	if(reg_in.substr(0,1) != "!" && reg_it == Assignreg.end() && ins_it == m.end()){
//
//		cout << reg_in <<" " << regName << endl;
//	}
//	// taking care of different register mapping
//	else if(reg_it != Assignreg.end()){
//		cout << regName;
//		//cout << "register " << (*reg_it).first << endl;
//		//cout << "geting here " << endl;
//	}
//	// taking care of different instruction
//	else if(ins_it != m.end()){
//		if(!regallocated.empty()){
//			num = (--regallocated.end())->first;
//			num = num +1;
//			stringstream snum;
//			snum << num;
//			regName = "r" +snum.str();
//			cout << "move "<< regName ;
//		}
//	}
//	else{
//		cout<< regName;
//	}
//
////	if(count == 2 && reg_in != "set_dest"){
////		reg_in = regName;
////	}
//	//if( reg_in == "set_dest"){
//	//	cout << storelastregester;
//	//}
//	// cout << "from regester allocation " << count << endl;
//	if((reg_in.substr(0,1) != "!") && (count%2 == 0)) cout << "move ";
//
//}
//


void assign(map<string,string>& m, Maptype parm, map<string, int> &keywords){

	vector<string> v;
	queue<string> reg;

	// creating a stack for storing variables
	list<string> arguments;

	//map for register assignation
	//map<string,string> Assignreg;
	map<string,string> T_table;
	map<string,string> D_table;

	map<string,string> trackreg;
	map<string,string>::iterator tit;
	map<int,string> regallocated;

	map<int,string>::iterator rl;

	// initialize and start
	bool set_lock = false;
	bool Rset_lock = true;
	bool reg_first = false;

	bool llock = true ;

	string Tname = "!";
	string Dname = "$-";

	// initializing iterator for the vecter
	vector<string>::iterator iv;

	// initializing iterator for the map
	map<string,string>::iterator it;
	map<string,string>::iterator Treg_it;
	map<string,string>::iterator Dreg_it;
	map<string,int>::iterator key_it;
	Maptype::iterator ls;

	//split(check, v, ' ');
	//cout << v.size();

	// check if the statment is link;
	bool linkstate = false;
 	int count = 0;
	int order, n, reglink;
	// going in order of the IR code
	for(ls=parm.begin(); ls!=parm.end(); ls++){

		order = (int)(*ls).first;
		std::pair<string,size_t> line = parm[order];
		n = (int)line.second;

		split(line.first, v, ' ');

		if(n < 3){
			// print IR commends
			//cout<<"Here is the line " << line.first << " " <<line.second << endl;
			//cout << line.first << endl;

			// going through each parms in the IR code
			//int count_arg = 0;
			for(iv = v.begin(); iv != v.end(); ++iv){
				//count_arg ++;
				 //cout<<*iv<<endl;
				string type;
				if ((*iv).substr(0,1) == Tname)
					type = "T";
				else if ((*iv).substr(0,2) == Dname)
					type = "D";

				// assigning register for each temporaries
				checkForAssignation((*iv), T_table, D_table);

				//	cout<<(*iv).length()<<endl;
				it = m.find(*iv);

				//looking for the register
				Treg_it = T_table.find(*iv);
				Dreg_it = D_table.find(*iv);
				key_it = keywords.find(*iv);


				// param is a command :
				if(it != m.end()){
					// cout << endl;
					//if((*it).second == "ret"){
					//	cout << "unlnk" <<endl;
					//}
					cout <<(*it).second << " ";

					//stackregvalue(trackreg, arguments);
					//arguments.clear();
					//resetting the stack from the next command
					//while(!arguments.empty()){
					//	arguments.pop();
					//}


				}
				// it is a regester
				else if((Treg_it != T_table.end()) || (Dreg_it != D_table.end())){
				//	if((count == 0) && !set_lock && !Rset_lock && !reg_first){
				//		cout <<regallocated[2]<<" " ;
				//		Rset_lock = true;
				//	}
				//
					if (type == "T")
						cout<<((*Treg_it).second)<<" ";
					else if (type == "D")
						cout<<((*Dreg_it).second)<<" ";
					else
						assert (1==0);
					//cout << (*reg_it).second << " ";
					 //arguments.push_back((*reg_it).second);
				}

				// it is a constant
				else if( it == m.end() && (Treg_it == T_table.end() && Dreg_it == D_table.end()) && key_it == keywords.end() ){
					// starting word
					//
					//
					//for(tit = trackreg.begin(); tit != trackreg.end(); tit++){

					//	cout<< "Here is the mapping " << (*tit).first << " " << (*tit).second << endl ;
					//}
					//if((*iv) == ("PUSHREG" || "PUSHREGS"))
					//	for(int i =0; i < 4 ; i++){
					//		checkForAssignation("push", T_table, D_table);
					//
					//	}
					//else{

					cout << *iv << " ";
					//}
					//arguments.push_back(*iv);
					//tit = trackreg.find(*iv);
					//if(tit != trackreg.end() && count_arg==2){
					//	cout<< (*tit).second << " ";
					//	}
					//else{
					// 	cout << *iv << " ";
					// 	arguments.push_back(*iv);
					//}

					 //if(linkstate){reglink = stoi(*iv); linkstate = false;}
				}

			}
			if(key_it != keywords.end() ){}
			else cout <<endl;

		}

		// handling mult, addi, divi casses;
		else{
			// receiving  arguments
			//cout<<line.first <<endl;

			//cout<<line.first << " "<< endl; // <<line.second;
			for(iv = v.begin(); iv != v.end(); ++iv){
				// cout <<*iv <<endl;
				it = m.find(*iv);

				// assigning register for each temporaries
				checkForAssignation((*iv), T_table, D_table);
				//checkForAssignation((*iv), Assignreg);
				//	cout<<(*iv).length()<<endl;
				// looking for assembly equivalent for IR code
				it = m.find(*iv);

				//looking for the register
				//reg_it = Assignreg.find(*iv);
				string type;
				if ((*iv).substr(0,1) == Tname)
					type = "T";
				else if ((*iv).substr(0,2) == Dname)
					type = "D";

				Treg_it = T_table.find(*iv);
				Dreg_it = D_table.find(*iv);

				// looking for IR keywords
				key_it = keywords.find(*iv);

				// param is a command
				if(it != m.end()){
					// put the alse element used on stack
					reg.push((*it).second);
				//	if ((*it).first == "NE"){
				//		//regallocation( *iv, count, regallocated, Assignreg, m);
				//		cout << " " ;
				//		llock = false;
				//		//cout << " this is before the call of the funciton " <<reg.front() ;
				//	}
					if (((*it).second == "jle" || (*it).second == "jeq" || (*it).second == "jgt" || (*it).second == "jlt"  ||  (*it).second == "jge" || (*it).second == "jne") && (*it).second != ("addi")){
						if((*iv) == "LEF" || (*iv) == "GEF"){
							cout << "cmpr" << " ";
						}
						else cout << "cmpi" << " ";
					}
					else{
					// print the assembly to the screen
					//cout << "comming here" << (*it).second << " ";
					cout << "move " ;
					}
					set_lock = false;
					Rset_lock = false;
					reg_first = true;
				}

				// it is a regester
				//else if(reg_it != Assignreg.end()){
				else if (Treg_it != T_table.end() || Dreg_it != D_table.end()) {

					// if(((count%2) == 0) && set_lock && !Rset_lock && reg_first){
					//		//cout << " this is word " << *iv << endl;
					//		//regallocation( *iv, count, regallocated);
					//		//cout <<"move "<< (*reg_it).second << " ";
					//		if(llock) {cout <<"move "  ;}
					//		regallocation( *iv, count-2, regallocated, Assignreg, m);
					//		cout << " ";
					//		regallocation( *iv, count, regallocated, Assignreg, m);
					//		cout << endl;
					//		cout << reg.front() << " ";
					//		reg.pop();
					//		regallocation(*iv, count-1, regallocated, Assignreg, m);
					//		cout << " ";
					//		regallocation(*iv, count, regallocated, Assignreg, m);
					//		//count++;
					//		count = 0;
					//		set_lock = false;
					//		reg_first = false;
					//	}


					if((set_lock && !Rset_lock) || reg_first){
						//cout << "yes it's here ";
						if (type == "T")
							cout<<((*Treg_it).second)<<" ";
						else if (type == "D")
							cout<<((*Dreg_it).second)<<" ";
						else
							assert (1==0);

						set_lock = false;
						reg_first = false;
					}
					// produce addi a b !T0
					else if(!set_lock && Rset_lock ){

						//cout << " this is: " <<reg.front() <<endl;
						if (type == "T") {
							reg.push((*Treg_it).second);
							cout << (*Treg_it).second << " " <<endl;
						}
						else if (type == "D") {
							reg.push((*Dreg_it).second);
							cout << (*Dreg_it).second << " " <<endl;
						}
						else
							assert (1==0);

						//reg.push((*reg_it).second);
						//cout << (*reg_it).second << " " <<endl;
						cout << reg.front () << " ";
						reg.pop();
						cout << reg.front() << " ";
						reg.pop();
						cout << reg.front() << " ";
						reg.pop();
						set_lock = false;

					}
					else if(set_lock && Rset_lock || !reg_first){

						if((reg.front() == "jle" || reg.front() == "jeq" || reg.front() == "jgt" || reg.front() == "jlt" || reg.front() == "jge" || reg.front() == "jne") && reg.front() != "addi"){
							//cout <<(*reg_it).second<<" ";
							if (type == "T")
								cout<<((*Treg_it).second)<<" ";
							else if (type == "D")
								cout<<((*Dreg_it).second)<<" ";
							else
								assert (1==0);
							reg.push("print");
							reg_first = true;
							}

						else{
							if (type == "T")
								reg.push((*Treg_it).second);
							else if (type == "D")
								reg.push((*Dreg_it).second);
							else
								assert (1==0);
							//reg.push((*reg_it).second);
						}
						set_lock = false;
						Rset_lock = true;
					}
				}

				// it is a constant
				else if(it == m.end() && (Treg_it == T_table.end() && Dreg_it == D_table.end()) && key_it == keywords.end() ){

					if(set_lock || !reg_first){
						reg.push(*iv);
						set_lock = false;
						Rset_lock = true;
					}
					else if(!set_lock){
						int test = (int) reg.size();

						if(test == 2) {
							cout<<endl;
							cout << reg.front() << " "<<*iv;
							reg.pop();
							reg.pop();
						}

						// checking to see if the argument is a memory reference
						//else if((*iv).substr(0,1) == "$"){

						//	regallocation( *iv, count, regallocated, Assignreg, m);

						//	count++;
						//	if(count == 2){
						//		set_lock = true;
						//		Rset_lock = false;
						//		reg_first = true;
						//	}
						//}
						//else if()
						else{
						//cout << (*iv).substr(0,1) << endl;
						//cout << reg.size() << endl;
						cout << *iv << " ";
						set_lock = true;
						Rset_lock = true ;
						reg_first = false;
						}
					}
				}
		}

		// start a new line
		cout << endl;

		// clear the vector
		split("  ", v, ' ');
		}
	}
}


