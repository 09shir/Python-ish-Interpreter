
#include "hash.h"
#include "Scanner.h"
#include "Parse.h"
#include "assert.h"
#include <cstdlib>

#include <iostream>
using namespace std;

#define tabstop 4


// var class is for storing variables
class var {
    public:
        string key;
        int val;

        // * * * add some more attributes and methods here * * * //
        var(string k, int v): key(k), val(v) {}

};



// * * * declare more things here * * * //


Set<var> memmap;
// Desc:  Executes python code stored under the subtree below root stmts
//        provided in the parameter
int run(StmtsNode *stmts);

// Desc:  Returns 1 or 0 from logical statement used in if-statement or 
//        while-loop.
bool boolExpression(ExpnNode* node);

// Desc:  Calculates arithmetic operations that are mainly in print 
//        statements and assignment statements in Postorder traversal
string calculatePostOrder(ExpnNode *root);

// Desc:  Executes assignment/if statement and while loop
void assignment(StmtNode* root);
void ifStmt(StmtNode* root);
void whileStmt(StmtNode* root);


// Desc:  Returns 1 or 0 from logical statement used in if-statement or 
//        while-loop.
bool boolExpression(ExpnNode* root){

    // checks for nottok in front of expression
    if (root->tok == nottok){

        // checks for logical operations AND and OR
        if (root->right_operand->tok == andtok){
            return !(boolExpression(root->right_operand->left_operand) && boolExpression(root->right_operand->right_operand));
        }
        else if (root->right_operand->tok == ortok){
            return !(boolExpression(root->right_operand->left_operand) || boolExpression(root->right_operand->right_operand));
        }

        int rhs, lhs;

        // checks for nottok in front of variable
        if (root->right_operand->left_operand->tok == nottok){
            if (calculatePostOrder(root->right_operand->left_operand->right_operand) == "0"){lhs = 1;}
            else {lhs = 0;}
        }
        else{
            lhs = stoi(calculatePostOrder(root->right_operand->left_operand));
        }
        if (root->right_operand->right_operand->tok == nottok){
            if (calculatePostOrder(root->right_operand->right_operand->right_operand) == "0"){rhs = 1;}
            else {rhs = 0;}
        }
        else{
            rhs = stoi(calculatePostOrder(root->right_operand->right_operand));
        }

        cout << lhs << " " << rhs << endl;

        if (root->right_operand->tok == eqtok && !(lhs == rhs)){return true;}
        else if (root->right_operand->tok == netok && !(lhs != rhs)){return true;}
        else if (root->right_operand->tok == letok && !(lhs <= rhs)){return true;}
        else if (root->right_operand->tok == getok && !(lhs >= rhs)){return true;}
        else if (root->right_operand->tok == lttok && !(lhs < rhs)){return true;}
        else if (root->right_operand->tok == gttok && !(lhs > rhs)){return true;}
        else{
            return false;
        }
    }
    else{
        // checks for logical operations AND and OR
        if (root->tok == andtok){
            return boolExpression(root->left_operand) && boolExpression(root->right_operand);
        }
        else if (root->tok == ortok){
            return boolExpression(root->left_operand) || boolExpression(root->right_operand);
        }

        int lhs, rhs;

        // checks for nottok in front of variable
        if (root->left_operand->tok == nottok){
            if (calculatePostOrder(root->left_operand->right_operand) == "0"){lhs = 1;}
            else {lhs = 0;}
        }
        else{
            lhs = stoi(calculatePostOrder(root->left_operand));
        }
        if (root->right_operand->tok == nottok){
            if (calculatePostOrder(root->right_operand->right_operand) == "0"){rhs = 1;}
            else {rhs = 0;}
        }
        else{
            rhs = stoi(calculatePostOrder(root->right_operand));
        }

        if (root->tok == eqtok && lhs == rhs){return true;}
        else if (root->tok == netok && lhs != rhs){return true;}
        else if (root->tok == letok && lhs <= rhs){return true;}
        else if (root->tok == getok && lhs >= rhs){return true;}
        else if (root->tok == lttok && lhs < rhs){return true;}
        else if (root->tok == gttok && lhs > rhs){return true;}
        else{
            return false;
        }
    }
}

// Desc:  Calculates arithmetic operations that are mainly in print 
//        statements and assignment statements in Postorder traversal
string calculatePostOrder(ExpnNode *root){

    // detects if negative sign is presented, if so return the negative value of its child
    if ((root->tok == mitok) && (root->left_operand == nullptr)){

        ExpnNode* tmpNode = root->right_operand;
        int neg = -1;

        // detects if there's another negative sign
        if (tmpNode->tok == mitok && tmpNode->left_operand == nullptr){
            neg = 1;
            tmpNode = tmpNode->right_operand;
        }
        if (isdigit((tmpNode->text)[0])){
            int ret = stoi(tmpNode->text)*neg;
            return to_string(ret);
        }
        else{
            var* tmp = memmap.search(tmpNode->text);
            // exception handling: throws run-time error when variable is not initialized but is
            // used in arithmetic expression
            if (tmp == nullptr){
                throw runtime_error("variable not initialized");
            }
            int ret = (tmp->val)*neg;
            return to_string(ret);
        }
    }

    // terminate statement
    if ((root->left_operand == nullptr) && (root->right_operand == nullptr)){
        if (isdigit((root->text)[0]) || (root->text)[0] == '-'){    // if 1st character is num or neg sign
            return root->text;
        }
        else{
            var* tmp = memmap.search(root->text);
            // exception handling: throws run-time error when variable is not initialized but is
            // used in arithmetic expression
            if (tmp == nullptr){
                throw runtime_error("variable not initialized");
            }
            return to_string(tmp->val);
        }
    }

    // PostOrder traversal
    string left_num = calculatePostOrder(root->left_operand);
    string right_num = calculatePostOrder(root->right_operand);

    // string to int convertion
    int left_int, right_int;

    // checks if left node is a variable
    if (isdigit(left_num[0]) || left_num[0] == '-'){
        left_int = stoi(left_num);
    }
    else{
        var* tmp = memmap.search(root->left_operand->text);
        // exception handling: throws run-time error when variable is not initialized but is
        // used in arithmetic expression
        if (tmp == nullptr){
            throw runtime_error("variable not initialized");
        }
        left_int = tmp->val;
    }

    // checks if right node is a variable
    if (isdigit(right_num[0]) || right_num[0] == '-'){
        right_int = stoi(right_num);
    }
    else{
        var* tmp = memmap.search(root->left_operand->text);
        // exception handling: throws run-time error when variable is not initialized but is
        // used in arithmetic expression
        if (tmp == nullptr){
            throw runtime_error("variable not initialized");
        }
        right_int = tmp->val;
    }

    // determines which arithmetic operation to use and returns the computed the answer in string format

    // implements Short-Circuit Evaluation
        // during multiplication, when one of lhs/rhs evaluates 0, return 0
        // during division, when lhs = 0, return 0
        // logic gates AND OR are implemented elsewhere 

    // two additional math tricks that I found:
        // during addition, when lhs = 0, return rhs; when rhs = 0, return lhs
        // during subtraction, when rhs = 0, return lhs

    if (root->tok == pltok){
        if (left_int == 0){return to_string(right_int);}
        else if (right_int == 0){return to_string(left_int);}
        else {return to_string(left_int + right_int);}
    }
    else if (root->tok == mitok){
        if (right_int == 0){return to_string(left_int);}
        else {return to_string(left_int - right_int);}
    }
    else if (root->tok == asttok){
        if (left_int == 0 || right_int == 0){return to_string(0);}
        else {return to_string(left_int * right_int);}
    }
    else if (root->tok == slashtok){
        // exception handling: throws run-time error when 0 is the denominotor 
        if (right_int == 0){throw runtime_error("0 cannot be the denominator during a division");} 
        else if (left_int == 0){return to_string(0);}
        else {
            if (left_int / right_int < 0) {return to_string(left_int / right_int - 1);}
            else {return to_string(left_int / right_int);}
        }
    }
    else {
        return root->text;
    }
}
// Desc:  Executes assignment statement
void assignment(StmtNode* root){
    string rhs = calculatePostOrder(root->expn);
    var* lhs = new var(root->ident,stoi(rhs));
    memmap.insert(lhs);

}

// Desc:  Executes if statement
void ifStmt(StmtNode* root){

    // detects nottok
    if (root->expn->tok == nottok){
        if (!boolExpression(root->expn->right_operand)){
            run(root->stmts);
        }
    }
    else{
        if (boolExpression(root->expn)){
            run(root->stmts);
        }
    }
}

// Desc:  Executes while loop
void whileStmt(StmtNode* root){

    // detects nottok
    if (root->expn->tok == nottok){
        while (!boolExpression(root->expn->right_operand)){
            run(root->stmts);
        }
    }
    else{
        while (boolExpression(root->expn)){
            run(root->stmts);
        }
    }
}

// Desc:  Executes python code stored under the subtree below root stmts
//        provided in the parameter
int run(StmtsNode *stmts) {

    StmtsNode* cur = stmts;

    while(cur != nullptr){
    
        if (cur->stmt->tok == printtok) {
            
            // detects if printing a true/false statement
            if (cur->stmt->expn->tok == nottok){
                cout << boolExpression(cur->stmt->expn->right_operand) << endl;
            }
            // determine if it consists an AND/OR logical operations
            else if (cur->stmt->expn->tok == andtok){
                cout << (boolExpression(cur->stmt->expn->left_operand) && boolExpression(cur->stmt->expn->right_operand)) << endl;
            }
            else if (cur->stmt->expn->tok == ortok){
                cout << (boolExpression(cur->stmt->expn->left_operand) || boolExpression(cur->stmt->expn->right_operand)) << endl;
            }
            // determine if it consists an arithmetic symbol
            else if (cur->stmt->expn->tok == eqtok || cur->stmt->expn->tok == netok || cur->stmt->expn->tok == letok || cur->stmt->expn->tok == getok || cur->stmt->expn->tok == lttok || cur->stmt->expn->tok == gttok){
                cout << boolExpression(cur->stmt->expn) << endl;
            }
            // if not then it's printing an arithmetic expression result
            else{
                cout << calculatePostOrder(cur->stmt->expn) << endl;
            }
        }
        else if (cur->stmt->tok == asgntok) {
            assignment(cur->stmt);
        }
        else if (cur->stmt->tok == iftok) {
            ifStmt(cur->stmt);
        }
        else if (cur->stmt->tok == whiletok) {
            whileStmt(cur->stmt);
        }
        
        cur = cur->stmts;
    }

    return 0;
}

///////////////////////////////////////////////
//----- carried over from testParse.cpp -----//
///////////////////////////////////////////////

// Desc:  These 3 functions return a string which is a pretty-printed
//        version of the subtree rooted there.
string printE(ExpnNode *root);
string printStmts(StmtsNode *root, int ilevel);
string print(StmtsNode *root);


// Desc:  Generate pretty printed pish of the parse tree rooted at root.
string print(StmtsNode *root) {
    return printStmts(root, 0);  // call helper
}


// Desc:  Generate pretty printed pish, by traversing like a linked list.
//        Parameter ilevel is the indentation level.
string printStmts(StmtsNode *root, int ilevel) {
    // generate leading tab
    string spc = "", tab = "";
    for (int i = 0; i < tabstop; i++) spc += " ";
    for (int i = 0; i < ilevel; i++) tab += spc;


    // traverse StmtsNode * like it is a linked list
    StmtsNode *cur = root;
    string accum = "";  // pretty-printed version


    while (cur != NULL) {
        assert(cur->stmt != NULL);

        // print statement
        if (cur->stmt->tok == printtok) {
            accum += tab + toktotext(printtok) + " " + printE(cur->stmt->expn) + toktotext(sctok) + "\n";
        }
        // assignment statement
        else if (cur->stmt->tok == asgntok) {
            accum += tab + cur->stmt->ident + " " + toktotext(asgntok) + " " + printE(cur->stmt->expn) + toktotext(sctok) + "\n";
        }
        // while statement: tab level increases for subStmts
        else if (cur->stmt->tok == whiletok) {
            accum += tab + toktotext(whiletok) + " " + printE(cur->stmt->expn) + " " + toktotext(lctok) + "\n" + printStmts(cur->stmt->stmts, ilevel+1) + tab + toktotext(rctok) + "\n";
        }
        // if/elif/else statement: tab level increases for subStmts
        else if (cur->stmt->tok == iftok) {
            StmtNode *elif = cur->stmt;
            while (elif != NULL) {
                accum += tab + toktotext(elif->tok) + ((elif->tok == elsetok) ? "" : (" " + printE(elif->expn))) + " " + toktotext(lctok) + "\n" + printStmts(elif->stmts, ilevel+1) + tab + toktotext(rctok) + "\n";
                elif = elif->elif;
            }
        }
        // break statement
        else if (cur->stmt->tok == breaktok) {
            accum += tab + toktotext(breaktok) + toktotext(sctok) + "\n";
        }
        else { assert(0); }


        // next node in the chain
        cur = cur->stmts;
    }
    return accum;
}


// Desc:  Generate bracketed infix via postorder traversal
string printE(ExpnNode *root) {
    if (root == NULL) return "";
    if (root->text != "") return root->text;
    if (root->left_operand == NULL) return "(" + toktotext(root->tok) + ((root->tok == nottok) ? " " : "") + printE(root->right_operand) + ")";
    else return "(" + printE(root->left_operand) + " " + toktotext(root->tok) + " " + printE(root->right_operand) + ")";
}


int main () {
    StmtsNode *pTree;
    try {
        pTree = Parse(cin);
    }
    catch(string s) {
        cout << "Parse error: " << s << endl;
        return 0;
    }

    cout << "Parse Successful!\n";
    run(pTree);
    cout << print(pTree); 
}

