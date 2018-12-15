// Helper functions used in compiler

#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <cassert>
#include "entry.hh"

using namespace std;

//API to print an AST tree  in-order
void print_ast (ASTNode * root) {
    ASTNode * left_temp = root->getLeft();
    ASTNode * right_temp = root->getRight();

    // print start in case of if/else, while, for
    astTypes_t type = root->getASTType();
    if (type==IF_CONTROL) cout << "IF START: " << endl;
    else if (type==WHILE_CONTROL) cout << "WHILE START: " << endl;
    else if (type==FOR_CONTROL) cout << "FOR START: " << endl;

    // In-order traversal
    // print left sub-tree
    if (left_temp != NULL)
        print_ast (left_temp);

    // print node
    root->print();

    // print right sub-tree
    if (right_temp != NULL)
        print_ast (right_temp);

    // print end in case of if/else, while, for
    if (type==IF_CONTROL) cout << "END_IF";
    else if (type==WHILE_CONTROL) cout << "END_WHILE";
    else if (type==FOR_CONTROL) cout << "END_FOR";

    return;
}

//API to print an list of AST tree(s) in-order - list of expressions
void print_ast_list (list<ASTNode*> * ast_list) {
    for (list<ASTNode *>::iterator it=(*ast_list).begin(); it!=(*ast_list).end(); it++) {
        print_ast(*it);
        cout << endl;
    }
}

// API to generate fresh temporary
string get_temporary() {
    static int temp_count = 0;
    char temp_name[5];
    sprintf(temp_name, "!T%d", temp_count++);
    return temp_name;
}

// API to update VarRef types in AST
void updateVarTypeInAST (ASTNode * ast_node, symbolTable_node * curr_scope) {
    // traverse all nodes, check if VarRef node, find variable in local/global scope and update its type
    ASTNode * child_l = ast_node->getLeft();
    ASTNode * child_r = ast_node->getRight();

    if (child_l != NULL)
        updateVarTypeInAST (child_l, curr_scope);
    if (child_r != NULL)
        updateVarTypeInAST (child_r, curr_scope);

    // for ast-list, traverse individual expresssions in list
    if (ast_node->getList() != NULL) {
        assert ((ast_node->getASTType() == LIST) || (ast_node->getASTType() == CALL)); // all other than LIST AST will return NULL
        list<ASTNode *> * expr_list = ast_node->getList();
        for (list<ASTNode *>::iterator it=(*expr_list).begin(); it!=(*expr_list).end(); it++)
            updateVarTypeInAST (*it, curr_scope);
    }

    // for if/while/for explicitly do cond/incr/init asts
    if (ast_node->getCondNode() != NULL)
        updateVarTypeInAST (ast_node->getCondNode(), curr_scope);
    if (ast_node->getInitNode() != NULL)
        updateVarTypeInAST (ast_node->getInitNode(), curr_scope);
    if (ast_node->getIncrNode() != NULL)
        updateVarTypeInAST (ast_node->getIncrNode(), curr_scope);

    // for ast-var, find and update type
    if (ast_node->getASTType() == VAR)
        if ((ast_node->getData())[1] == "na")
            FindAndUpdateVarRef (ast_node, curr_scope);
    return;
}


// API to update VarId of function local variables in AST
void updateVarIdInAST (ASTNode * ast_node, map<string, string> * func_ar) {
    // return if write
    //if ((ast_node->getASTType() == RW) && (ast_node->getData()[0] == "w"))
    //    return;

    // traverse the expr_list in the function
    if (ast_node->getList() != NULL) {
        assert ((ast_node->getASTType() == LIST) || (ast_node->getASTType() == CALL)); // all other than LIST AST will return NULL
        list<ASTNode *> * expr_list = ast_node->getList();
        for (list<ASTNode *>::iterator it=(*expr_list).begin(); it!=(*expr_list).end(); it++)
            updateVarIdInAST (*it, func_ar);
    }

   // traverse non-LIST ASTNodes
    ASTNode * child_l = ast_node->getLeft();
    ASTNode * child_r = ast_node->getRight();

    if (child_l != NULL)
        updateVarIdInAST (child_l, func_ar);
    if (child_r != NULL)
        updateVarIdInAST (child_r, func_ar);

   // for if/while/for explicitly do cond/incr/init asts
    if (ast_node->getCondNode() != NULL)
        updateVarIdInAST (ast_node->getCondNode(), func_ar);
    if (ast_node->getInitNode() != NULL)
        updateVarIdInAST (ast_node->getInitNode(), func_ar);
    if (ast_node->getIncrNode() != NULL)
        updateVarIdInAST (ast_node->getIncrNode(), func_ar);

    // for ast-var, find and update type
    map<string, string>::iterator ar_slotId;
    string id;
    if (ast_node->getASTType() == VAR) {
        id = (ast_node->getData())[0];
        ar_slotId = func_ar->find(id);
        if (ar_slotId != func_ar->end()) // only update for local variable
            ast_node->updateId(ar_slotId->second);
    }

    return;
}


// API to update return address
void updateRETIdInAST (ASTNode * ast_node, int param_size, int local_var_size) {
    // traverse the expr_list in the function
    if (ast_node->getList() != NULL) {
        assert ((ast_node->getASTType() == LIST) || (ast_node->getASTType() == CALL)); // all other than LIST AST will return NULL
        list<ASTNode *> * expr_list = ast_node->getList();
        for (list<ASTNode *>::iterator it=(*expr_list).begin(); it!=(*expr_list).end(); it++)
            updateRETIdInAST (*it, param_size, local_var_size);
    }

   // traverse non-LIST ASTNodes
    ASTNode * child_l = ast_node->getLeft();
    ASTNode * child_r = ast_node->getRight();

    if (child_l != NULL)
        updateRETIdInAST (child_l, param_size, local_var_size);
    if (child_r != NULL)
        updateRETIdInAST (child_r, param_size, local_var_size);

    // for ast-var, find and update type
    char ret_id[20];
    sprintf(ret_id, "$%d", param_size+2);
    char link_id[20];
    sprintf(link_id, "%d", local_var_size+1);
    if (ast_node->getASTType() == RET) {
        ast_node->updateId(ret_id); // Updates param_size for correct calculation o return value's add
    }
    if (ast_node->getASTType() == FUNC) { // Updates local_var_size for coorect calc. of link size
        ast_node->updateId(link_id);
    }

    return;
}


// API to find and update the type of VarRef AST Node in stmt
bool FindAndUpdateVarRef (ASTNode * ast_node, symbolTable_node * curr_scope) {
    vector<string> data = ast_node->getData();
    string varId = data[0];
    assert (data[1] == "na"); // VarRef ASTNode's initial type value is "na"

    // search Id in symboltable
    string type_found = "NOTFOUND";
    list<var_entry*> * temp_entry = curr_scope->getEntries();
    for (list<var_entry*>::iterator it=(*temp_entry).begin(); it!=(*temp_entry).end(); it++) {
        if ((*it)->getId() == varId)
            type_found = (*it)->getType();
    }

    // update type
    if (type_found == "INT" || type_found == "FLOAT" || type_found == "STRING") {
        ast_node->updateType (type_found);
        return true;
    }
    return false;
}

// API to print list of instructions in an ASTNode (CodeObject of ASTNode)
void print_inst_ASTNode (ASTNode * ast_node) {
    list<string> * inst_list = ast_node->getCodeObjectCode();
    for (list<string>::iterator it=(*inst_list).begin(); it!=(*inst_list).end(); it++)
        cout << ";" <<(*it) << endl;
}

//// API to print all instructions (all scopes) in a program
//void print_IR_inst (symbolTable_node * curr_scope) {
//    // traverse the statements in this scope
//    list<ASTNode *> * astList = curr_scope->getStmts();
//    for (list<ASTNode *>::iterator it=(*astList).begin(); it!=(*astList).end(); it++) {
//        ASTNode * temp = *it;
//        assert (!temp->isNull()); // A null ASTNode (lambda) can't be part of an expression
//        print_inst_ASTNode (temp);
//    }
//
//    // traverse nested scopes
//    list<symbolTable_node*> * temp_node = curr_scope->getNodes();
//    if (temp_node->size() > 0) {
//      for (list<symbolTable_node*>::iterator it=(*temp_node).begin(); it!=(*temp_node).end(); it++)
//        print_IR_inst (*it);
//    }
//    else
//      return;
//}

//// API to traverse ASTs of all statements (of a function) in a scope - function is identified by its symbolTableNode
//// also generates function header/footer instructions
//void PostTraverseAST (symbolTable_node * node) {
//    // traverse statements in this function scope
//    list<ASTNode *> * astList = node->getStmts();
//
//    // add function header - LABEL, LINK
//    string label_inst;
//    label_inst.assign("LABEL FUNC_");
//    label_inst += node->getScope();
//    if (astList->size() > 0) {
//        astList->front()->addCodeObjectCode(label_inst);
//        astList->front()->addCodeObjectCode("LINK");
//    }
//    // add instructions of statement within function
//    for (list<ASTNode *>::iterator it=(*astList).begin(); it!=(*astList).end(); it++) {
//        ASTNode * temp = *it;
//        assert (!temp->isNull()); // A null ASTNode (lambda) can't be part of an expression
//
//        // post-order traversal of temp to add codeobject
//        ASTNode * child_l = temp->getLeft();
//        ASTNode * child_r = temp->getRight();
//        if (child_l != NULL)
//            PostTraverseAST (child_l);
//        if (child_r != NULL)
//            PostTraverseAST (child_r);
//        generateIRCode(temp);
//    }
//    // add function footer - RET
//    if (astList->size() > 0) {
//        astList->back()->addCodeObjectCode("RET");
//    }
//
//    // traverse nested scopes
//    list<symbolTable_node*> * temp_node = node->getNodes();
//    if (temp_node->size() > 0) {
//      for (list<symbolTable_node*>::iterator it=(*temp_node).begin(); it!=(*temp_node).end(); it++)
//        PostTraverseAST (*it);
//    }
//}

// API to traverse AST for an root AST-node (a root AST node can be expr/stmt or expr-list) (function is identified by its ASTNode)
void PostTraverseAST (ASTNode * ast_node) {
        assert (!ast_node->isNull()); // A null ASTNode (lambda) can't be part of an expression

        // post-order traversal of temp to add codeobject
        ASTNode * child_l = ast_node->getLeft();
        ASTNode * child_r = ast_node->getRight();
        if (child_l != NULL)
            PostTraverseAST (child_l);
        if (child_r != NULL)
            PostTraverseAST (child_r);
        generateIRCode(ast_node);
        return;
}

// API to generate IR Code for an ASTNode - look at left/right child (as required), and generate inst for one ASTNode
void generateIRCode (ASTNode * ast_node) {
    CodeObject * ir = NULL;
    astTypes_t ast_type = ast_node->getASTType();

    switch (ast_type) {
        case VAR: { ir = generateIRCode_VAR (ast_node);
                    break; }

        case LIT: { ir = generateIRCode_LIT (ast_node);
                    break; }

        case RW: { ir = generateIRCode_RW (ast_node);
                   break; }

        case OP: { ir = generateIRCode_OP (ast_node);
                   break; }

        case LIST: { ir = generateIRCode_LIST (ast_node);
                     break; }

        case FUNC: { ir = generateIRCode_FUNC (ast_node);
                     break; }

        case CALL: { ir = generateIRCode_CALL (ast_node);
                     break; }


        case RET: { ir = generateIRCode_RET (ast_node);
                     break; }

        case IF_CONTROL: { ir = generateIRCode_IF_CONTROL (ast_node);
                           break; }

        case WHILE_CONTROL: { ir = generateIRCode_WHILE_CONTROL (ast_node);
                              break; }

        case FOR_CONTROL: { ir = generateIRCode_FOR_CONTROL (ast_node);
                            break; }

        default: assert(1==0); //invalid type of ast_node in ast_tree
    }

    // add data in ir to ast (NOTE: CodeObject contain's only on inst - inst created for ast_node)
    if (ir != NULL) {
        ast_node->addCodeObjectType(ir->getType());
        ast_node->addCodeObjectDest(ir->getDest());
        if (ast_type != VAR) {
            list<string> * ir_inst = ir->getCode(); // this will list only for LIST (all other cases only one inst)
            ast_node->addCodeObjectCode(ir_inst);
        }
    }
    delete ir;
}

// API to generate IRCode for VAR
CodeObject * generateIRCode_VAR (ASTNode * ast_node) {
    CodeObject * ir = new CodeObject();

    // read variable from ASTNode
    vector<string> data = ast_node->getData();
    assert (data.size() == 2); // VAR data should be Id, Type
    string dest = data[0];
    string type = data[1];
    assert (type != "na"); // each variable should have a type defined before ir-code-gen

    // fill CodeObject - no code generated for VAR
    ir->addType(type);
    ir->addDest(dest);
    assert ((ir->getCode())->empty()); // ir returned should only have 0 inst
    return ir;
}

// API to generate IRCode for LIT
CodeObject * generateIRCode_LIT (ASTNode * ast_node) {
    CodeObject * ir = new CodeObject();

    // read literal from ASTNode
    vector<string> data = ast_node->getData();
    assert (data.size() == 2); // LIT data should be Id, Type
    string lit_val = data[0];
    string type = data[1];
    string dest = get_temporary();
    string opcode = "STORE";
    // decode int/float
    assert (type=="INT" || type=="FLOAT" || type=="STRING"); // type is invalid
    opcode = (type == "INT") ? opcode+"I" : ((type == "FLOAT") ? opcode+"F" : opcode+"S");

    // construct instruction
    string inst = opcode + " " + lit_val + " " + dest;

    // fill CodeObject - no code generated for LIT
    ir->addCode(inst);
    ir->addType(type);
    ir->addDest(dest);
    assert ((ir->getCode())->size() == 1); // ir returned should only have 1 inst
    return ir;
}

// API to generate IRCode for RW
CodeObject * generateIRCode_RW (ASTNode * ast_node) {
    CodeObject * ir = new CodeObject();

    // read dest of left child node
    ASTNode * child_l = ast_node->getLeft();
    string dest = child_l->getCodeObjectDest(); // --> part of 3AC code
    string type = child_l->getCodeObjectType();
    assert (ast_node->getRight() == NULL); // RW nodes have only left child

    // read operator and construct instruction
    vector<string> data = ast_node->getData();
    assert (data.size() == 1); // RW data should be operator (r/w)
    string op = data[0];
    string opcode; // --> part of 3AC code (opcode based on op and type)
    string inst;

    if (op == "r") opcode = "READ";
    else opcode = "WRITE";
    // decode int/float
    assert (type=="INT" || type=="FLOAT" || type=="STRING"); // type is invalid
    opcode = (type == "INT") ? opcode+"I" : ((type == "FLOAT") ? opcode+"F" : opcode+"S");
    inst = opcode + " " + dest;

    // add instruction, type, dest to CodeObject
    ir->addCode(inst);
    ir->addType(type);
    ir->addDest(dest);
    assert ((ir->getCode())->size() == 1); // ir returned should only have 1 inst
    return ir;
}

// API to generate IRCode for OP
CodeObject * generateIRCode_OP (ASTNode * ast_node) {
    CodeObject * ir = new CodeObject();

    // read dest/type of left and right child nodes
    ASTNode * child_l = ast_node->getLeft();
    ASTNode * child_r = ast_node->getRight();
    string dest_l = child_l->getCodeObjectDest(); // --> part of 3AC code
    string type_l = child_l->getCodeObjectType();
    string dest_r = child_r->getCodeObjectDest(); // --> part of 3AC code
    string type_r = child_r->getCodeObjectType();
    if (child_r->getASTType() != CALL)
        assert (type_l == type_r); // type checking within expression is invalid

    // add IRcode from lower-level nodes to CodeObject
    addIRCodefromListtoList (ast_node, child_l);
    addIRCodefromListtoList (ast_node, child_r);

    // read operator and construct instructions
    vector<string> data = ast_node->getData();
    assert (data.size() == 1); // OP data should be operator
    string op = data[0];
    string opcode; // --> part of 3AC code (opcode based on op and type)
    string inst;
    string dest;

    if (op == ":=") {
        dest = dest_l;
        opcode = "STORE";
        // decode int/float
        assert (type_l=="INT" || type_l=="FLOAT" || type_l=="STRING"); // type is invalid
        opcode = (type_l == "INT") ? opcode+"I" : ((type_l == "FLOAT") ? opcode+"F" : opcode+"S");

        // Generate two instructions for VAR/LIT right childs - to match IR on website
        astTypes_t ast_type = ast_node->getRight()->getASTType();
        if (!(ast_type ==  VAR || ast_type == LIT)) { // Case1 - right child is not a variable/lit
            inst = opcode + " " + dest_r + " " + dest;
            ir->addCode(inst);
        }
        else { // Case2 - right child is a variable/lit - add two isntructions
            string inst1, inst2;
            string dest_temp = get_temporary();
            inst1 = opcode + " " + dest_r + " " + dest_temp;
            inst2 = opcode + " " + dest_temp + " " + dest_l;
            ir->addCode(inst1);
            ir->addCode(inst2);
        }

        // add instruction, type, dest to CodeObject
        ir->addType(type_l);
        ir->addDest(dest);
        //assert ((ir->getCode())->size() == 1); // ir returned should only have 1 inst
    }
    else {
        // decode opcode
        if (op == "+")      opcode = "ADD";
        else if (op == "-") opcode = "SUB";
        else if (op == "*") opcode = "MUL";
        else if (op == "/") opcode = "DIV";
        else if (op == "=") opcode = "NE"; // generates complement for all conditional operators
        else if (op == "!=") opcode = "EQ";
        else if (op == "<") opcode = "GE";
        else if (op == ">") opcode = "LE";
        else if (op == "<=") opcode = "GT";
        else if (op == ">=") opcode = "LT";
        else assert (1==0); // invalid operator
        // decode int/float
        assert (type_l=="INT" || type_l=="FLOAT" || type_l=="STRING"); // type is invalid
        opcode = (type_l == "INT") ? opcode+"I" : ((type_l == "FLOAT") ? opcode+"F" : opcode+"S");
        if (op=="+" || op=="-" || op=="*" || op=="/") {
            dest = get_temporary();
            inst = opcode + " " + dest_l + " " + dest_r  + " " + dest;
            ir->addCode(inst);
            ir->addType(type_l);
            ir->addDest(dest);
        }
        else {
            // andle cases with two variables
            astTypes_t ast_typeL = ast_node->getLeft()->getASTType();
            astTypes_t ast_typeR = ast_node->getRight()->getASTType();
            string dest_temp = dest_r;
            string inst_temp;
            string opcode_temp = "STORE";
            opcode_temp = (type_r == "INT") ? opcode_temp+"I" : ((type_r == "FLOAT") ? opcode_temp+"F" : opcode_temp+"S");
            if ((ast_typeL == VAR) && (ast_typeR == VAR)) {
                dest_temp = get_temporary();
                inst_temp = opcode_temp + " " + dest_r + " " + dest_temp;
                ir->addCode(inst_temp);

            }
            inst = opcode + " " + dest_l + " " + dest_temp + " ";
            ir->addCode(inst);
            ir->addType(type_l);
            ir->addDest(dest);

        }
        // add instruction, type, dest to CodeObject
        //ir->addCode(inst);
        //ir->addType(type_l);
        //ir->addDest(dest);
        //assert ((ir->getCode())->size() == 1); // ir returned should only have 1 inst
    }

    return ir;
}

// API to generate IRCode for LIST
CodeObject * generateIRCode_LIST (ASTNode * ast_node) {
    // the CodeObject for expr-list won't have a type or dest (only code)
    CodeObject * ir = new CodeObject();

    // get list of ASTs in ast_node and generate code for each AST-node in the list
    // add ir-code of each AST (root-node) to codeobject
    list<ASTNode *> * expr_list = ast_node->getList();
    for (list<ASTNode *>::iterator it=(*expr_list).begin(); it!=(*expr_list).end(); it++) {
        //print_ast(*it);
        //cout << endl;
        PostTraverseAST(*it);
        ir->addCode((*it)->getCodeObjectCode());
    }
    return ir;
}


// API to generate IRCode for FUNC_DECL
CodeObject * generateIRCode_FUNC (ASTNode * ast_node) {
    // the CodeObject won't have a type or dest (only code)
    CodeObject * ir = new CodeObject();

    // generate function header/footer
    string label_inst, link_inst, ret_inst;

    //// aading code for functional call
    //string push_inst, pushreg_inst, jsr_inst, halt_inst;
    //push_inst.assign("PUSH");
    //pushreg_inst.assign("PUSHREGS");
    //jsr_inst.assign("JSR FUNC_");
    //halt_inst.assign("HALT");

    label_inst.assign("LABEL FUNC_");
    link_inst.assign("LINK ");
    //ret_inst.assign("RET");

    vector<string> data = ast_node->getData();
    label_inst += data[0];
    link_inst += data[1];
    //jsr_inst+=data[0];

    // aading code for functional call
    //ir->addCode (push_inst);
    //ir->addCode (pushreg_inst);
    //ir->addCode (jsr_inst);
    //ir->addCode (halt_inst);

    ir->addCode (label_inst); // evalutaion of condition
    ir->addCode (link_inst); // evalutaion of condition

    ASTNode * expr_list_ast = ast_node->getLeft();
    ir->addCode(expr_list_ast->getCodeObjectCode());

    // add unlink return of not the end of program
    list<string> * code = ir->getCode();
    if (code->back() != "RET") {
        ir->addCode("UNLINK");
        ir->addCode("RET");
    }

    return ir;
}


// API to generate IRCode for FUNC_CALL
CodeObject * generateIRCode_CALL (ASTNode * ast_node) {
    // the CodeObject won't have a type or dest (only code)
    CodeObject * ir = new CodeObject();

    // get list of ASTs in ast_node's param_list and generate code for each AST-node in the list
    // add ir-code of each AST (root-node) to codeobject
    list<ASTNode *> * param_list = ast_node->getList();
    for (list<ASTNode *>::iterator it=(*param_list).begin(); it!=(*param_list).end(); it++) {
        PostTraverseAST(*it);
        ir->addCode((*it)->getCodeObjectCode());
    }

    string push_inst, pop_inst, jsr_inst;

    // generate push for header and function parameters
    push_inst.assign("PUSH");
    ir->addCode (push_inst);
    ir->addCode (push_inst+"REG");
    for (list<ASTNode *>::iterator it=(*param_list).begin(); it!=(*param_list).end(); it++) {
        string temp_inst = push_inst + " " + (*it)->getCodeObjectDest();
        ir->addCode (temp_inst);
    }

    // generate jsr for function call
    jsr_inst.assign("JSR FUNC_");
    vector<string> data = ast_node->getData();
    jsr_inst+=data[0];
    ir->addCode (jsr_inst);

    // generate pop for footer and function return
    pop_inst.assign("POP");

    // generate same number of pops as puses above
    for (int i=0; i<(param_list->size()); i++)
        ir->addCode (pop_inst);

    ir->addCode (pop_inst+"REG");

    string dest = get_temporary();
    ir->addCode(pop_inst + " " + dest);
    ir->addDest(dest);

    return ir;
}


// API to generate IRCode for FUNC_RET
CodeObject * generateIRCode_RET (ASTNode * ast_node) {
    // the CodeObject won't have a type or dest (only code)
    CodeObject * ir = new CodeObject();

    // read dest/type of right child node
    //ASTNode * child_l = ast_node->getLeft();
    //assert (child_l == NULL);

    ASTNode * child_r = ast_node->getRight();
    string dest = child_r->getCodeObjectDest(); // --> part of 3AC code
    string type = child_r->getCodeObjectType();

    addIRCodefromListtoList (ast_node, child_r);

    string opcode = "STORE";
    assert (type=="INT" || type=="FLOAT" || type=="STRING"); // type is invalid
    opcode = (type == "INT") ? opcode+"I" : ((type == "FLOAT") ? opcode+"F" : opcode+"S");
    string st_inst = opcode +  " " + dest + " " + (ast_node->getData())[0];
    ir->addCode (st_inst); // label foo start of else

    string unlink_inst = "UNLINK";
    ir->addCode (unlink_inst); // label foo start of else

    string ret_inst = "RET";
    ir->addCode (ret_inst); // label foo start of else

    return ir;
}


// API to generate IRCode for IF/ELSE
CodeObject * generateIRCode_IF_CONTROL (ASTNode * ast_node) {
    CodeObject * ir = new CodeObject();

    // code-gen for then-expr-list and else-expr-list already finish
    // before if-ast (NOTE :post-order traversal)
    // code-gen for cond-expr
    ASTNode * cond_ast = ast_node->getCondNode();
    PostTraverseAST (cond_ast);

    // stitch together cond, then, else with label and jumps
    vector<string> label_vec = get_if_else_label();
    string label_inst, uncond_jump_inst;
    label_inst.assign("LABEL ");
    uncond_jump_inst.assign("JUMP ");

    // remove last instrn of condExpr, form conditional jump
    list<string> * cond_code = cond_ast->getCodeObjectCode();
    string cond_jump_inst = cond_code->back();
    cond_code->pop_back();
    ir->addCode (cond_code); // evalutaion of condition

    cond_jump_inst += label_vec[0];
    ir->addCode (cond_jump_inst); // conditional jump

    ASTNode * then_list_ast = ast_node->getLeft();
    ir->addCode(then_list_ast->getCodeObjectCode());

    uncond_jump_inst += label_vec[1];
    ir->addCode (uncond_jump_inst); // unconditional jump

    string else_label_inst = label_inst + label_vec[0];
    ir->addCode (else_label_inst); // label foo start of else

    // not every if, has an else
    ASTNode * else_list_ast = ast_node->getRight();
    if (else_list_ast != NULL) {
        ir->addCode(else_list_ast->getCodeObjectCode());
    }

    string end_label_inst = label_inst + label_vec[1];
    ir->addCode (end_label_inst); // label foo end of if_else block

    return ir;
}

// API to generate IRCode for WHILE
CodeObject * generateIRCode_WHILE_CONTROL (ASTNode * ast_node) {
    CodeObject * ir = new CodeObject();

    // code-gen for cond-expr
    ASTNode * cond_ast = ast_node->getCondNode();
    PostTraverseAST (cond_ast);

    // stitch together cond, then with label and jumps
    vector<string> label_vec = get_while_label();
    string label_inst, uncond_jump_inst;
    label_inst.assign("LABEL ");
    uncond_jump_inst.assign("JUMP ");

    string start_label_inst = label_inst + label_vec[0];
    ir->addCode (start_label_inst); // label for start of while

    // remove last instrn of condExpr, form conditional jump
    list<string> * cond_code = cond_ast->getCodeObjectCode();
    string cond_jump_inst = cond_code->back();
    cond_code->pop_back();
    ir->addCode (cond_code); // evalutaion of condition

    cond_jump_inst += label_vec[1];
    ir->addCode (cond_jump_inst); // conditional jump

    ASTNode * then_list_ast = ast_node->getLeft();
    ir->addCode(then_list_ast->getCodeObjectCode());

    uncond_jump_inst += label_vec[0];
    ir->addCode (uncond_jump_inst); // unconditional jump

    string end_label_inst = label_inst + label_vec[1];
    ir->addCode (end_label_inst); // label foo end of if_else block

    return ir;
}

// API to generate IRCode for FOR
CodeObject * generateIRCode_FOR_CONTROL (ASTNode * ast_node) {
    CodeObject * ir = new CodeObject();

    // code-gen for init-expr
    ASTNode * init_ast = ast_node->getInitNode();
    PostTraverseAST (init_ast);
    list<string> * init_code = init_ast->getCodeObjectCode();
    ir->addCode (init_code);

    // code-gen for cond-expr
    ASTNode * cond_ast = ast_node->getCondNode();
    PostTraverseAST (cond_ast);

    // stitch together cond, then with label and jumps
    vector<string> label_vec = get_for_label();
    string label_inst, uncond_jump_inst;
    label_inst.assign("LABEL ");
    uncond_jump_inst.assign("JUMP ");

    string start_label_inst = label_inst + label_vec[0];
    ir->addCode (start_label_inst); // label for start of for

    // remove last instrn of condExpr, form conditional jump
    list<string> * cond_code = cond_ast->getCodeObjectCode();
    string cond_jump_inst = cond_code->back();
    cond_code->pop_back();
    ir->addCode (cond_code); // evalutaion of condition

    cond_jump_inst += label_vec[2];
    ir->addCode (cond_jump_inst); // conditional jump

    ASTNode * then_list_ast = ast_node->getLeft();
    ir->addCode(then_list_ast->getCodeObjectCode());

    // Add increment for FOR loops
    ASTNode * incr_ast = ast_node->getIncrNode();
    PostTraverseAST (incr_ast);
    list<string> * incr_code = incr_ast->getCodeObjectCode();
    ir->addCode (incr_code);

    uncond_jump_inst += label_vec[0];
    ir->addCode (uncond_jump_inst); // unconditional jump

    string end_label_inst = label_inst + label_vec[2];
    ir->addCode (end_label_inst); // label foo end of for block

    return ir;

}

// API to generate fresh labels
vector<string> get_if_else_label () {
    static int label_count = 0;
    char else_label[20];
    char end_label[20];
    sprintf(else_label, "ELSE_%d", label_count);
    sprintf(end_label, "END_IF_ELSE_%d", label_count++);

    vector<string> label_vec;
    label_vec.push_back(else_label);
    label_vec.push_back(end_label);
    return label_vec;
}

// API to generate fresh labels
vector<string> get_while_label () {
    static int label_count = 0;
    char start_label[20];
    char end_label[20];
    sprintf(start_label, "WHILE_%d", label_count);
    sprintf(end_label, "END_WHILE_%d", label_count++);

    vector<string> label_vec;
    label_vec.push_back(start_label);
    label_vec.push_back(end_label);
    return label_vec;
}

// API to generate fresh labels
vector<string> get_for_label () {
    static int label_count = 0;
    char start_label[20];
    char inc_label[20];
    char end_label[20];
    sprintf(start_label, "FOR_%d", label_count);
    sprintf(inc_label, "INC_FOR_%d", label_count);
    sprintf(end_label, "END_FOR_%d", label_count++);

    vector<string> label_vec;
    label_vec.push_back(start_label);
    label_vec.push_back(inc_label);
    label_vec.push_back(end_label);
    return label_vec;
}

// API to print declarations before assembly code
void print_decl (symbolTable_node * curr_scope) {
    list<var_entry*> * var_temp = curr_scope->getEntries();
    for (list<var_entry*>::iterator it=(*var_temp).begin(); it!=(*var_temp).end(); it++) {
        string type = (*it)->getType();
        if (type == "STRING")
            cout << "str " << (*it)->getId() << " " <<(*it)->getValue() << endl;

        else if (type == "INT" || type == "FLOAT")
            cout << "var " << (*it)->getId() << endl;
    }
}


// API to create function activation record
map<string, string> * create_activation_record (list<var_entry*> * param_list, list<var_entry*> * local_list) {
    int num_param = param_list->size();
    int num_local = local_list->size();

    // map to representa activation record
    map<string, string> * ar = new map<string, string>;
    char slotId[20];

    int param_count = 0;
    for (list<var_entry*>::iterator it=(*param_list).begin(); it!=(*param_list).end(); it++) {
        sprintf(slotId, "$%d", (1+(num_param-param_count)));
        ar->insert(pair<string, string>((*it)->getId(), slotId));
        param_count++;
    }

    int local_count = 0;
    for (list<var_entry*>::iterator it=(*local_list).begin(); it!=(*local_list).end(); it++) {
        sprintf(slotId, "$-%d", (1+local_count));
        ar->insert(pair<string, string>((*it)->getId(), slotId));
        local_count++;
    }

    //for (map<string, string>::iterator it=(*ar).begin(); it!=(*ar).end(); it++)
    //    cout << (*it).first << " "  << (*it).second << endl;
    return ar;
}

