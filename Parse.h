
#pragma once
#include "Scanner.h"
#include <string>
#include <iostream>


class ExpnNode;   // read declaration below
class StmtNode;   // read declaration below
class StmtsNode;  // read declaration below


StmtsNode *Parse(istream &str);


// Desc:  A node in an expression tree.  
//  Inv:  All internal nodes have two children, unless the 
//        operator is unary minus or unary not, in which 
//        case they only have the right child.
//  Inv:  All leaf nodes are either identifiers or integers,
//        i.e., tok == ident || tok == integer
class ExpnNode {
    public:
        ExpnNode *left_operand, *right_operand;
        TokenType tok;  
        string text;




        /////////////////////////////////////////////////////////////
        //  you will not need to call any of the methods shown below
        /////////////////////////////////////////////////////////////



        // Desc:  constructor for leaf node (integer or identifier)
        ExpnNode(TokenType tok, string val) { left_operand = NULL; right_operand = NULL; this->tok = tok; text = val; }
        // Desc:  constructor for interior node
        ExpnNode(ExpnNode *left, ExpnNode *right, TokenType op) { left_operand = left; right_operand = right; tok = op; text = ""; }
        // Desc:  recursive destructor
        ~ExpnNode() { if (left_operand != NULL) delete left_operand; if (right_operand != NULL) delete right_operand; }
};



// Desc:  A node in a parse tree, representing a single statement.  
class StmtNode {
    public:
        TokenType tok;     //  one of: printtok, asgntok, whiletok, 
                           //          iftok, eliftok, elsetok, breaktok
        string ident;      //  equals "" unless tok == asgntok
        ExpnNode *expn;    //  NULL if tok == breaktok
        StmtsNode *stmts;  //  NULL if tok == printtok, asgntok or breaktok
        StmtNode *elif;    //  NULL unless tok == iftok or eliftok




        /////////////////////////////////////////////////////////////
        // you will not need to call any of the methods shown below
        /////////////////////////////////////////////////////////////



        // Desc:  constructor 
        StmtNode(TokenType tok, string ident, ExpnNode *expn, StmtsNode *stmts, StmtNode *elif) { this->tok = tok; this->ident = ident; this->expn = expn; this->stmts = stmts; this->elif = elif; }
        // Desc:  recursive destructor
        ~StmtNode(); // { if (expn != NULL) delete expn; if (stmts != NULL) delete stmts; if (elif != NULL) delete elif; }
};



// Desc:  A node in a parse tree, representing a sequence of statements.  
//  Inv:  ->stmt is the first statement
//  Inv:  ->stmts is the statements following the first
class StmtsNode {
    public:
        StmtNode *stmt;    // first of a list of instructions, never NULL
        StmtsNode *stmts;  // remainder of list of instructions, NULL if empty




        /////////////////////////////////////////////////////////////
        // you will not need to call any of the methods shown below
        /////////////////////////////////////////////////////////////



        // Desc:  constructor 
        StmtsNode(StmtNode *stmt, StmtsNode *stmts) { this->stmt = stmt; this->stmts = stmts; }
        // Desc:  recursive destructor
        ~StmtsNode() { if (stmt != NULL) delete stmt; if (stmts != NULL) delete stmts; }
};


