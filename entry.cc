// Class defnitions, member functions and associated helper functions

#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <cassert>
#include "entry.hh"

using namespace std;

/****** var_entry (symbol table entry) member functions *******/

// constructor(s)
var_entry::var_entry(string id_in, string type_in) : id(id_in), type(type_in)  {return;}
var_entry::var_entry(string id_in, string type_in, string value_in) : id(id_in), type(type_in), value(value_in)  {return;}

// accessor
string var_entry::getId() {return id;}

string var_entry::getType() {return type;}

string var_entry::getValue() {return value;}

void var_entry::print() {
  if (type != "STRING")
    cout << "name " << getId() << " type " << getType() << endl;
  else
    cout << "name " << getId() << " type " << getType() << " value " << getValue() << endl;
}

// modifiers
void var_entry::addId(string id_in) {id = id_in;}

void var_entry::addType(string type_in) {type = type_in;}


/****** str_entry (symbol table entry) member functions *******/
// constructor(s)
str_entry::str_entry(string id_in, string value_in) : var_entry(id_in, "STRING", value_in) {return;}

// accessor
string str_entry::getValue() {return value;}

// modifier
void str_entry::addValue(char* value_in) {value = value_in;}


/****** symbolTable_node (A node in the tree of symbol tables) member functions *******/
// constructors
symbolTable_node::symbolTable_node() : scope_name("NotAssigned") {return;}
symbolTable_node::symbolTable_node(string scope_n) : scope_name(scope_n) {return;}

// modifiers
void symbolTable_node::addEntry (var_entry * entry) {
  s_table.push_back(entry);
}

void symbolTable_node::addNode (symbolTable_node * node) {
    nested_scope.push_back(node);
}

void symbolTable_node::addStmt (ASTNode * stmt_node) {
    stmt_list.push_back(stmt_node);
}

void symbolTable_node::addScope (string scope_name_in) {
    scope_name = scope_name_in;
}

void symbolTable_node::update_scopeIds(int &count) {
    // add scope names to unassigned blocks (these come from stmt_list)
    if (this->getScope() == "NotAssigned") {
        char block_name[10];
        sprintf(block_name, "BLOCK %d", count++);
        this->addScope (block_name);
    }
    // traverse subnodes
    list<symbolTable_node*> temp_node = this->nested_scope;
    if (temp_node.size() > 0) {
      for (list<symbolTable_node*>::iterator it=temp_node.begin(); it!= temp_node.end(); it++)
        (*it)->update_scopeIds(count);
    }
    else
      return;
}

// accessors
string symbolTable_node::getScope(){return scope_name;}

void symbolTable_node::print () { //print entries in symbol table
    cout << "Symbol table " << this->scope_name << endl;

    list<var_entry*> temp_entry = this->s_table;
    for (list<var_entry*>::iterator it=temp_entry.begin(); it!= temp_entry.end(); it++)
      (*it)->print();

    cout << endl;

    list<symbolTable_node*> temp_node = this->nested_scope;
    if (temp_node.size() > 0) {
      for (list<symbolTable_node*>::iterator it=temp_node.begin(); it!= temp_node.end(); it++)
        (*it)->print();
    }
    else
      return;
}

void symbolTable_node::print_stmts () { // print ASTNodes in stmt_list
    list<ASTNode *> temp_stmt_list = this->stmt_list;
    for (list<ASTNode *>::iterator it=temp_stmt_list.begin(); it!=temp_stmt_list.end(); it++) {
        print_ast(*it);
        cout << endl;
    }

    list<symbolTable_node*> temp_node = this->nested_scope;
    if (temp_node.size() > 0) {
      for (list<symbolTable_node*>::iterator it=temp_node.begin(); it!= temp_node.end(); it++)
        (*it)->print_stmts();
    }
    else
      return;
}

list<symbolTable_node*> * symbolTable_node::getNodes() {
  return &(nested_scope);
}

list<var_entry*> * symbolTable_node::getEntries() {
  return &(s_table);
}

list<ASTNode*> * symbolTable_node::getStmts() {
    return &(stmt_list);
}

int symbolTable_node::isEmpty() {
    return (scope_name == "EMPTY");
}

int symbolTable_node::isStmt() {
    return (scope_name == "STMT");
}

string symbolTable_node::check_duplicate () {
    vector<string> id_vec;

    // read ids from symbol table and put it to a temp list
    list<var_entry*> temp_entry = this->s_table;
    for (list<var_entry*>::iterator it=temp_entry.begin(); it!= temp_entry.end(); it++) {
      string id_temp = (*it)->getId();
      // search for id in id_vec
      vector<string>::iterator it;
      it = find (id_vec.begin(), id_vec.end(), id_temp);
      if (it != id_vec.end())
          return *it;
      else
          id_vec.push_back(id_temp);
    }

    // traverse sub-nodes
    list<symbolTable_node*> temp_node = this->nested_scope;
    if (temp_node.size() > 0) {
        string dup;
        for (list<symbolTable_node*>::iterator it=temp_node.begin(); it!= temp_node.end(); it++) {
            dup = (*it)->check_duplicate();
            if (dup != "")
                return dup;
        }
        return "";
    }
    else
        return "";
    delete &id_vec;
}

/****** other APIs used in semantic analysis *******/
void addEntriesFromListToNode (symbolTable_node * node, var_entry_list * entry_list) {
    int n = entry_list->size();
    for (int i=0; i<n; i++) {
        var_entry* temp = entry_list->front();
        entry_list->pop_front();
        node->addEntry(temp);
    }
}

void addNodesFromListToNode (symbolTable_node * node, symbolTable_node_list * node_list) {
    // remove the front node if it is dummy node (stmt-node)
    if ((node_list->front())->isStmt())
        node_list->pop_front();
    // copy non-dummy nodes
    int num_node = node_list->size();
    for (int i=0; i<num_node; i++) {
      symbolTable_node * temp =  node_list->front();
      assert (!temp->isStmt());
      assert (!temp->isEmpty());
      node_list->pop_front();
      node->addNode(temp);
    }
}

void addNodesFromListToList (symbolTable_node_list * src_node_list, symbolTable_node_list * child_node_list) {
    int n = child_node_list->size();
    for (int i=0; i<n; i++) {
        symbolTable_node * temp = child_node_list->front();
        assert (!temp->isStmt());
        assert (!temp->isEmpty());
        child_node_list->pop_front();
        src_node_list->push_back(temp);
    }
}

void addEntriesFromListToList (var_entry_list * src_entry_list, var_entry_list * child_entry_list) {
    int n = child_entry_list->size();
    for (int i=0; i<n; i++) {
        var_entry* temp = child_entry_list->front();
        child_entry_list->pop_front();
        src_entry_list->push_back(temp);
    }
}


void addIdsFromListToList (list_id_t * src_id_list, list_id_t * child_id_list) {
    int n = child_id_list->size();
    for (int i=0; i<n; i++) {
        string temp = child_id_list->front();
        child_id_list->pop_front();
        src_id_list->push_back(temp);
    }
}

void addIdsFromListToList (var_entry_list * src_entry_list, list_id_t * child_id_list, string type) {
    int n = child_id_list->size();
    for (int i=0; i<n; i++) {
      var_entry * temp = new var_entry(child_id_list->front(), type);
      child_id_list->pop_front();
      src_entry_list->push_back(temp);
    }
}

/****** APIs for ASTNode and derived classes *******/

ASTNode::ASTNode(int null) : if_null(null), left_node(NULL), right_node(NULL) { return; }
ASTNode::ASTNode(astTypes_t type_in) : type(type_in), if_null(0), left_node(NULL), right_node(NULL) { return; }

bool ASTNode::isNull () { return (if_null == 1); }
void ASTNode::addLeft (ASTNode * node) { left_node = node; }
void ASTNode::addRight (ASTNode * node) { right_node = node; }
ASTNode * ASTNode::getLeft () {return left_node; }
ASTNode * ASTNode::getRight () {return right_node; }
astTypes_t ASTNode::getASTType () { return type; }
void ASTNode::updateType(string type_in) { assert (1==0); } // updateType is invalid func_call for non VarRef ASTNode
void ASTNode::updateId(string id_in) { assert (1==0); } // updateType is invalid func_call for non VarRef ASTNode
vector<string> ASTNode::getData() { //return operator
    vector<string> data;
    return data;
}
ASTNode * ASTNode::getCondNode() { return NULL; }
ASTNode * ASTNode::getInitNode() { return NULL; }
ASTNode * ASTNode::getIncrNode() { return NULL; }
list<ASTNode*> * ASTNode::getList() { return NULL; }
void ASTNode::addExpr(ASTNode * expr_in) {};
void ASTNode::addExprList(list<ASTNode*> * expr_list_in) {};

// ast_node's function for handling codeobject within ast_node
void ASTNode::addCodeObjectDest (string dest_in) { ir_code.addDest(dest_in); }
void ASTNode::addCodeObjectType (string type_in) { ir_code.addType(type_in); }
void ASTNode::addCodeObjectCode (string code_in) { ir_code.addCode(code_in); }
void ASTNode::addCodeObjectCode (list<string> * code_in_list) { ir_code.addCode(code_in_list); }
string ASTNode::getCodeObjectDest () { return ir_code.getDest(); }
string ASTNode::getCodeObjectType () { return ir_code.getType(); }
list<string> * ASTNode::getCodeObjectCode () { return ir_code.getCode(); }

AddExpr::AddExpr() : ASTNode(1) { return; }
AddExpr::AddExpr(string op_in) : op(op_in), ASTNode(OP) { return; }
void AddExpr::print() { cout << op << ""; }
vector<string> AddExpr::getData() { //return operator
    vector<string> data;
    data.push_back(op);
    return data;
}

MulExpr::MulExpr() : ASTNode(1) { return; }
MulExpr::MulExpr(string op_in) : op(op_in), ASTNode(OP) { return; }
void MulExpr::print() { cout << op << ""; }
vector<string> MulExpr::getData() { //return operator
    vector<string> data;
    data.push_back(op);
    return data;
}

VarRef::VarRef(string var_id) : var(new var_entry(var_id,"na")), ASTNode(VAR) { return; } // "na" - type of variable found in expression is yet to be determined
void VarRef::print() { cout << var->getId() << ""; }
vector<string> VarRef::getData() { //return Id, Type
    vector<string> data;
    data.push_back(var->getId());
    data.push_back(var->getType());
    return data;
}
void VarRef::updateType(string type_in) { var->addType(type_in); }
void VarRef::updateId(string id_in) { var->addId(id_in); }

LitVal::LitVal(string val_in, string type_in) : val(val_in), lit_type(type_in), ASTNode(LIT) { return; }
void LitVal::print() { cout << val << ""; }
vector<string> LitVal::getData() { //return Val, Lit_type
    vector<string> data;
    data.push_back(val);
    data.push_back(lit_type);
    return data;
}

AssignExpr::AssignExpr() : op(":="), ASTNode(OP) { return; }
void AssignExpr::print() { cout << op << ""; }
vector<string> AssignExpr::getData() { //return operator
    vector<string> data;
    data.push_back(op);
    return data;
}

RdWrExpr::RdWrExpr(string op_in) : op(op_in), ASTNode(RW) { return; }
void RdWrExpr::print() { cout << "-" << op << ""; }
vector<string> RdWrExpr::getData() { //return operator
    vector<string> data;
    data.push_back(op);
    return data;
}

CondExpr::CondExpr(string op_in) :op(op_in), ASTNode(OP) { return; }
void CondExpr::print() { cout << op << ""; }
vector<string> CondExpr::getData() { //return operator
    vector<string> data;
    data.push_back(op);
    return data;
}

ExprList::ExprList (list<ASTNode*> * stmt_list_in) : stmt_list(stmt_list_in), ASTNode(LIST) { return; }
ExprList::ExprList () : stmt_list(new list<ASTNode*>), ASTNode(LIST) { return; }
void ExprList::print() { print_ast_list(stmt_list); }
list<ASTNode*> * ExprList::getList() { return stmt_list; }
void ExprList::addExpr(ASTNode * expr_in) { stmt_list->push_back(expr_in); } // applicable to only ExprList
void ExprList::addExprList(list<ASTNode*> * expr_list_in) { stmt_list->splice(stmt_list->end(), *expr_list_in); } // applicable to only ExprList

FuncExpr::FuncExpr (string scope_name_in) : scope_name(scope_name_in), ASTNode(FUNC) { return; }
void FuncExpr::print () { cout << scope_name << ""; }
void FuncExpr::updateId (string id_in) { link_id=id_in; }
vector<string> FuncExpr::getData() { //return scope_name
    vector<string> data;
    data.push_back(scope_name);
    data.push_back(link_id);
    return data;
}

CallExpr::CallExpr (string func_name_in, list<ASTNode*> * param_list_in) : func_name(func_name_in), param_list(param_list_in), ASTNode(CALL) { return; }
list<ASTNode*> * CallExpr::getList() { return param_list; }
void CallExpr::print () { cout << func_name << "(";
                          print_ast_list(param_list);
                          cout << ")" << endl;
                        }
vector<string> CallExpr::getData() { //return scope_name
    vector<string> data;
    data.push_back(func_name);
    return data;
}

RetExpr::RetExpr () : ASTNode(RET) { return; }
void RetExpr::print () { cout << "RET"; }
void RetExpr::updateId (string id_in) { ret_id=id_in; }
vector<string> RetExpr::getData() { //return scope_name
    vector<string> data;
    data.push_back(ret_id);
    return data;
}

IfExpr::IfExpr (ASTNode * CondExpr_in) : cond_node(CondExpr_in), ASTNode(IF_CONTROL) { return; }
ASTNode * IfExpr::getCondNode() { return cond_node; }
void IfExpr::print() {
    print_ast (cond_node);
    cout << endl;
}

WhileExpr::WhileExpr (ASTNode * CondExpr_in) : cond_node(CondExpr_in), ASTNode(WHILE_CONTROL) { return; }
ASTNode * WhileExpr::getCondNode() { return cond_node; }
void WhileExpr::print() {
    print_ast (cond_node);
    cout << endl;
}

ForExpr::ForExpr (ASTNode * AssignExpr_in1, ASTNode * CondExpr_in, ASTNode * AssignExpr_in2)
    : init_node(AssignExpr_in1), cond_node(CondExpr_in), incr_node(AssignExpr_in2), ASTNode(FOR_CONTROL) { return; }
ASTNode * ForExpr::getCondNode() { return cond_node; }
ASTNode * ForExpr::getInitNode() { return init_node; }
ASTNode * ForExpr::getIncrNode() { return incr_node; }
void ForExpr::print() {
    print_ast (cond_node);
    cout << " ";
    print_ast (init_node);
    cout << " ";
    print_ast (incr_node);
    cout << endl;
}

// API to copy stmts from sub-tree to parent node
void addStmtsFromNodeToNode (symbolTable_node * node, symbolTable_node * child_node, string start) {
    assert (start=="front" || start=="back"); // astList in symbolTableNode can be read either front/back
    list<ASTNode*> * temp = child_node->getStmts();
    int n = temp->size();
    ASTNode* ast_temp;
    for (int i=0; i<n; i++) { // entries get added to stmt_list opposite to program order
        if (start == "back"){
            ast_temp = temp->back();
            temp->pop_back();
        }
        else {
            ast_temp = temp->front();
            temp->pop_front();
        }
        node->addStmt(ast_temp);
    }
}

// API to copy stmts from list in sub-tree to parent node's list
void addStmtsFromListToList (list<ASTNode *> * root_list, list<ASTNode *> * child_list, string start) {
    assert (start=="front" || start=="back"); // astList in symbolTableNode can be read either front/back
    int n = child_list->size();
    ASTNode* ast_temp;
    for (int i=0; i<n; i++) { // entries get added to stmt_list opposite to program order
        if (start == "back"){
            ast_temp = child_list->back();
            child_list->pop_back();
        }
        else {
            ast_temp = child_list->front();
            child_list->pop_front();
        }
        root_list->push_back(ast_temp);
    }
}

// API to crate AST nodes from id_list found in sub-tree (id_list) to root (root symboltableNode)
void addStmtsFromIdListToNode (symbolTable_node * node, list_id_t * id_list, string rw_type) {
    int n = id_list->size();
    for (int i=0; i<n; i++) {
        // Create a VarRef AST node - will be child of RdWrExpr AST node
        string id_temp = id_list->back(); //Back because id_list gets created in opposite to program order
        id_list->pop_back();
        ASTNode * var_temp = new VarRef (id_temp);

        // Create RdWrExpr AST node
        ASTNode * temp_ast_node = new RdWrExpr(rw_type);
        temp_ast_node->addLeft (var_temp);
        node->addStmt (temp_ast_node);
    }
}

// APIs for CodeObject class
CodeObject::CodeObject () { return; }
string CodeObject::getDest () { return result_dest; }
string CodeObject::getType () { return result_type; }
list<string> * CodeObject::getCode () { return &code_list; }
void CodeObject::addDest (string dest_in) { result_dest = dest_in; }
void CodeObject::addType (string type_in) { result_type = type_in; }
void CodeObject::addCode (string code_in) { code_list.push_back(code_in); }
void CodeObject::addCode (list<string> * code_in_list) {
    list<string>::iterator it;
    it = code_list.end(); // it points to 'after last instr' in code_list
    code_list.insert(it, (*code_in_list).begin(), (*code_in_list).end());
}
void CodeObject::print () {
    for (list<string>::iterator it=code_list.begin(); it!=code_list.end(); it++)
        cout << *it << endl;
}

// API to add code_list from one ASTNode to other ASTNode
void addIRCodefromListtoList (ASTNode * root_node, ASTNode * child_node) {
    list<string> * code_temp = child_node->getCodeObjectCode();
    for (list<string>::iterator it=(*code_temp).begin(); it!=(*code_temp).end(); it++) {
        root_node->addCodeObjectCode(*it);
    }
}

// API to read string assignments from a decl_list and add to another list of stmts
list<ASTNode*> * GetStringAssignsFromList(list<var_entry *> * decl_list) {
    list<ASTNode*> * str_assign_list = new list<ASTNode *>;
    ASTNode * temp;
    ASTNode * child_l;
    ASTNode * child_r;

    for (list<var_entry *>::iterator it=(*decl_list).begin(); it!=(*decl_list).end(); it++) {
        string type = (*it)->getType();
        if (type == "STRING") {
            temp = new AssignExpr();
            child_l = new VarRef((*it)->getId());
            child_r = new LitVal((*it)->getValue(), (*it)->getType());
            temp->addLeft (child_l);
            temp->addRight (child_r);
            str_assign_list->push_back(temp);
        }
    }
    return str_assign_list;
}

