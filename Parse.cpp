
#include "Parse.h"
#include <iostream>

using namespace std;

void match(TokenType tt);
StmtsNode *ParseS();
StmtsNode *ParseStmts();
StmtNode *ParseStmt();
ExpnNode *ParseE();
ExpnNode *ParseLAE();
ExpnNode *ParseRE();
ExpnNode *ParseAE();
ExpnNode *ParseT();
ExpnNode *ParseF();


Scanner *S;
Token nextToken;


StmtsNode *Parse(istream &str) {
    S = new Scanner(str);
    nextToken = S->getnext();
    StmtsNode *ret = ParseS();
    delete S;
    return ret;
}


///////////////////////////////////////////////////////////////
//                     -descent parsing-
///////////////////////////////////////////////////////////////


void match(TokenType tt) {
    if (nextToken.tt != tt) {
        char buf[64];
        sprintf(buf, "%d", nextToken.line);
        throw("Expected " + toktotext(tt) + " but scanned " + toktotext(nextToken.tt) + " on line " + buf);
    }
    nextToken = S->getnext();
}


StmtsNode *ParseS() {
    StmtsNode *ret = ParseStmts();
    match(eof);
    return ret;
}


StmtsNode *ParseStmts() {
    //------------------------------------------------
    //  <Stmts> -> <Stmt>*
    //  strategy: create a linked list of StmtNode *
    //------------------------------------------------
    StmtsNode ret(NULL, NULL);
    StmtsNode *cur = &ret;
    while ((nextToken.tt == printtok) || (nextToken.tt == ident) || (nextToken.tt == whiletok) || (nextToken.tt == iftok) || (nextToken.tt == breaktok)) {
        cur->stmts = new StmtsNode(ParseStmt(), NULL);
        cur = cur->stmts;
    }
    cur = ret.stmts;
    ret.stmts = NULL;
    return cur;
}


StmtNode *ParseStmt() {
    if (nextToken.tt == printtok) {
        //------------------------
        //  <Stmt> -> print <E> ;
        //------------------------
        match(printtok);
        ExpnNode *E = ParseE();
        match(sctok);
        return new StmtNode(printtok, "", E, NULL, NULL);
    }
    else if (nextToken.tt == ident) {
        //--------------------------
        //  <Stmt> -> ident = <E> ;
        //--------------------------
        string text = nextToken.text;
        match(ident);
        match(asgntok);
        ExpnNode *E = ParseE();
        match(sctok);
        return new StmtNode(asgntok, text, E, NULL, NULL);
    }
    else if (nextToken.tt == whiletok) {
        //----------------------------------
        //  <Stmt> -> while <E> { <Stmts> }
        //----------------------------------
        match(whiletok);
        ExpnNode *E = ParseE();
        match(lctok);
        StmtsNode *Stmts = ParseStmts();
        match(rctok);
        return new StmtNode(whiletok, "", E, Stmts, NULL);
    }
    else if (nextToken.tt == iftok) {
        //------------------------------------------------------------------------------------
        //  <Stmt> -> if <E> { <Stmts> } (elif <E> { <Stmts> })* (epsilon | else { <Stmts> })
        //------------------------------------------------------------------------------------
        match(iftok);
        ExpnNode *E = ParseE();
        match(lctok);
        StmtsNode *Stmts = ParseStmts();
        match(rctok);
        StmtNode *ret = new StmtNode(iftok, "", E, Stmts, NULL);
        StmtNode *cur = ret;
        while (nextToken.tt == eliftok) {
            match(eliftok);
            ExpnNode *E = ParseE();
            match(lctok);
            StmtsNode *Stmts = ParseStmts();
            match(rctok);
            cur->elif = new StmtNode(eliftok, "", E, Stmts, NULL);
            cur = cur->elif;
        }
        if (nextToken.tt == elsetok) {
            match(elsetok);
            match(lctok);
            StmtsNode *Stmts = ParseStmts();
            match(rctok);
            cur->elif = new StmtNode(elsetok, "", new ExpnNode(integer, "1"), Stmts, NULL);
        }
        return ret;
    }
    else if (nextToken.tt == breaktok) {
        match(breaktok); match(sctok);
        return new StmtNode(breaktok, "", NULL, NULL, NULL);
    }
}


ExpnNode *ParseE() {
    //---------------------------
    //  <E> -> <LAE> (or <LAE>)*
    //---------------------------
    ExpnNode *ret = ParseLAE();
    while(1) {
        if (nextToken.tt == ortok) {
            match(nextToken.tt);
            ret = new ExpnNode(ret, ParseLAE(), ortok);
        }
        else return ret;
    }
}


ExpnNode *ParseLAE() {
    //----------------------------
    //  <LAE> -> <RE> (and <RE>)*
    //----------------------------
    ExpnNode *ret = ParseRE();
    while(1) {
        if (nextToken.tt == andtok) {
            match(nextToken.tt);
            ret = new ExpnNode(ret, ParseRE(), andtok);
        }
        else return ret;
    }
}


ExpnNode *ParseRE() {
    //--------------------------------------
    //  <RE> -> <AE> (epsilon | <rop> <AE>)
    //--------------------------------------
    int notct = 0;
    while (nextToken.tt == nottok) {
        notct++;
        match(nottok);
    }
    ExpnNode *ret = ParseAE();
    if ((nextToken.tt == lttok) || (nextToken.tt == gttok) || (nextToken.tt == eqtok) || (nextToken.tt == netok) || (nextToken.tt == letok) || (nextToken.tt == getok)) {
        TokenType tmp = nextToken.tt;
        match(nextToken.tt);
        ret = new ExpnNode(ret, ParseAE(), tmp);
    }
    while (notct > 2) notct -= 2;
    if (notct == 1) {
        return new ExpnNode(NULL, ret, nottok);
    }
    else if (notct == 2) {
        return new ExpnNode(NULL, new ExpnNode(NULL, ret, nottok), nottok);
    }
    else {
        return ret;
    }
}


ExpnNode *ParseAE() {
    //-----------------------------
    //  <AE> -> <T> ((+ | -) <T>)*
    //-----------------------------
    ExpnNode *ret = ParseT();
    while(1) {
        if ((nextToken.tt == pltok) || (nextToken.tt == mitok)) {
            TokenType tmp = nextToken.tt;
            match(nextToken.tt);
            ret = new ExpnNode(ret, ParseT(), tmp);
        }
        else return ret;
    }
}


ExpnNode *ParseT() {
    //-----------------------------
    //  <T> -> <F> ((* | /) <F>)*
    //-----------------------------
    ExpnNode *ret = ParseF();
    while(1) {
        if ((nextToken.tt == asttok) || (nextToken.tt == slashtok)) {
            TokenType tmp = nextToken.tt;
            match(nextToken.tt);
            ret = new ExpnNode(ret, ParseF(), tmp);
        }
        else return ret;
    }
}


ExpnNode *ParseF() {
    //----------------------------------------------
    //  <F> -> (+ | -)* (integer | ident | ( <E> ))
    //----------------------------------------------
    int sign = 0;
    ExpnNode *ret;
    while (1) {
        if (nextToken.tt == pltok) { match(pltok); }
        else if (nextToken.tt == mitok) { match(mitok); sign = (sign + 1) % 2; }
        else if ((nextToken.tt == integer) || (nextToken.tt == ident)) { 
            ret = new ExpnNode(nextToken.tt, nextToken.text);
            match(nextToken.tt);
            if (sign) ret = new ExpnNode(NULL, ret, mitok);
            return ret;
        }
        else if (nextToken.tt == lptok) {
            match(lptok);
            ret = ParseE();
            match(rptok);
            if (sign) ret = new ExpnNode(NULL, ret, mitok);
            return ret;
        }
        else {
            char buf[64];
            sprintf(buf, "%d", nextToken.line);
            throw("Expected literal but scanned " + toktotext(nextToken.tt) + " on line " + buf);
        }
    }
}


StmtNode::~StmtNode() { if (expn != NULL) delete expn; if (stmts != NULL) delete stmts; if (elif != NULL) delete elif; }


