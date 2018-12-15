/*Scanner for ECE 573 - Compilers course*/
%option noyywrap
%option nounput
    /* %option c++ */
%{
    /* need this for the call to atof() or atoi() */
    #include <stdlib.h>
    #include <math.h>
    #include "entry.hh"
    #include "microParser.hpp"
    extern char* yytext;
    extern FILE* yyin;
%}

    /*Definitions*/
DIGIT [0-9]
ID [a-zA-Z][a-zA-Z0-9]*
SPACE [ \n\t\r]

    /*Rules and Targets*/
%%
{DIGIT}+ {
        //printf ("Token Type: INTLITERAL\nValue: %s\n", yytext);
        yylval.str_val = strdup(yytext);
        return INTLITERAL;
}


({DIGIT}+\.{DIGIT}*)|(\.{DIGIT}*) {
        //printf ("Token Type: FLOATLITERAL\nValue: %s\n", yytext);
        yylval.str_val = strdup(yytext);
        return FLOATLITERAL;
}


\"[^\"]*+\" {
        //printf ("Token Type: STRINGLITERAL\nValue: %s\n", yytext);
        yylval.str_val = strdup(yytext);
        return STRINGLITERAL;

}


--.*\n /* comment isn't //printed */


PROGRAM {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return PROGRAM;
}

BEGIN {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return _BEGIN;
}

END {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return END;
}

FUNCTION {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return FUNCTION;
}

READ {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return READ;
}

WRITE {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return WRITE;
}

IF {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return IF;
}

ELSE {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return ELSE;
}

ENDIF {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return ENDIF;
}

WHILE {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return WHILE;
}

ENDWHILE {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return ENDWHILE;
}

RETURN {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return RETURN;
}

INT {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        yylval.str_val = strdup(yytext);
        return INT;
}

VOID {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return VOID;
}

STRING {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return STRING;
}

FLOAT {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        yylval.str_val = strdup(yytext);
        return FLOAT;
}

TRUE {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return TRUE;
}

FALSE {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return FALSE;
}

FOR {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return FOR;
}

ENDFOR {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return ENDFOR;
}

CONTINUE {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return CONTINUE;
}

BREAK {
        //printf ("Token Type: KEYWORD\nValue: %s\n", yytext);
        return BREAK;
}


{ID} {
        //printf ("Token Type: IDENTIFIER\nValue: %s\n", yytext);
        yylval.str_val = strdup(yytext);
        return IDENTIFIER;
}


":=" {
        //printf ("Token Type: OPERATOR\nValue: %s\n", yytext);
        return ASSIGN;
}

"+" {
        //printf ("Token Type: OPERATOR\nValue: %s\n", yytext);
        return ADD;
}

"-" {
        //printf ("Token Type: OPERATOR\nValue: %s\n", yytext);
        return SUB;
}

"*" {
        //printf ("Token Type: OPERATOR\nValue: %s\n", yytext);
        return MUL;
}

"/" {
        //printf ("Token Type: OPERATOR\nValue: %s\n", yytext);
        return DIV;
}

"=" {
        //printf ("Token Type: OPERATOR\nValue: %s\n", yytext);
        return EQL;
}

"!=" {
        //printf ("Token Type: OPERATOR\nValue: %s\n", yytext);
        return NEQL;
}

"<" {
        //printf ("Token Type: OPERATOR\nValue: %s\n", yytext);
        return LES;
}

">" {
        //printf ("Token Type: OPERATOR\nValue: %s\n", yytext);
        return GRT;
}

"(" {
        //printf ("Token Type: OPERATOR\nValue: %s\n", yytext);
        return ST_BRACE;
}

")" {
        //printf ("Token Type: OPERATOR\nValue: %s\n", yytext);
        return END_BRACE;
}
";" {
        //printf ("Token Type: OPERATOR\nValue: %s\n", yytext);
        return SEMICOLON;
}
"," {
        //printf ("Token Type: OPERATOR\nValue: %s\n", yytext);
        return COMMA;
}
"<=" {
        //printf ("Token Type: OPERATOR\nValue: %s\n", yytext);
        return LEQ;
}

">=" {
        //printf ("Token Type: OPERATOR\nValue: %s\n", yytext);
        return GEQ;
}


{SPACE}+ /* eat whitespace space, end of line, carriage return*/

%%

