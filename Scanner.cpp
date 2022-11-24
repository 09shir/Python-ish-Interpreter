
#include "Scanner.h"



string operator+(string &lhs, char &rhs) {
    char c[2]; c[0] = rhs; c[1] = 0;
    return lhs+c;
}


string toktotext(TokenType tt) {
    if (tt == whiletok) return "while";
    else if (tt == iftok) return "if";
    else if (tt == eliftok) return "elif";
    else if (tt == elsetok) return "else";
    else if (tt == printtok) return "print";
    else if (tt == breaktok) return "break";
    else if (tt == asgntok) return "=";
    else if (tt == pltok) return "+";
    else if (tt == mitok) return "-";
    else if (tt == asttok) return "*";
    else if (tt == slashtok) return "/";
    else if (tt == lptok) return "(";
    else if (tt == rptok) return ")";
    else if (tt == lttok) return "<";
    else if (tt == gttok) return ">";
    else if (tt == eqtok) return "==";
    else if (tt == netok) return "!=";
    else if (tt == letok) return "<=";
    else if (tt == getok) return ">=";
    else if (tt == ortok) return "or";
    else if (tt == andtok) return "and";
    else if (tt == nottok) return "not";
    else if (tt == lctok) return "{";
    else if (tt == rctok) return "}";
    else if (tt == sctok) return ";";
    else if (tt == ident) return "identifier";
    else if (tt == integer) return "integer";
    else if (tt == eof) return "eof";
    else return "err";
}


ostream &operator<<(ostream &lhs, TokenType &rhs) {
    lhs << toktotext(rhs);
    return lhs;
}



Scanner::Scanner(istream &str) {
    line = 1;
    this->str = &str;
    buf[0] = buf[1] = 0;
}


int newl(char c) { return (c == '\n'); }


Token Scanner::getnext() {
    Token ret; ret.text = "";
    if (buf[0] == 0) { buf[0] = str->get(); line += newl(buf[0]); }

    // collapse whitespace
    while (isspace(buf[0]) || (buf[0] == '\n')) {
        buf[0] = str->get(); line += newl(buf[0]);
        if (str->eof()) break;
    }

    // case 1: eof    
    if (str->eof()) { ret.tt = eof; ret.line = line; ret.text = ""; return ret; }

    // case 2: identifier-  begins with a-zA-z_, may contain 0-9
    if ((buf[0] == '_') || isalpha(buf[0])) {
        ret.tt = ident; ret.line = line; ret.text = buf;
        buf[0] = str->get(); line += newl(buf[0]);
        while ((buf[0] == '_') || isalpha(buf[0]) || isdigit(buf[0])) {
            ret.text += buf;
            buf[0] = str->get(); line += newl(buf[0]);
        }
        if (isspace(buf[0]) || (buf[0] == '\n')) buf[0] = 0;
        if (ret.text == "while") ret.tt = whiletok;
        else if (ret.text == "if") ret.tt = iftok;
        else if (ret.text == "elif") ret.tt = eliftok;
        else if (ret.text == "else") ret.tt = elsetok;
        else if (ret.text == "print") ret.tt = printtok;
        else if (ret.text == "break") ret.tt = breaktok;
        else if (ret.text == "or") ret.tt = ortok;
        else if (ret.text == "and") ret.tt = andtok;
        else if (ret.text == "not") ret.tt = nottok;
        return ret;
    }


    // case 3: numerical-   [0-9]+
    if (isdigit(buf[0])) {
        ret.tt = integer; ret.line = line; ret.text = buf;
        buf[0] = str->get(); line += newl(buf[0]);
        while (isdigit(buf[0])) {
            ret.text += buf;
            buf[0] = str->get(); line += newl(buf[0]);
        }
        if (isspace(buf[0]) || (buf[0] == '\n')) buf[0] = 0;
        return ret;
    }


    // case 4: symbol- anything not covered above (which isn't whitespace)
    ret.text = buf; 
    ret.line = line; 
    if (buf[0] == '+') { ret.tt = pltok; buf[0] = 0; }
    else if (buf[0] == '-') { ret.tt = mitok; buf[0] = 0; }
    else if (buf[0] == '*') { ret.tt = asttok; buf[0] = 0; }
    else if (buf[0] == '/') { ret.tt = slashtok; buf[0] = 0; }
    else if (buf[0] == '(') { ret.tt = lptok; buf[0] = 0; }
    else if (buf[0] == ')') { ret.tt = rptok; buf[0] = 0; }
    else if (buf[0] == '{') { ret.tt = lctok; buf[0] = 0; }
    else if (buf[0] == '}') { ret.tt = rctok; buf[0] = 0; }
    else if (buf[0] == ';') { ret.tt = sctok; buf[0] = 0; }
    else if (buf[0] == '<') { buf[0] = str->get(); if (buf[0] == '=') { ret.tt = letok; ret.text += buf[0]; buf[0] = 0; } else { ret.tt = lttok; } }
    else if (buf[0] == '>') { buf[0] = str->get(); if (buf[0] == '=') { ret.tt = getok; ret.text += buf[0]; buf[0] = 0; } else { ret.tt = gttok; } }
    else if (buf[0] == '=') { buf[0] = str->get(); if (buf[0] == '=') { ret.tt = eqtok; ret.text += buf[0]; buf[0] = 0; } else { ret.tt = asgntok; } }
    else if (buf[0] == '!') { buf[0] = str->get(); if (buf[0] == '=') { ret.tt = netok; ret.text += buf[0]; buf[0] = 0; } else { ret.tt = errtok; } }
    else { ret.tt = errtok; buf[0] = 0; }
    return ret;
}


