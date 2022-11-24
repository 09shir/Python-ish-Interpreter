
#ifndef _SCANNER_H_

#define _SCANNER_H_


#include <iostream>
#include <string>

using namespace std;


// Desc:  TokenType identifies the token
//
// keywords:  and, or, not, print, if, while
//            (elif, else, break - in extensions only)
//  symbols:  =, +, -, *, /, (, ), <, >, ==, !=, <=, >=, {, }, ;
// literals:  integer, ident (must begin with _[a-z][A-Z])
//  special:  eof, err
typedef enum { whiletok, iftok, eliftok, elsetok, printtok, breaktok, asgntok, pltok, mitok, asttok, slashtok, lptok, rptok, lttok, gttok, eqtok, netok, letok, getok, ortok, andtok, nottok, lctok, rctok, sctok, ident, integer, errtok, eof }
TokenType ;


string toktotext(TokenType tt);
ostream &operator<<(ostream &lhs, TokenType &rhs);




////////////////////////////////////////////////////////////
//     -you can ignore everything beyond this point-
////////////////////////////////////////////////////////////




class Token {
    public:
        TokenType tt;
        int line;  // first line of input is line 1
        string text;
};


class Scanner {
    private:
        int line;
        istream *str;
        char buf[2];
    public:
        Scanner(istream &str);
        Token getnext();
};


#endif // _SCANNER_H_
