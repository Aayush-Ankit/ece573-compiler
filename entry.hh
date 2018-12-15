#ifndef _ENTRY_H_INLCUDED_
#define _ENTRY_H_INLCUDED_

// declarations - types and classes
#include <list>
#include <vector>
#include <string>
#include <stack>
#include <map>

using namespace std;

// definition of string type
typedef string string_t;

// class definiton of integer/float entry for symbol table
class var_entry {
  protected:
    string id;
    string type;
    string value;

  public:
    // constructors
    var_entry(string id_in, string type_in);
    var_entry(string id_in, string type_in, string value_in);

    // accessors
    string getId();
    string getType();
    string getValue();
    void print();

    // modifiers
    void addId(string id_in);
    void addType(string type_in);
};

// class definiton of string entry for symbol table
class str_entry : public var_entry {
  public:
    // constructors
    str_entry(string id_in, string value_in);

    // accessors
    string getValue();
    void print();

    // modifier
    void addValue(char* value_in);
};

// class defnition of code object (IR code)
class CodeObject {
    protected:
        list<string> code_list;
        string result_dest;
        string result_type;
    public:
        // constructors
        CodeObject ();

        // accessors
        string getDest(); // output the destination of result from the CodeObject
        string getType(); // output the type of result from the CodeObject
        list<string> * getCode(); // output the code_list stored in CodeObject
        void print();

        // modifiers
        void addDest(string dest_in);
        void addType(string type_in);
        void addCode(string code_in);
        void addCode(list<string> * code_in_list);
};

// enumeration for types of ASTNodes - variable, literal, operator (=,+,-,*,/), read/write, list of expressions, IF
typedef enum astTypes {VAR, LIT, OP, RW, LIST, FUNC, IF_CONTROL, WHILE_CONTROL, FOR_CONTROL, CALL, RET} astTypes_t;

// abstract class definiton of AST node
class ASTNode {
    protected:
        int if_null; // identify if a AST is null or not
        astTypes_t type; // type of AST node - variable, liteeral, operator
        ASTNode * left_node;
        ASTNode * right_node;
        CodeObject ir_code;
    public:
       // constructors
       ASTNode(astTypes_t type_in);
       ASTNode (int null);

       // modifiers
       void addLeft (ASTNode * node);
       void addRight (ASTNode * node);
       // function for interfacing CodeObject - add dest, type, code of codeobject within ast_node
       void addCodeObjectDest(string dest_in);
       void addCodeObjectType(string type_in);
       void addCodeObjectCode (string code_in);
       void addCodeObjectCode (list<string> * code_in_list);

       // Expr specific modifiers
       virtual void updateType(string type_in); // only applicable for VarRef ASTNode
       virtual void updateId(string id_in); // only applicable for VarRef/RetExpr ASTNode

       // accessssors
       virtual void print() = 0;
       bool isNull (); // a null ASTNode corresponds to 'lambda'
       ASTNode * getLeft();
       ASTNode * getRight();
       astTypes_t getASTType(); // types of AST - VAR/LIT/OP
       // function for interfacing CodeObject - get dest and type of codeobject within ast_node
       string getCodeObjectDest();
       string getCodeObjectType(); // type of CodeObject - INT/FLOAT
       list<string> * getCodeObjectCode();

       // Expr specific accessors
       virtual vector<string> getData(); // applicable to all except If/While/For/Expr
       virtual ASTNode * getCondNode(); // applicable to If/While/For Expr
       virtual ASTNode * getInitNode(); // applicable to If/While/For Expr
       virtual ASTNode * getIncrNode(); // applicable to If/While/For Expr
       virtual list<ASTNode*> * getList(); // applicable to only ExprList
       virtual void addExpr(ASTNode * expr_in); // applicable to only ExprList
       virtual void addExprList(list<ASTNode*> * expr_list_in); // applicable to only ExprList
};

// List of ASTNodes
typedef list<ASTNode *> ASTNodeList;

// API to print the expression represented by an ASTNode
void print_ast (ASTNode * root) ;

//API to print an list of AST tree(s) in-order - list of expressions
void print_ast_list (list<ASTNode*> * ast_list);

// class defintion of symbol table - a node in the tree of symbol tables
class symbolTable_node {
  private:
    string scope_name;
    list<var_entry*> s_table; // list of pointers to symbol table entries in the scope
    list<symbolTable_node*> nested_scope; // list of nested scope
    list<ASTNode*> stmt_list; // list of statements in a scope

  public:
    // constructor
    symbolTable_node();
    symbolTable_node(string scope_n);

    // modifiers
    void addEntry (var_entry * entry);
    void addNode (symbolTable_node * node);
    void addStmt (ASTNode * stmt_node);
    void addScope (string scope_name_in);
    void update_scopeIds (int &count);

    // accessors
    string getScope();
    void print ();
    void print_stmts(); // print the ASTNodes in stmt_list
    list<symbolTable_node*> * getNodes();
    list<var_entry*> * getEntries();
    list<ASTNode*> * getStmts();
    int isEmpty (); // check if a node is a dummy node - not belonging to any while/if block
    int isStmt (); // check if a node is an stmt node - stmt node comes from sub-tree and contains (assign/read/write_stmt)
    string check_duplicate (); // check if
};

// adding a 'list type' for ids
typedef list<string> list_id_t;

// adding a 'list type' for var_entry objects
typedef list<var_entry*> var_entry_list;

// adding a 'stack type' for SymbolTable_nodes
typedef stack<symbolTable_node *> scope_stack;

// adding a 'list type' for symbol table nodes
typedef list<symbolTable_node *> symbolTable_node_list;

// API - add entries from a var_entry_list to a symbolTable_node
void addEntriesFromListToNode (symbolTable_node * node, var_entry_list * entry_list);

// API - add nodes from a symbolTable_node_list to a symbolTable_node
void addNodesFromListToNode (symbolTable_node * node, symbolTable_node_list * node_list);

// API - add nodes from a symbolTable_node_list to a symbolTable_noe_list
void addNodesFromListToList (symbolTable_node_list * src_node_list, symbolTable_node_list * child_node_list);

// API - add entries from a var_entry_list to another var_entry_list
void addEntriesFromListToList (var_entry_list * src_entry_list, var_entry_list * child_entry_list);

// APIs - add ids from a id_list to another id_list/var_entry_list
void addIdsFromListToList (list_id_t * src_id_list, list_id_t * child_id_list);
void addIdsFromListToList (var_entry_list * src_entry_list, list_id_t * child_id_list, string type);

/********* Different types of ASTNodes **********/

// ASTNode for add_op (+,-)
class AddExpr : public ASTNode {
    private:
        string op; // operator ast node
    public:
        AddExpr (); // for defining null AST node
        AddExpr (string op_in);
        void print();
        vector<string> getData();
};

// ASTNode for mul_op (*,/)
class MulExpr : public ASTNode {
    private:
        string op; // operator ast node
    public:
        MulExpr (); // for defining null AST node
        MulExpr (string op_in);
        void print();
        vector<string> getData();
};

// ASTNode for cond_op (=,!=,<,>,<=,>=, TRUE, FALSE)
class CondExpr : public ASTNode {
    private:
        string op;
    public:
        CondExpr (string op_in);
        void print();
        vector<string> getData();
};

// ASTNode for Variable (leaf-node)
class VarRef : public ASTNode {
    private:
        var_entry * var; // Leaf in AST tree, store varId name/type
    public:
        VarRef (string var_id);
        void print();
        vector<string> getData();
        void updateType(string type_in); // Update type of var_entry stored in VarRef ASTNode
        void updateId(string id_in); // only applicable for VarRef ASTNode
};

// ASTNode for Literal (leaf-node)
class LitVal : public ASTNode {
    private:
        string val; // Leaf in AST tree, store int/float literal
        string lit_type;
    public:
        LitVal (string val_in, string type_in);
        void print();
        vector<string> getData();
};

// ASTNode for assign statements (:=)
class AssignExpr : public ASTNode {
    private:
        string op; // assign-operator AST node
    public:
        AssignExpr ();
        void print();
        vector<string> getData();
};

// ASTNode for read/write statements (=)
class RdWrExpr : public ASTNode {
    private:
        string op; // read/write-operator ("r"/"w") AST node
    public:
        RdWrExpr (string op_in);
        void print();
        vector<string> getData();
};

// ASTNode for list of expressions (stmt_list)
class ExprList : public ASTNode {
    private:
        list<ASTNode*> * stmt_list;
    public:
        ExprList (list<ASTNode*> * stmt_list_in);
        ExprList ();
        void print ();
        list<ASTNode*> * getList(); // applicable to only ExprList
        void addExpr(ASTNode * expr_in); // applicable to only ExprList
        void addExprList(list<ASTNode*> * expr_list_in); // applicable to only ExprList

};

// ASTNode for program definition
class ProgExpr : public ASTNode {
    public:
        ProgExpr ();
};

// ASTNode for function definition
class FuncExpr : public ASTNode {
    private:
        string scope_name;
        string link_id;
    public:
        FuncExpr (string scope_name_in);
        void print();
        void updateId (string id_in);
        vector<string> getData();
};

// ASTNode for function call
class CallExpr : public ASTNode {
    private:
        string func_name; // name of the function to call
        list<ASTNode*> * param_list; // list of parameters for function call
    public:
        CallExpr (string func_name_in, list<ASTNode*> * param_list_in);
        void print();
        vector<string> getData();
        list<ASTNode*> * getList(); // applicable to only ExprLis/CallExpr
};

// ASTNode for function call
class RetExpr : public ASTNode {
    private:
        string ret_id;
    public:
        RetExpr ();
        void print();
        void updateId (string id_in);
        vector<string> getData();
};

// ASTNode for if stmt
class IfExpr : public ASTNode {
    private:
        ASTNode * cond_node; // adding an extra ASTNode to AST of If (cond, left(then), right(else))
    public:
        IfExpr (ASTNode * CondExpr_in);
        void print();
        ASTNode * getCondNode();
};

// ASTNode for while stmt
class WhileExpr : public ASTNode {
    private:
        ASTNode * cond_node;
    public:
        WhileExpr (ASTNode * CondExpr_in);
        void print();
        ASTNode * getCondNode();
};

// ASTNode for for stmt
class ForExpr : public ASTNode {
    private:
        ASTNode * cond_node;
        ASTNode * init_node;
        ASTNode * incr_node;
    public:
        ForExpr (ASTNode * AssignExpr_in1, ASTNode * CondExpr_in, ASTNode * AssignExpr_in2);
        void print();
        ASTNode * getCondNode();
        ASTNode * getInitNode();
        ASTNode * getIncrNode();
};

// API to pass AST nodes foun in sub-tree (child symbolTableNode) to root (root symboltableNode)
void addStmtsFromNodeToNode (symbolTable_node * node, symbolTable_node * child_node, string start);

// API to copy stmts from list in sub-tree to parent node's list
void addStmtsFromListToList (list<ASTNode *> * root_list, list<ASTNode *> * child_list, string start);

    // API to crate AST nodes from id_list found in sub-tree (id_list) to root (root symboltableNode)
void addStmtsFromIdListToNode (symbolTable_node * node, list_id_t * child_id_list, string rw_type);

// API to generate IR Code for an ASTNode
void generateIRCode (ASTNode * ast_node);

//// API to traverse ASTs of all statements in a function (function is identified by its symbolTableNode)
//void PostTraverseAST (symbolTable_node * func_node);

// API to traverse AST for one statement (function is identified by its ASTNode)
void PostTraverseAST (ASTNode * ast_node);

// API to add code_list from one ASTNode to other ASTNode
void addIRCodefromListtoList (ASTNode * root_node, ASTNode * child_node);

// API to get fresh temporary
string get_temporary ();

// API to print list of instructions in an ASTNode (CodeObject of ASTNode)
void print_inst_ASTNode (ASTNode * ast_node);

//// API to print list of instructions in a program
//void print_IR_inst (symbolTable_node * curr_scope);

// API to update VarRef types in AST
void updateVarTypeInAST (ASTNode * ast_node, symbolTable_node * curr_scope);

// API to update VarId of function local variables in AST
void updateVarIdInAST (ASTNode * ast_node, map<string, string> * func_ar);

// API to update return address
void updateRETIdInAST (ASTNode * ast_node, int param_size, int local_var_size);

// API to find and update the type of VarRef AST Node in stmt - return True if found
bool FindAndUpdateVarRef (ASTNode * ast_node, symbolTable_node * curr_scope);

// API to read string assignments from a decl_list and add to another list of stmts
list<ASTNode*> * GetStringAssignsFromList(list<var_entry *> * decl_list);

// API to generate IRCode for VAR
CodeObject * generateIRCode_VAR (ASTNode * ast_node);

// API to generate IRCode for LIT
CodeObject * generateIRCode_LIT (ASTNode * ast_node);

// API to generate IRCode for RW
CodeObject * generateIRCode_RW (ASTNode * ast_node);

// API to generate IRCode for OP
CodeObject * generateIRCode_OP (ASTNode * ast_node);

// API to generate IRCode for LIST
CodeObject * generateIRCode_LIST (ASTNode * ast_node);

// API to generate IRCode for FUNC_DECL
CodeObject * generateIRCode_FUNC (ASTNode * ast_node);

// API to generate IRCode for FUNC_CALL
CodeObject * generateIRCode_CALL (ASTNode * ast_node);

// API to generate IRCode for FUNC_RET
CodeObject * generateIRCode_RET (ASTNode * ast_node);

// API to generate IRCode for IF/ELSE
CodeObject * generateIRCode_IF_CONTROL (ASTNode * ast_node);

// API to generate IRCode for WHILE
CodeObject * generateIRCode_WHILE_CONTROL (ASTNode * ast_node);

// API to generate IRCode for FOR
CodeObject * generateIRCode_FOR_CONTROL (ASTNode * ast_node);

// API to generate fresh labels
vector<string> get_if_else_label ();

// API to generate fresh labels
vector<string> get_while_label ();

// API to generate fresh labels
vector<string> get_for_label ();

// API to print declarations before assembly code
void print_decl (symbolTable_node * curr_scope);

// API to create function activation record
map<string, string> * create_activation_record (list<var_entry*> * param_list, list<var_entry*> * local_list);

#endif
