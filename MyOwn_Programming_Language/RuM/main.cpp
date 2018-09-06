//
//  main.cpp
//  RuM
//
//  Created by Konrad Rauscher on 10/22/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

// NOTE1: have to include a begin and end for each program --> begin 2 + 4; end

// NOTE2: when writing RuM code, tabs dont always work as delimiters between tokens. I have sometimes had to add a space after each tab to get the code working.

// NOTE3: calls to member functions of classes that involve variables initilized in the class don't work

// NOTE4: implmented boolean, handling of runtime errors, and memory and garbage allocation

// NOTE5: lists not completed
















//  ------- IGNORE -------
// lists --> objects with member variables and indexes --> "1", "2", etc. then convert get(1) to get("1")

// test function with recursive call

// solution: add bool 'reference' to Val
// if bool is true, when leaving scope look at var table and update correct variable.

 
 // QUESTIONS:

 // Q: need to be able to display lists? so for list of lists x, x[0]
 // name and size instead
 
 // to do:
 /*
 go through all: DO THIS
 
 pass by reference: create a Val pointer, have a vector of pointers (byReference vector)
 modify fun_Table_element, function_def, function_call, obj_manip, etc...
 
 add possibliity for function_invocation in all math functions, boolean, etc.
 
 FIX: math, bool, relational need to access var values from table
 
 finish bool handling
------------ END IGNORE ----------
 */

#include <iostream>
#include <cstdlib>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <fstream>
#include <cstring>
#include <cmath>
#include <string>

#include "DataTypes.h"
#include "SymbolTables.h"
#include "Runtime_Stack.h"

using namespace std;

enum CharType{
    
    LETTER,
    DIGIT,
    LASTCHAR,
    UNKNOWN,
    SPACE,
    SINGLE_PEREN
};

//enum

//constant definitions
const int  NAMELENG = 100;      /* Maximum length of a name */
const int  MAXINPUT = 40000;     /* Maximum length of an input */
const char*   PROMPT = "RuM_interpreter> ";
const char*   PROMPT2 = "--> ";
const char  COMMENTCHAR = '%';
const char  ENDOFINPUT = '$';   // use as sentinel to mark end of input

//global variables
vector<Val> args;
bool mode_interactive = false;

int charClass;
char lexeme[NAMELENG];
char nextChar;
int lexemeLength;
//string lexemeString;
map<string, string> tokenTypes;
char userinput[MAXINPUT];
int inputleng, pos;
int enterIfScope = 0;
string currentOutput = "Output: No statements";
int lexemeStartPlace = 0;
vector<vector<string> > tokenList;
int currToken_index = 0;  //

//function definitions

int isChar(int pos);
void makeMap();
void tokenize();
void getChar();
int lex();
void addChar();
int classifyChars(int pos);
void skipBlankSpace();

//function implementations


// NEW for RuM:
//var_Table var_table_global;

Runtime_Stack runtimeStack;


/*********************************
 LOW-LEVEL HELPER METHODS FOR READING INPUT
 *********************************/
void nextchar(char& c) // pulls next char
{
    scanf("%c", &c);
    if (c == COMMENTCHAR)
    {
        while (c != '\n')
            scanf("%c", &c);
    }
}

// skips over blanks
int skipblanks(int p)
{
    while (userinput[p] == ' ')
        ++p;
    return p;
}

int isDelim(char c) // checks char for particular delimiters
{
    return ((c == '(') || (c == ')') || (c == ' ') || (c == COMMENTCHAR) || (c == '\n'));
}

int matches(int s, int leng, char* nm) // lookahead checks input for matching string. Helps to check for keywords.
{
    int i = 0;
    int tester = 0;
    
    while (i < leng)
    {
        
        if (userinput[s] != nm[i]) {
            
            //  cout << nm << endl;
            //            cout << userinput[s] << endl;
            return 0;
        }
        ++i;
        ++s;
    }
    if (!isDelim(userinput[s]))
        return 0;
    
    return 1;
}/* matches */

int matches(int s, int leng, char* nm, bool testing) // lookahead checks input for matching string. Helps to check for keywords.
{
    int i = 0;
    int tester = 0;
    
    while (i < leng)
    {
        if (userinput[s] != nm[i]) {
            return 0;
        }
        ++i;
        ++s;
    }
    
    
    return 1;
}/* matches */


// assures that scanf waits until matching paren {  } is input. Tokenizer will not begin until a match is found.
// This function is not necessary but allows for easy input of large blocks of code
void readParens()
{
    int parencnt; /* current depth of parentheses */
    char c = ' ';
    parencnt = 1; // '(' just read
    do
    {
        if (c == '\n')
            cout << PROMPT2;
        nextchar(c);
        pos++;
        if (pos == MAXINPUT)
        {
            cout << "User input too long\n";
            exit(1);
        }
        if (c == '\n') {
            userinput[pos] = ' ';
        }
        
        else
            userinput[pos] = c;
        if (c == '{') // holds the prompt for blocks of code
            ++parencnt;
        if (c == '}')
            parencnt--;
    } while (parencnt != 0);
} //readParens



// reads input from console and stores in global userinput[]
void readInput()
{
    char  c;
    cout << PROMPT;
    pos = 0;
    do
    {
        ++pos;
        if (pos == MAXINPUT)
        {
            cout << "User input too long\n";
            exit(1);
        }
        nextchar(c);
        if (c == '\n')
            userinput[pos] = ' ';// or space
        else
            userinput[pos] = c;
        
        if (userinput[pos] == '{')  // if pos >6 and matches(pos-1,6,"define")
            readParens();// readDef  ... in def until read key word end
    } while (c != '\n');
    inputleng = pos + 1;
    userinput[pos+1] = ENDOFINPUT; // sentinel
    // input buffer is now filled, reset pos to 1 (later in reader()) and begin parse
}

// reads input from console and stores in global userinput[]
void readInputFromFile()
{
    char  c;
    //cout << PROMPT;
    pos = 0;
    do
    {
        ++pos;
        if (pos == MAXINPUT)
        {
            cout << "User input too long\n";
            exit(1);
        }
        nextchar(c);
        if (c == '\n')
            userinput[pos] = ' ';// or space
        else
            userinput[pos] = c;
        
        if (userinput[pos] == '{')  // if pos >6 and matches(pos-1,6,"define")
            readParens();// readDef  ... in def until read key word end
    } while (c != '\n');
    inputleng = pos + 1;
    userinput[pos+1] = ENDOFINPUT; // sentinel
    // input buffer is now filled, reset pos to 1 (later in reader()) and begin parse
}


// Reads input until end
void reader()
{
    do
    {
        readInput();
        pos = skipblanks(1); // reset pos for tokenizer
    } while (pos > inputleng-1); // ignore blank lines
}

void readerFromFile()
{
    do
    {
        readInputFromFile();
        pos = skipblanks(1); // reset pos for tokenizer
    } while (pos > inputleng-1); // ignore blank lines
}

/*********************************
 END OF LOW-LEVEL HELPER METHODS FOR READING INPUT
 *********************************/


/**************************************************
 **********   Some Tokenizer Helpers below  *********
 * Feel free to insert tokenizer code here (or in other file)*
 ***************************************************/

//makeMap function creates a mapping of tokens to their token categories

void makeMap(){
    
    tokenTypes.insert(make_pair("begin", "start_key"));
    tokenTypes.insert(make_pair("end", "stop_key"));
    tokenTypes.insert(make_pair("inherits", "inherit_key"));
    tokenTypes.insert(make_pair("from", "from_key"));
    tokenTypes.insert(make_pair("classdef", "class_key"));
    tokenTypes.insert(make_pair("endclass", "endclass_key"));
    tokenTypes.insert(make_pair("fundef", "fundef_key"));
    tokenTypes.insert(make_pair("endfun", "endfun_key"));
    tokenTypes.insert(make_pair("if", "if_key"));
    tokenTypes.insert(make_pair("endif", "endif_key"));
    tokenTypes.insert(make_pair("else", "else_key"));
    tokenTypes.insert(make_pair("while", "while_key"));
    tokenTypes.insert(make_pair("endwhile", "endwhile_key"));
    tokenTypes.insert(make_pair("[", "square_left"));
    tokenTypes.insert(make_pair("]", "square_right"));
    tokenTypes.insert(make_pair("(", "p_left"));
    tokenTypes.insert(make_pair(")", "p_right"));
    tokenTypes.insert(make_pair("new", "new_key"));
    tokenTypes.insert(make_pair(".", "dot"));
    tokenTypes.insert(make_pair("+", "plus_op"));
    tokenTypes.insert(make_pair("-", "minus_op"));
    tokenTypes.insert(make_pair("*", "mult_op"));
    tokenTypes.insert(make_pair("/", "div_op"));
    tokenTypes.insert(make_pair("^", "pow_op"));
    tokenTypes.insert(make_pair("&&", "and_op"));
    tokenTypes.insert(make_pair("&", "reference_op")); //added
    tokenTypes.insert(make_pair("||", "or_op"));
    tokenTypes.insert(make_pair("==", "equal_op"));
    tokenTypes.insert(make_pair("!=", "not_equal_op"));
    tokenTypes.insert(make_pair("<", "less_than"));
    tokenTypes.insert(make_pair(">", "greater_than"));
    tokenTypes.insert(make_pair("<=", "less_than_equal"));
    tokenTypes.insert(make_pair(">=", "greater_than_equal"));
    tokenTypes.insert(make_pair("end", "stop_key"));
    tokenTypes.insert(make_pair("'", "single_quote"));
    tokenTypes.insert(make_pair("=", "assign_op"));
    tokenTypes.insert(make_pair("!", "bang_op"));
    tokenTypes.insert(make_pair(",", "comma_token"));
    tokenTypes.insert(make_pair(";", "semicolon_token"));
    
    
}

//helper functions for lexer

int isDigit(int pos)
{
    if ((userinput[pos] < '0') || (userinput[pos] > '9'))
        return 0;
    else
        return 1;
}

// checks to see if user input at index pos is char
int isChar(int pos)
{
    if ((userinput[pos] >= 'a') && (userinput[pos] <= 'z') ||
        (userinput[pos] >= 'A') && (userinput[pos] <= 'Z') ||
        userinput[pos] == '_')
        return 1;
    else
        return 0;
}

int isNewLine(int pos) {
    
    pos = skipblanks(pos);
    if (userinput[pos] == '\n') {
        return 1;
    }
    else {
        return 0;
    }
}

int isParen(int pos)
{
    if ((userinput[pos] == '(') || (userinput[pos] == ')'))
        return 1;
    else
        return 0;
}

//helper functions for lexer implemented by me

//getchar function will get the next character of the input and classify it

/*int classifyChars(int pos){
 
 
 
 
 }*/


void getChar(int position){
    
    nextChar = userinput[position];
    
    // cout << "the nextchar variable is equal to: " << nextChar << endl;
    
    if(isChar(position)){
        charClass = LETTER;
    }
    
    else if(isDigit(position)){
        charClass = DIGIT;
        
    }
    
    else if(userinput[position] == '$'){
        charClass = LASTCHAR;
    }
    
    else if((userinput[position] == '\0') || (userinput[position] == ' ')){
        charClass = SPACE;
    }
    else if(userinput[position] == '\'')
    {
        charClass = SINGLE_PEREN;
    }
    
    else{
        
        charClass = UNKNOWN;
    }
    
    
} // end getchar function


//addchar function will add the next character to a lexeme

void addChar(){
    
    if(lexemeLength <= (NAMELENG -2)){
        // cout << "in the add function " << endl;
        
        lexeme[lexemeLength++] = nextChar;
        lexeme[lexemeLength] = 0;
        //  lexemeLength++;
        
        /*  cout << "lexeme added is: " << lexeme[lexemeLength--];
         cout << "lexeme length is: " << lexemeLength << endl;
         for(int i = 1; i < lexemeLength; i++){
         
         cout << "the lexeme value is: " << lexeme[i] << endl;
         
         } */
    }
}

int lex(int position){
    
    // cout << "|" << nextChar << "| " << "type: " << charClass << endl;
    
    lexemeLength = 0;
    
    int count = 0;
    
    switch(charClass){
            
        case SINGLE_PEREN:
        {
            
            vector<string> token;
            token.push_back("'");
            token.push_back("single_quote");
            tokenList.push_back(token);
            
            skipblanks(position);
            
            addChar();
            getChar(++position);
            
            //           cout << "lexeme[0]: " << lexeme[0] << endl;
            
            
            while(charClass != SINGLE_PEREN){
                
                addChar();
                getChar(++position);
                
            }
            
            addChar();
            getChar(++position);
            
            string stringLexeme;
            
            if(lexeme[0] == '\'')
            {
                //                cout << "HERE " << endl;
                int i = 1;
                while ((i < lexemeLength - 1))
                {
                    //                    cout << "lexemLength: " << lexemeLength << ", lexeme: " << lexeme[i] << endl;
                    stringLexeme.push_back(lexeme[i]);
                    i++;
                }
                
                token.clear();
                token.push_back(stringLexeme);
                token.push_back("string");
                tokenList.push_back(token);
                
                token.clear();
                vector<string> token;
                token.push_back("'");
                token.push_back("single_quote");
                tokenList.push_back(token);
                
                memset(&lexeme[0], 0, sizeof(lexeme));
                
            }
            
            break;
        }
            
        case LETTER:
        {
            skipblanks(position);
            //   cout << "the character is a letter" << endl;
            while(charClass == LETTER || charClass == DIGIT){
                
                addChar();
                getChar(++position);
                
            }
            
            string stringLexeme;
            for(int i = 0; i < lexemeLength; i++){
                
                stringLexeme += lexeme[i];
            }
            
            //     cout << "string is " << stringLexeme << endl;
            
            if(tokenTypes.count(stringLexeme) > 0){
                //        cout << "in here" << endl;
                
                //     cout << "lexeme is a keyword" << endl;
                
                vector<string> tokens;
                tokens.push_back(stringLexeme);
                tokens.push_back(tokenTypes.find(stringLexeme)->second);
                tokenList.push_back(tokens);
                
                memset(&lexeme[0], 0, sizeof(lexeme));
                //lexeme.clear
            }
            
            else{
                
                vector<string> token;
                token.push_back(stringLexeme);
                token.push_back("var");
                
                tokenList.push_back(token);
                
                memset(&lexeme[0], 0, sizeof(lexeme));
                
            }
            
            break;
        }
        case DIGIT:
        {
            skipblanks(position);
            
            bool decimalEncountered = false;
            
            //   cout << "in the digit case" << endl;
            position = skipblanks(position);
            
            while(charClass == DIGIT){
                
                addChar();
                getChar(++position);
                
            }
            
            if(nextChar == '.' && decimalEncountered == false){
                
                decimalEncountered = true;
                
                //                vector<string> token;
                //                token.push_back(".");
                //                token.push_back(tokenTypes.find(".")->second);
                //                tokenList.push_back(token);
                
                addChar();
                
                position = skipblanks(position);
                getChar(++position);
                
                while(charClass == DIGIT){
                    addChar();
                    getChar(++position);
                    
                }
                
                string stringLexeme;
                for(int i = 0; i < lexemeLength; i++){
                    stringLexeme += lexeme[i];
                }
                
                vector<string> token;
                token.push_back(stringLexeme);
                token.push_back("float");
                
                tokenList.push_back(token);
                
                memset(&lexeme[0], 0, sizeof(lexeme));
                
            }
            
            else{
                
                string stringLexeme;
                for(int i = 0; i < lexemeLength; i++){
                    stringLexeme += lexeme[i];
                }
                
                vector<string> token;
                token.push_back(stringLexeme);
                token.push_back("integer");
                
                tokenList.push_back(token);
                
                memset(&lexeme[0], 0, sizeof(lexeme));
                
            }
            
            break;
            
        }
            
        case SPACE:
        {
            position++;
            break;
        }
            
        case UNKNOWN:{
            
            skipblanks(position);
            
            
            if ((nextChar == '&') || (nextChar == '|') || (nextChar == '<') || (nextChar == '>') || (nextChar == '=') || (nextChar == '!')   )
            {
                
                
                while( (charClass == UNKNOWN) && ( (nextChar == '&') || (nextChar == '|') || (nextChar == '<') || (nextChar == '>') || (nextChar == '=') || (nextChar == '!')  ) ){
                    
                    // cout << nextChar << endl;
                    addChar();
                    //      cout << nextChar << endl;
                    getChar(++position);
                    
                }
            }
            else
            {
                addChar();
                //cout << nextChar << endl;
                getChar(++position);
            }
            
            string stringLexeme;
            
            //cout << "lexemLength: " << lexemeLength << endl;
            for(int i = 0; i < lexemeLength; i++){
                
                //         cout << "in this for loop" << endl;
                
                //ensure surrounding space is not included
                
                if(lexeme[i] != ' '){
                    stringLexeme += lexeme[i];
                }
                
                //       cout << "string lexeme is " << stringLexeme << endl;
                
            }
            
            if(stringLexeme.length() > 1){ // if lexeme length is more than 1
                
                if(tokenTypes.count(stringLexeme) == 0){ // if lexeme not in tokenTypes
                    
                    for(int i = 0; i < lexemeLength-1; i++){
                        
                        string s;
                        s.push_back(lexeme[i]);
                        vector<string> token;
                        token.push_back(s);
                        if(tokenTypes.count(s)) {
                            token.push_back(tokenTypes.find(s)->second);
                            tokenList.push_back(token);
                            
                        }
                    }
                    
                    memset(&lexeme[0], 0, sizeof(lexeme));
                    
                } // END if double token is not in the thing
                
                else{
                    
                    vector<string> token;
                    token.push_back(stringLexeme);
                    token.push_back(tokenTypes.find(stringLexeme)->second);
                    tokenList.push_back(token);
                    memset(&lexeme[0], 0, sizeof(lexeme));
                    
                }
                
            } // end check for double unknown characters
            
            else{ // lexeme length is 1
                
                // check if token is '
                
                // cout << stringLexeme << endl;
                
                vector<string> token;
                token.push_back(stringLexeme);
                
                if(tokenTypes.count(stringLexeme))
                {
                    // cout << tokenTypes.find(stringLexeme)->second << endl;
                    token.push_back(tokenTypes.find(stringLexeme)->second);
                }
                
                tokenList.push_back(token);
                memset(&lexeme[0], 0, sizeof(lexeme));
                
            }
            
            break;
        }
            
            
        case LASTCHAR:
            position++;
            break;
    }
    
    return position;
    
}

void tokenize(){
    
    int count = 0;
    while(count < inputleng){
        
        //    cout << "in this while loop where input length is : " << inputleng;
        
        getChar(count);
        count = lex(count);
        
    }
    
    for(int i = 0; i < tokenList.size(); i++){ // remove empty elements
        
        if((tokenList[i][1] == "") || (tokenList[i][0] == "") || (tokenList[i][0] == "\r") || (tokenList[i][0] == "\t"))
        {
            tokenList.erase(tokenList.begin() + i);
        }
        
    }
    
    //    for(int i = 0; i < tokenList.size(); i++)
    //    {
    //        cout << "Token is: " << tokenList[i][0] << ". Category is: " << tokenList[i][1] << "." << endl;
    //    }
    
    // tokenList.clear();
    
    // count ++;
}

// BEGIN PARSER CODE

// prototypes
Val parseVar(string & toPrint);
Val parseInteger(string & toPrint);
Val parseString_token(string & toPrint);
Val parseTerm(string & toPrint);
Val parseMath_pow(string & toPrint);
Val parseFloat(string &);
Val parseNeg(string & toPrint);
Val parseLiteral(string & toPrint);
Val parseMath(string & toPrint);
Val parseMath_value(string & toPrint);
Val parseMath_term(string & toPrint);
Val parseRel_value(string &);
Val parseRel_expr(string &);
Val parseRel_op(string &);
Val parseFunction_invoc(string &);
void parseOb_create(string &);
Val parseOb_manip(string &);
void parseList_element(string &);
Val parseBool_exp(string &);
Val parseVar_assign(string &);
void parseList(string &);
Val parseIf_stmt(string &);
Val parseWhile(string &);
Val parseExpr(string &);
Val parseStmt(string &);
void parseParams(string &, vector<Val> &);
Val parseStmt_list(string &);
void parseFunction_def(string &);
void parseClass_def(string &);
Val parsePerens(string &);
void parseList_contents(string &);


void append(string & stringA , string stringB)
{
    stringA += stringB;
}

// program
Val parseProgram(string & toPrint)
{
    Val prog;
    
    try {
        //(toPrint, "Enter Program\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        if (category.compare("start_key") == 0)
        {
            //(toPrint, "Token Match. Expected start_key and got a " + token + "\n");
            
            
            
            //for (int i = 0; i < tokenList.size(); i++)
            //{
            //  cout << tokenList[i][0] << " " << tokenList[i][1] << endl;
            //}
            
            currToken_index++;
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            
            while (category.compare("stop_key") != 0) {
                
                if (category.compare("class_key") == 0)         // check for possible class_def
                {
                    parseClass_def(toPrint);
                    token = tokenList[currToken_index][0];
                    category = tokenList[currToken_index][1];
                }
                
                //            token = tokenList[currToken_index][0];
                //            category = tokenList[currToken_index][1];
                else if (category.compare("fundef_key") == 0)          // check for possible function_def, many can be in sequence
                {
                    parseFunction_def(toPrint);
                    token = tokenList[currToken_index][0];
                    category = tokenList[currToken_index][1];
                }
                
                else if ( (category.compare("integer") == 0) ||  // stmt --> !, var, math, if, while, square_left, p_left as first term
                         (category.compare("single_quote") == 0) ||
                         (category.compare("float") == 0) ||
                         (category.compare("minus_op") == 0)  ||
                         (category.compare("var") == 0) ||
                         (category.compare("bang_op") == 0) ||
                         (category.compare("if_key") == 0) ||
                         (category.compare("while_key") == 0) ||
                         (category.compare("square_left") == 0) ||
                         (category.compare("p_left") == 0) )
                {
                    prog = parseStmt_list(toPrint);
                    token = tokenList[currToken_index][0];
                    category = tokenList[currToken_index][1];
                }
                
                // currToken_index--; // go back ?
                if (currToken_index >= tokenList.size())
                {
                    return prog;
                }
                
            }
            
            
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            if (category.compare("stop_key") == 0)         // check for end
            {
                //(toPrint, "Token Match. Expected stop_key and got a " + token + "\n");
                currToken_index++;
                //                runtimeStack.pop();
            }
            else
            {
                // error
                throw "compiler error. Expected stop_key and got a " + token + "\n";
                //                return;
                //                runtimeStack.pop();
            }
            
        }
        else
        {
            // error
            throw "compiler error. Expected start_key and got a " + token + "\n";
            //            return;
        }
        //(toPrint, "Exit Program\n");
        
        
        
        
    } catch (string error) {
        throw error;
    }
    
    return prog;
}

// class_def
void parseClass_def(string & toPrint)
{
    // add to global scope --> interface with class table
    
    // dynamically allocate memory for new object.
    
    // if inherit from, copy proper class from classTable to object (assignment operator already overloaded)
    
    // set name of newClass
    
    // add newClass to runtimeStack.global.classTable.
    
    // change runtimeStack.mode from "regular" to name of new classe (use this in var assign and function def to put variables and functions inside scope of this new class)
    
    // add member functions, variables to new object
    
    // change runtimeStack.mode back ro "regular"
    
    string className;
    string inheritName = "None";
    
    try {
        //(toPrint, "Enter class_def\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        if (category.compare("class_key") == 0)
        {
            //(toPrint, "Token Match. Expected class_key and got a " + token + "\n");
            
            currToken_index++; // get next
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            
            // check for var
            if(category.compare("var") == 0)
            {
                className = parseVar(toPrint).token;
                
                // dynamically allocate memory for new object.
                Object newClass;
                newClass.name = className;
                
                token = tokenList[currToken_index][0];
                category = tokenList[currToken_index][1];
                
                if (category.compare("inherit_key") == 0)
                {
                    //(toPrint, "Token Match. Expected possible inherit_key and got a " + token + "\n");
                    
                    currToken_index++; // get next
                    token = tokenList[currToken_index][0];
                    category = tokenList[currToken_index][1];
                    
                    if (category.compare("from_key") == 0)
                    {
                        currToken_index++;
                    }
                    else
                    {
                        throw "compiler error. Expected from and got a " + token + "\n";
                    }
                    
                    // currToken_index++; // get next
                    inheritName = parseVar(toPrint).token;
                    
                    if (! runtimeStack.global().classTable.has(inheritName)) {
                        throw "runtime error. attempted inheritence from undefined class";
                    }
                    
                    newClass = runtimeStack.global().classTable.get(inheritName); // if inherit from, copy proper class from classTable to object (assignment operator already overloaded)
                    newClass.name = className; // give proper name
                }
                
                runtimeStack.global().classTable.add(newClass);
                runtimeStack.mode = newClass.name;
                
                // update
                token = tokenList[currToken_index][0];
                category = tokenList[currToken_index][1];
                
                // check for optional stmt_list
                if ( (category.compare("integer") == 0) ||  // stmt --> !, var, math, if, while, square_left as first term
                    (category.compare("single_quote") == 0) ||
                    (category.compare("float") == 0) ||
                    (category.compare("minus_op") == 0)  ||
                    (category.compare("var") == 0) ||
                    (category.compare("bang_op") == 0) ||
                    (category.compare("if_key") == 0) ||
                    (category.compare("while_key") == 0) ||
                    (category.compare("square_left") == 0) ||
                    (category.compare("p_left") == 0) )
                {
                    //(toPrint, "Token Match. Expected possible stmt_list term and got a " + token + "\n");
                    
                    parseStmt_list(toPrint); // involves variable assignments --> need to be in scope of class
                }
                
                // update
                token = tokenList[currToken_index][0];
                category = tokenList[currToken_index][1];
                
                // check for optional function_def
                while (category.compare("fundef_key") == 0) // check for optional inherits from
                {
                    //(toPrint, "Token Match. Expected possible fundef_key and got a " + token + "\n");
                    
                    
                    
                    parseFunction_def(toPrint); // LOSE member2 upon entry
                    
                    
                    // Is the function parsed here being added to the correct place? i.e. the funtable of this class. Yes, it is
                    
                    token = tokenList[currToken_index][0];
                    category = tokenList[currToken_index][1];
                }
                
                token = tokenList[currToken_index][0];
                category = tokenList[currToken_index][1];
                
                runtimeStack.mode = "regular";
                
                // check for endclass
                if (category.compare("endclass_key") == 0) // check for optional inherits from
                {
                    //(toPrint, "Token Match. Expected endclass_key and got a " + token + "\n");
                    currToken_index++;
                    
                }
                else
                {
                    throw "compiler error. Expected endclass_key and got a " + token + "\n";
                    return;
                }
                
            }
            else
            {
                // error
                throw "compiler error. Expected var and got a " + token + "\n";
                return;
            }
        }
        else
        {
            // error
            throw "compiler error. Expected class_key and got a " + token + "\n";
            return;
            
        }
        //(toPrint, "Exit class_def\n");
    } catch (string error) {
        throw error;
    }
}

// function_def
void parseFunction_def(string & toPrint)
{
    
    try {
        //(toPrint, "Enter function_def\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        Val returnVar;
        Val functionName;
        vector<Val> paramList;
        vector< vector< string > > code;
        
        if (category.compare("fundef_key") == 0)
        {
            
            //(toPrint, "Token Match. Expected fundef_key and got a " + token + "\n");
            
            currToken_index++; // go to next token
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            
            if (category.compare("var") == 0)
            {
                returnVar = parseVar(toPrint);
                
                token = tokenList[currToken_index][0];
                category = tokenList[currToken_index][1];
                
                // check next
                if (category.compare("assign_op") == 0)
                {
                    
                    //(toPrint, "Token Match. Expected assign_op and got a " + token + "\n");
                    
                    currToken_index++; // go to next token
                    
                    functionName = parseVar(toPrint);
                    
                    
                    token = tokenList[currToken_index][0];
                    category = tokenList[currToken_index][1];
                    
                    
                    // check for left peren
                    
                    if (category.compare("p_left") == 0)
                    {
                        
                        //(toPrint, "Token Match. Expected p_left and got a " + token + "\n");
                        
                        currToken_index++; // go to next token
                        token = tokenList[currToken_index][0];
                        category = tokenList[currToken_index][1];
                        
                        // check for optional params inside perens
                        if ( (category.compare("integer") == 0) ||  // params --> expr --> !, var, or math as first term
                            (category.compare("single_quote") == 0) ||
                            (category.compare("float") == 0) ||
                            (category.compare("minus_op") == 0)  ||
                            (category.compare("var") == 0) ||
                            (category.compare("bang_op") == 0) ||
                            (category.compare("square_left") == 0) ||
                            (category.compare("p_left") == 0) ||
                            (category.compare("reference_op") == 0) )
                        {
                            // parse params
                            parseParams(toPrint, paramList);
                        }
                        
                        token = tokenList[currToken_index][0];
                        category = tokenList[currToken_index][1];
                        
                        // check for p_right
                        if (category.compare("p_right") == 0)
                        {
                            //(toPrint, "Token Match. Expected p_right and got a " + token + "\n");
                            currToken_index++;
                            
                            token = tokenList[currToken_index][0];
                            category = tokenList[currToken_index][1];
                            
                            
                            // extract all code from within function
                            
                            while (category.compare("endfun_key") != 0) {
                                
                                vector<string> code_element;
                                code_element.push_back(token);
                                code_element.push_back(category);
                                
                                code.push_back(code_element);
                                
                                currToken_index++;
                                token = tokenList[currToken_index][0];
                                category = tokenList[currToken_index][1];
                            }
                            
                            //                            // check for optional stmt_list
                            //                            if ( (category.compare("integer") == 0) ||  // stmt --> !, var, math, if, while, square_left as first term
                            //                                (category.compare("single_quote") == 0) ||
                            //                                (category.compare("float") == 0) ||
                            //                                (category.compare("minus_op") == 0)  ||
                            //                                (category.compare("var") == 0) ||
                            //                                (category.compare("bang_op") == 0) ||
                            //                                (category.compare("if_key") == 0) ||
                            //                                (category.compare("while_key") == 0) ||
                            //                                (category.compare("square_left") == 0) ||
                            //                                (category.compare("p_left") == 0) )
                            //                            {
                            //                                parseStmt_list(toPrint);
                            //                            }
                            
                            token = tokenList[currToken_index][0];
                            category = tokenList[currToken_index][1];
                            
                            // check for endfun
                            if (category.compare("endfun_key") == 0)
                            {
                                //(toPrint, "Token Match. Expected endfun_key and got a " + token + "\n");
                                currToken_index++;
                                
                                
                                if( runtimeStack.mode.compare("regular") == 0)
                                {
                                    runtimeStack.global().funTable.add(functionName.token, paramList, code, returnVar.token);
                                }
                                else
                                {
                                    
                                    //                                    Scope * temp = &runtimeStack.global();
                                    //                                    Object * temp2 = &temp->classTable.get(runtimeStack.mode);
                                    //                                    temp2->member_functions.add(functionName.token, paramList, code, returnVar.token);   // THIS IS THE PROBLEMxrxr
                                    
                                    runtimeStack.global().classTable.get(runtimeStack.mode).member_functions.add(functionName.token, paramList, code, returnVar.token);
                                }
                                
                            }
                            else
                            {
                                // error
                                throw "compiler error. Expected endfun_key and got a " + token + "\n";
                                return;
                            }
                            
                        }
                        else
                        {
                            throw "compiler error. Expected p_Right and got a " + token + "\n";
                            return;
                        }
                        
                    }
                    else
                    {
                        // error
                        throw "compiler error. Expected p_left and got a " + token + "\n";
                        return;
                    }
                    
                }
                
            }
            else
            {
                // error
                throw "compiler error. Expected var and got a " + token + "\n";
                return;
            }
        }
        else
        {
            // error
            throw "compiler error. Expected fundef_key and got a " + token + "\n";
            return;
            
        }
        //(toPrint, "Exit function_def\n");
    } catch (string error) {
        throw error;
    }
}

// stmt_list
Val parseStmt_list(string & toPrint)
{
    
    Val stmt;
    Val empty;
    
    try {
        //(toPrint, "Enter stmt_list\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        if ( (category.compare("integer") == 0) ||  // stmt --> !, var, math, if, while, square_left as first term
            (category.compare("single_quote") == 0) ||
            (category.compare("float") == 0) ||
            (category.compare("minus_op") == 0)  ||
            (category.compare("var") == 0) ||
            (category.compare("bang_op") == 0) ||
            (category.compare("if_key") == 0) ||
            (category.compare("while_key") == 0) ||
            (category.compare("square_left") == 0) ||
            (category.compare("p_left") == 0) )
        {
            //        append(toPrint, "Token Match. Expected - and got a " + token + "\n");
            stmt = parseStmt(toPrint); // assumed: currToken_index incremented
            
            if (currToken_index >= tokenList.size()) {
                return stmt;
            }
            
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            
            if ( (category.compare("semicolon_token") == 0) ) {
                
                //(toPrint, "Token Match. Expected possible semicolon_token and got a " + token + "\n");
                
                currToken_index++;
                
                if (currToken_index >= tokenList.size())
                {
                    
                    //                    string error = "Error: out of bounds";
                    //                    throw error;
                    return stmt;
                }
                
                token = tokenList[currToken_index][0];
                category = tokenList[currToken_index][1];
                
                if ( (category.compare("stop_key") == 0) )
                {
                    return stmt;
                }
                
            }
            
            if ( (category.compare("integer") == 0) ||  // stmt --> !, var, math, if, while, square_left as first term
                (category.compare("single_quote") == 0) ||
                (category.compare("float") == 0) ||
                (category.compare("minus_op") == 0)  ||
                
                (category.compare("var") == 0) ||
                (category.compare("bang_op") == 0) ||
                (category.compare("if_key") == 0) ||
                (category.compare("while_key") == 0) ||
                (category.compare("square_left") == 0) ||
                (category.compare("p_left") == 0) )
            {
                return parseStmt_list(toPrint);  // NEED TO DEAL WITH THIS
            }
        }
        else
        {
            // error
            throw "compiler error. Expected possible integer, string, float, minus_op, var, bang_op, if_key, or while_key and got a " + token + "\n";
            //            return;
            throw "Token Error. Expected possible integer, string, float, minus_op, var, bang_op, if_key, or while_key and got a " + token + "\n";
            
        }
        
        //(toPrint, "Exit stmt_list\n");
        
        return stmt;
        
    } catch (string error) {
        throw error;
    }
}

// stmt
Val parseStmt(string & toPrint)
{
    
    Val result;
    
    try {
        //(toPrint, "Enter stmt\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        // currToken_index++;
        
        if (category.compare("if_key") == 0) // if_stmt
        {
            // parse if_stmt
            result = parseIf_stmt(toPrint);
            
        }
        else if (category.compare("while_key") == 0) // while_stmt
        {
            // parse while_stmt
            result = parseWhile(toPrint);
            
        }
        else if ( (category.compare("integer") == 0) ||  // expr --> !, var, or math as first term
                 (category.compare("single_quote") == 0) ||
                 (category.compare("float") == 0) ||
                 (category.compare("minus_op") == 0)  ||
                 (category.compare("var") == 0) ||
                 (category.compare("bang_op") == 0) ||
                 (category.compare("square_left") == 0) ||
                 (category.compare("p_left") == 0) )
        {
            // parse expr
            result = parseExpr(toPrint);
            
        }
        else
        {
            // error
            throw "compiler error. Expected <var_assign>, <bool_exp>, <ob_create>, or <list> and got a " + token + "\n";
            //            return;
            
            
        }
        //(toPrint, "Exit stmt\n");
        
        
        
    } catch (string error) {
        throw error;
    }
    
    return result;
}

// expr
Val parseExpr(string & toPrint) // poasible issue -->
{
    
    Val bool_exp;
    Val empty;
    Val var;
    
    try {
        //(toPrint, "Enter expr\n");
        
        // cout << toPrint << endl;
        currToken_index++; // use 1st look ahead --> check for '='
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        
        // if (, then parse function invocation?
        if (token.compare("(") == 0)
        {
            currToken_index--;
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            return parseFunction_invoc(toPrint);
        }
        
        
        if (category.compare("assign_op") == 0)// var_assign or ob_create test
        {
            
            // check for ob_Create
            
            currToken_index++; // use 2nd look a ahead --> check for 'new'
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            
            if (category.compare("new_key") == 0)
            {
                currToken_index--;
                currToken_index--; // put back to var
                
                parseOb_create(toPrint);
                return empty;
            }
            
            else
            {
                currToken_index--; // put back to var
                currToken_index--;
                
                var = parseVar_assign(toPrint);
                return var;
            }
            
        }
        else
        {
            currToken_index--; // put back to first look ahead
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
        }
        
        if (category.compare("square_left") == 0) // list[;]
        {
            // check list --> [
            parseList(toPrint);
            
        }
        else if  ( (category.compare("integer") == 0)    ||  // bool_exp --> ! or math as first term
                  (category.compare("single_quote") == 0)    ||
                  (category.compare("float") == 0)     ||
                  (category.compare("minus_op") == 0)  ||
                  (category.compare("var") == 0)       ||
                  (category.compare("bang_op") == 0) ||
                  (category.compare("p_left") == 0) )
        {
            
            bool_exp = parseBool_exp(toPrint);
            
        }
        else
        {
            // error
            throw "compiler error. Expected <var_assign>, <bool_exp>, <ob_create>, or <list> and got a " + token + "\n";
            //            return;
        }
        
        
        //(toPrint, "Exit expr\n");
        
    } catch (string error) {
        throw error;
    }
    
    return bool_exp;
    
}

// if_stmt
Val parseIf_stmt(string & toPrint)
{
    try{
        //(toPrint, "Enter if_stmt\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        Val result;
        
        //currToken_index++;
        if (category.compare("if_key") == 0)
        {
            //(toPrint, "Token Match. Expected if_key and got a " + token + "\n");
            currToken_index++;
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            
            if (category.compare("p_left") == 0)
            {
                //(toPrint, "Token Match. Expected p_left and got a " + token + "\n");
                
                currToken_index++;
                Val myBool = parseBool_exp(toPrint); // assume currToken_index incremented
                
                // SKIPING p_right
                
                // lookahead
                token = tokenList[currToken_index][0];
                category = tokenList[currToken_index][1];
                
                if (category.compare("p_right") == 0)
                {
                    //(toPrint, "Token Match. Expected p_right and got a " + token + "\n");
                    
                    currToken_index++;
                    
                    // if myBool == 1, parseStmt_list
                    // else, skip over all tokens until end_if key
                    
                    if (stoi(myBool.token) == 1)
                    {
                        result = parseStmt_list(toPrint);
                        token = tokenList[currToken_index][0];
                        category = tokenList[currToken_index][1];
                    }
                    else
                    {
                        token = tokenList[currToken_index][0];
                        category = tokenList[currToken_index][1];
                        while ((category.compare("else_key") != 0)  && category.compare("endif_key") != 0)
                        {
                            currToken_index++;
                            token = tokenList[currToken_index][0];
                            category = tokenList[currToken_index][1];
                        }
                    }
                    
                    if (category.compare("endif_key") == 0)
                    {
                        //(toPrint, "Token Match. Expected endif_key and got a " + token + "\n");
                        
                        currToken_index++;
                        
                    }
                    else if ((category.compare("else_key") == 0 ) && (stoi(myBool.token) == 0))
                    {
                        //(toPrint, "Token Match. Expected else_key and got a " + token + "\n");
                        
                        currToken_index++;
                        result = parseStmt_list(toPrint);
                        
                        token = tokenList[currToken_index][0];
                        category = tokenList[currToken_index][1];
                        
                        // check for endif_key
                        if (category.compare("endif_key") == 0)
                        {
                            //(toPrint, "Token Match. Expected endif_key and got a " + token + "\n");
                            
                            currToken_index++;
                        }
                        else
                        {
                        // error
                            throw "compiler error. Expected endif_key and got a " + token + "\n";
                        }
                        
                    }
                    else
                    {
                        // error
                        throw "compiler error. Expected else_key and got a " + token + "\n";
                        return result;
                    }
                    
                }
                else
                {
                    // error
                    throw "compiler error. Expected p_right and got a " + token + "\n";
                    return result;
                }
                
            }
            else
            {
                // error
                throw "compiler error. Expected p_left and got a " + token + "\n";
                return result;
            }
            
        }
        else
        {
            // error
        }
        //(toPrint, "Exit if_stmt\n");
        return result;
    } catch (string error) {
        throw error;
    }
}

// while
Val parseWhile(string & toPrint)
{
    try{
        //(toPrint, "Enter while\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        Val result;
        
        if (category.compare("while_key") == 0)
        {
            //(toPrint, "Token Match. Expected while_key and got a " + token + "\n");
            
            // check for left peren
            currToken_index++;
            string token = tokenList[currToken_index][0];
            string category = tokenList[currToken_index][1];
            if (category.compare("p_left") == 0)
            {
                
                currToken_index++;
                
                int index_bool_exp = currToken_index;
                Val myBool = parseBool_exp(toPrint); // assumed: currtoken incremented
                
                // check for <p_right>
                token = tokenList[currToken_index][0];
                category = tokenList[currToken_index][1];
                
                if (category.compare("p_right") == 0)
                {
                    
                    //(toPrint, "Token Match. Expected p_right and got a " + token + "\n");
                    
                    currToken_index++;
                    
                    // ALSO need to save the starting index for bool_exp, as need to evaluate this boolean expression after each while iteration
                    
                    int before_stmt_list = currToken_index;
                    
                    while (stoi(myBool.token) == 1)
                    {
                        currToken_index = before_stmt_list; // set back to begining of stmt list
                        result = parseStmt_list(toPrint);
                        int after_stmt_list = currToken_index;
                        
                        currToken_index = index_bool_exp; // update bool val
                        myBool = parseBool_exp(toPrint);
                        
                        currToken_index = after_stmt_list; // set position in token list back to after the stmt list
                        
                    }
                    
                    // check for end_if or else
                    token = tokenList[currToken_index][0];
                    category = tokenList[currToken_index][1];
                    
                    // check for endwhile_key
                    
                    if (category.compare("endwhile_key") == 0)
                    {
                        //(toPrint, "Token Match. Expected endwhile_key and got a " + token + "\n");
                        currToken_index++;
                    }
                    else
                    {
                        // error
                        throw "compiler error. Expected endwhile_key and got a " + token + "\n";
                        return result;
                    }
                    
                }
                else
                {
                    // error
                    throw "compiler error. Expected p_right and got a " + token + "\n";
                    return result;
                }
                
            }
            else
            {
                // error
                throw "compiler error. Expected p_left and got a " + token + "\n";
                return result;
            }
            
        }
        else
        {
            // error
            throw "compiler error. Expected while_key and got a " + token + "\n";
            return result;
        }
        //(toPrint, "Exit while\n");
        
        return result;
    } catch (string error) {
        throw error;
    }
}

// var_assign
Val parseVar_assign(string & toPrint)
{
    try{
        //(toPrint, "Enter var_assign\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        Val var;
        Val expr;
        
        Val reference;
        
        if (category.compare("var") == 0)
        {
            //        append(toPrint, "Token Match. Expected - and got a " + token + "\n");
            var = parseVar(toPrint);
            
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            if (category.compare("assign_op") == 0)
            {
                //(toPrint, "Token Match. Expected assign_op and got a " + token + "\n");
                
                currToken_index++;
                
                token = tokenList[currToken_index][0];
                category = tokenList[currToken_index][1];
                
                expr = parseExpr(toPrint);  // NEED UPDATE THIS FOR LISTS
                
                token = tokenList[currToken_index][0];
                category = tokenList[currToken_index][1];
                
//                if (token.compare(";")) {
//                    currToken_index++;
//                }
                
                if ((expr.lexeme.compare("var") == 0) )
                {
                    // check if in table
                    
                    if ( runtimeStack.hasVar(expr.token) || (expr.value.compare("undefined") != 0)) {
                        
                        if (expr.value.compare("undefined") != 0) // initialized variable returned from function
                        {
                            var.type = expr.type;
                            var.value = expr.value;
                        }
                        else if (! (runtimeStack.getVar(expr.token).value.compare("undefined") == 0)) {
                            // check in table
                            
                            // SHALLOW COPY HERE
                            // update referenceName
                            
                            // update everything except name
                            // expr = runtimeStack.getVar(expr.token);
                            
                            if (expr.byReference) {
                                var.referenceName = expr.token;
                                var.hasRefVar = true;
                                
                                var.type = expr.type;
                                var.value = expr.value;
                            }
                            else
                            {
                                var.type = expr.type;
                                var.value = expr.value;
                            }
                            
                        }
                        else
                        {
                            // throw undefined error
                            throw "runtime error. variable " + expr.token + " undefined.";
                        }
                    }
                    else
                    {
                        // throw unkown var error
                        throw "runtime error. unkown variable or function: " + expr.token;
                    }
                }
                else if (expr.lexeme.compare("square_left") == 0)  // parseList
                {
                    parseList(toPrint);
                    
                    
                    
                }
                else // string, int, float
                {
                    var.value = expr.token;
                    var.type = expr.lexeme;
                    //                    reference = &var;
                    //                    reference->value = expr.token;
                    //                    reference->type = expr.lexeme;
                }
                
                if( runtimeStack.mode.compare("regular") == 0)
                {
                    if ( runtimeStack.hasVar(var.token) ) // check that var in table; if exists, update
                    {
                        
                        // save name
                        //                        string name = runtimeStack.getVar(var.token).token;
                        
                        // shallow copy
                        //                        runtimeStack.getVar(var.token) = reference;
                        
                        // restore name
                        //                        runtimeStack.getVar(var.token).token = name;
                        
                        runtimeStack.updateVar(var.token, var);
                        
                        //                    runtimeStack.getVar(var.token)->type = var.type;
                        //                    runtimeStack.getVar(var.token)->value = var.value;
                    }
                    else if ( ! runtimeStack.hasVar(var.token) ) //
                    {
                        if (mode_interactive)
                        {
                            //                            reference->token = var.token; //set name
                            
                            runtimeStack.global().varTable.add(var);
                            
                        }
                        else
                        {
                            //                            reference->token = var.token;
                            
                            runtimeStack.top().varTable.add(var); // if not, then add
                            
                        }
                    }
                }
                else // class_def mode, with mode == name of new class
                {
                    
                    if ( runtimeStack.global().classTable.get(runtimeStack.mode).member_variables.has(var.token) ) // check that var in table; if exists, update
                    {
                        //                        reference->token = var.token; // set name
                        
                        runtimeStack.global().classTable.get(runtimeStack.mode).member_variables.update(var.token, var);
                        //                    runtimeStack.global().classTable.get(runtimeStack.mode).member_variables.get(var.token)->value = var.value;
                    }
                    else if ( ! runtimeStack.hasVar(var.token) ) //
                    {
                        //                        reference->token = var.token; // set name
                        
                        runtimeStack.global().classTable.get(runtimeStack.mode).member_variables.add(var);
                        
                        //                        Scope * temp =  &runtimeStack.global();
                        //                        Object * temp2 = &temp->classTable.get(runtimeStack.mode);
                        //                        temp2->member_variables.add(reference); // if not, then add
                    }
                }
                
            }
            else
            {
                // error
                //(toPrint, "Token Match. Expected assign_op and got a " + token + "\n");
                currToken_index++;
            }
        }
        else
        {
            // error
            throw "compiler error. Expected var and got a " + token + "\n";
        }
        //(toPrint, "Exit var_assign\n");
        
        return var;
    } catch (string error) {
        throw error;
    }
}

// list
void parseList(string & toPrint)
{
    try{
        //(toPrint, "Enter List\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        if (category.compare("square_left") == 0)
        {
            //(toPrint, "Token Match. Expected square_left and got a " + token + "\n");
            currToken_index++;
            
            parseList_contents(toPrint);
            //        append(toPrint, "Token Match. Expected - and got a " + token + "\n");
            
            
            string token = tokenList[currToken_index][0];
            string category = tokenList[currToken_index][1];
            if (category.compare("square_right") == 0)
            {
                //(toPrint, "Token Match. Expected square_right and got a " + token + "\n");
                currToken_index++;
            }
            else
            {
                // error
                throw "compiler error. Expected square_right and got a " + token + "\n";
                currToken_index++;
            }
            
        }
        else
        {
            throw "compiler error. Expected square_left and got a " + token + "\n";
            return;
        }
        //(toPrint, "Exit List\n");
    } catch (string error) {
        throw error;
    }
}

void parseList_contents(string & toPrint)
{
    try{
        //(toPrint, "Enter list_contents\n");
        // string token = tokenList[currToken_index][0];
        // string category = tokenList[currToken_index][1];
        
        //   if (category.compare("comma_token") == 0) // if coma
        // { // parseList_contents
        //     parseList_contents(toPrint);
        // }
        // else // parseList_element
        // {
        //    parseList_element(toPrint);
        // }
        
        // append(toPrint, "Exit list_element\n");
        
        // parseList_element
        parseList_element(toPrint);
        
        string token = tokenList[currToken_index][0]; // check for optional comma
        string category = tokenList[currToken_index][1];
        if (category.compare("comma_token") == 0)  // if comma, then parseList_contents
        {
            //(toPrint, "Token Match. Expected possible comma_token and got a " + token + "\n");
            currToken_index++;
            parseList_contents(toPrint);
        }
    } catch (string error) {
        throw error;
    }
}

// list_element
void parseList_element(string & toPrint)
{
    try{
        //(toPrint, "Enter list_element\n");
        
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        // decide between list, var, and empty string
        if (category.compare("var") == 0) // var
        {
            parseVar(toPrint);
        }
        else if ((category.compare("integer") == 0) ||  // literal check
                 (category.compare("single_quote") == 0) ||
                 (category.compare("float") == 0) )
        {
            parseLiteral(toPrint);
        }
        else if (category.compare("square_left") == 0)
        {
            parseList(toPrint);
        }
        else // empty string condition ???
        {
            //(toPrint, "Token Match. Expected possible empty string and got a " + token + "\n");
            currToken_index++;
        }
        
        //(toPrint, "Exit list_element\n");
    } catch (string error) {
        throw error;
    }
}

// object
Val parseObject(string & toPrint)
{
    try{
        // c
        Val result;
        
        //(toPrint, "Enter Ojbect\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        
        if (category.compare("var") == 0)
        {
            parseVar(toPrint); // assume that parseVar increment currToken_index
            string token = tokenList[currToken_index][0];
            string category = tokenList[currToken_index][1];
            
            // decide between ob_create and ob_manip
            if (category.compare("assign_op") == 0) // ob_create
            {
                currToken_index--; // put back to <var>
                parseOb_create(toPrint);
            }
            else if (category.compare("dot") == 0) // ob_manip
            {
                currToken_index--; // put back to <var>
                result = parseOb_manip(toPrint);
            }
            else
            {
                // error
                throw "compiler error. Expected assign_op or dot and got a " + token + "\n";
                return result;
            }
        }
        else
        {
            // error
            throw "compiler error. Expected var and got a " + token + "\n";
            return result;
        }
        //(toPrint, "Exit Object\n");
        
        return result;
    } catch (string error) {
        throw error;
    }
    
}

// ob_create
void parseOb_create(string & toPrint)
{
    try {
        // interface with runtimeStack.global.objectTable
        
        // check that class name exists in table:
        // if not, throw error
        // if does, then load new object of type className into objectTable
        
        
        //(toPrint, "Enter Ob_create\n");
        
        string instance_Name = parseVar(toPrint).token;
        string class_Name;
        
        //currToken_index++;
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        if (category.compare("assign_op") == 0)
        {
            //(toPrint, "Token Match. Expected assign_op and got a " + token + "\n");
            
            // lookahead
            currToken_index++;
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            if (category.compare("new_key") == 0)
            {
                currToken_index++;
                class_Name = parsePerens(toPrint).token;
                
                if (runtimeStack.global().classTable.has(class_Name)) {
                    Object newObject;
                    newObject = runtimeStack.global().classTable.get(class_Name);
                    newObject.name = instance_Name;
                    runtimeStack.global().objectTable.add(newObject);
                }
                else
                {
                    throw "Error. Cannot create new instance of " + class_Name + " because " + class_Name + " has not been defined";
                }
            }
            else
            {
                throw "compiler error. Expected new_key and got a " + token + "\n";
                return;
            }
        }
        else
        {
            throw "compiler error. Expected dot and got a " + token + "\n";
            return;
        }
        //(toPrint, "Exit Ob_create\n");
    } catch (string error) {
        throw error;
    }
}

// START HERE --> x in object table doesn't have everything it should, like the function from Foo'

// ob_manip
Val parseOb_manip(string & toPrint)
{
    try {
        // need to extract class and function name,
        // then look up class in scope 0 (global)
        // then, if one of the default functions, call the right one, else, look up in funTable of the class
        
        //(toPrint, "Enter parseOb_manip\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        //    currToken_index++;
        
        Val result;
        
        string funName;
        string objName;
        
        if (category.compare("var") == 0)
        {
            objName = parseVar(toPrint).token;
            
            // look ahead
            //currToken_index++;
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            
            if (category.compare("dot") == 0)
            {
                //(toPrint, "Token Match. Expected possible dot and got a " + token + "\n");
                currToken_index++;
                
                // NEED TO ADJUST parseFunction_invoc for ob_manip --> change from looking at global scope, to looking at member functions of correct object
                runtimeStack.mode = objName;
                
                args.clear();
                
                funName = parseFunction_invoc(toPrint).token; // need to extract function names, paramaters
                
                if (runtimeStack.global().objectTable.has(objName))
                {
                    // three predifined functions:
                    
                    //                    START HERE
                    
                    if (funName.compare("toString") == 0) { // toString()
                        result.lexeme = "string";
                        result.token = runtimeStack.global().objectTable.get(objName).toString();
                    }
                    else if (funName.compare("get") == 0) // get( <string> )
                    {
                        // check that one paramater of correct type
                        if (args.size() == 1)
                        {
                            if ((args[0].lexeme.compare("string") == 0) ) // literal
                            {
                                
                                result = runtimeStack.global().objectTable.get(objName).get(args[0].token);
                                //                                result =
                                //                                string test = temp->get(args[0].token);
                                
                            }
                            else if (args[0].type.compare("string") == 0) // var
                            {
                                result = runtimeStack.global().objectTable.get(objName).get(args[0].value);
                            }
                            else
                            {
                                throw "runtime error. invalid parameter type for function call: " + funName;
                            }
                        }
                        else
                        {
                            throw "runtime error. invalid number of paramaters fo function call: " + funName;
                        }
                    }
                    else if (funName.compare("set") == 0) // set(<string>,val)
                    {
                        // check that two paramaters of correct type
                        // check that one paramater of correct type
                        if (args.size() == 2)
                        {
                            if ((args[0].lexeme.compare("string") == 0) )
                            {
                                runtimeStack.global().objectTable.get(objName).set(args[0].token, args[1]);
                            }
                            else if (args[0].type.compare("string") == 0) // var
                            {
                                runtimeStack.global().objectTable.get(objName).set(args[0].value, args[1]);
                            }
                            else
                            {
                                throw "runtime error. invalid parameter type for function call: " + funName;
                            }
                        }
                        else
                        {
                            throw "runtime error. invalid number of paramaters fo function call: " + funName;
                        }
                        
                    }
                    else if (runtimeStack.global().objectTable.get(objName).member_functions.has(funName)) { // else other function
                        
                        // check correct number and type of paramaters
                        if (runtimeStack.global().objectTable.get(objName).member_functions.get(funName).param_list.size() == args.size()) {
                            
                            //                            START HERE
                            // need to run function in similiar manner to last half of parseFunction_invoc() ... create new scope, etc.
                            runtimeStack.push_on(); // CREATE NEW SCOPE ON RUNTIME STACK
                            
                            // LOAD function params into var table of current scope
                            for (int i = 0; i < runtimeStack.global().objectTable.get(objName).member_functions.get(funName).param_list.size(); i++) {
                                Val param = runtimeStack.global().objectTable.get(objName).member_functions.get(funName).param_list[i];
                                runtimeStack.top().varTable.add(param);
                            }
                            
                            // LOAD passed arguments to their corresponding variables.
                            for (int i = 0; i < args.size(); i++) {
                                if (args[i].lexeme.compare("var") == 0) {
                                    
                                    if (runtimeStack.hasVar(args[i].token)) {
                                        Val param = runtimeStack.global().objectTable.get(objName).member_functions.get(funName).param_list[i];
                                        
                                        if (param.type.compare("reference"))
                                        {
                                            runtimeStack.top().varTable.update(param.token, runtimeStack.getVar_aboveScopes(args[i].token));
                                        }
                                        else
                                        {
                                            runtimeStack.top().varTable.get(param.token).type = runtimeStack.getVar_aboveScopes(args[i].token).type;
                                            runtimeStack.top().varTable.get(param.token).value = runtimeStack.getVar_aboveScopes(args[i].token).value;
                                        }
                                        
                                    }
                                    else
                                    {
                                        throw "runtime error. unkown variable " + args[i].token + " referenced in invocation of " + funName;
                                    }
                                    
                                }
                                else
                                {
                                    Val param = runtimeStack.global().objectTable.get(objName).member_functions.get(funName).param_list[i];
                                    runtimeStack.top().varTable.get(param.token).type = args[i].lexeme;
                                    runtimeStack.top().varTable.get(param.token).value = args[i].token;
                                }
                                
                            }
                            
                            // execute code found in function table
                            // save tokenList
                            vector< vector < string > > temp_tokenList = tokenList;
                            // save currToken_index
                            int temp_currToken_index = currToken_index;
                            // update tokenList
                            //                            start here --> is code getting placed in objects->member_functions.get(funName).code ? check parseClass_def(), mimic parseFunction_def if neccesary
                            
                            tokenList = runtimeStack.global().objectTable.get(objName).member_functions.get(funName).code;
                            // set currToken_index to 0
                            currToken_index = 0;
                            // call parse stmt list
                            
                            //                            Also: do args or runtimeStack.mode need to be changed before callsing parseStmt_list()?
                            
                            result = parseStmt_list(toPrint);
                            // restore tokenList
                            tokenList = temp_tokenList;
                            // restore currToken_index
                            currToken_index = temp_currToken_index;
                            
                            // find return value in local scope
                            string returnVarName = runtimeStack.global().objectTable.get(objName).member_functions.get(funName).return_var;
                            
                            result = runtimeStack.global().objectTable.get(objName).member_variables.get(returnVarName);
                            
                            // pop scope
                            runtimeStack.pop();
                        }
                        else
                        {
                            throw "runtime error. invalid number of paramaters fo function call: " + funName;
                        }
                        
                    }
                    else
                    {
                        throw "runtime error. " + funName + " is not a member function of " + objName;
                    }
                    
                }
                else
                {
                    throw "runtime error. attempted maniplation of undefined object: " + objName;
                }
                
                runtimeStack.mode = "regular";
                
            }
            else
            {
                throw "compiler error. Expected dot and got a " + token + "\n";
                return result;
            }
            
        }
        
        
        //(toPrint, "Exit Bool_ex\n");
        return result;
    } catch (string error) {
        throw error;
    }
}

// params
void parseParams(string & toPrint, vector<Val> & paramList) // NEED TO STORE WHICH ARE BY REFERENCE, AND WHICH NOT
{
    try{
        //(toPrint, "Enter params\n");
        
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        if (category.compare("reference_op") == 0)
        {
            //(toPrint, "Token Match. Expected possible reference_op and got a " + token + "\n");
            
            currToken_index++; // go to expr
            Val term = parseExpr(toPrint); // assumed that parseExpr increases currToken_index
           //  term.type = "reference";
            term.byReference = true;
            
            paramList.push_back(term); // NEED TO CHANGE THIS FOR PASS BY REFERENCE
        }
        else
        {
            Val term = parseExpr(toPrint);
            term.byReference = false;
            
            paramList.push_back(term);
        }
        
        token = tokenList[currToken_index][0];
        category = tokenList[currToken_index][1];
        
        if (category.compare("comma_token") == 0) // allow for infinate number expression
        {
            //(toPrint, "Token Match. Expected possible comma_token and got a " + token + "\n");
            
            currToken_index++; // go to expr
            
            parseParams(toPrint, paramList); // assumed that parseExpr increases currToken_index
        }
        
        //(toPrint, "Exit params\n");
    } catch (string error) {
        throw error;
    }
}

// bool_exp
Val parseBool_exp(string & toPrint)
{

    //    add var handling

    Val term1;
    Val term2;
    bool isBang = false;

    try {
        append(toPrint, "Enter Bool_exp\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];



        if ( (category.compare("bang_op") == 0) || (category.compare("and_op") == 0)     ||
            (category.compare("or_op") == 0)      ||
            (category.compare("equal_op") == 0)   ||
            (category.compare("not_equal_op") == 0) )
        {
            
            if ((category.compare("bang_op") == 0)) {
                append(toPrint, "Token Match. Expected possible bang_op and got a " + token + "\n");
                currToken_index++;
                isBang = true;
            }
            
            

            term1 = parseRel_expr(toPrint);

            // look ahead for possible  {<bool_op> <rel_value>}
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];

            //            bool bang_op = true;

            if ( (category.compare("and_op") == 0)     ||
                (category.compare("or_op") == 0)      ||
                (category.compare("equal_op") == 0)   ||
                (category.compare("not_equal_op") == 0)  ) // bool op
            {

                //                bool_op.lexeme = category;
                //                bool_op.token = token;
                append(toPrint, "Token Match. Expected possible bool_op and got a " + token + "\n");
                currToken_index++;

                term2 = parseRel_expr(toPrint);

                if ( runtimeStack.hasVar(term2.token)) {

                    // check defined
                    if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                    {

                    }
                    else
                    {
                        throw "ERROR. Variable " + term2.token + " undefined.";
                    }

                }
                else
                {
                    // throw unkown var error
                    throw "ERROR. Unkown variable: " + term2.token;

                }


                // if term 1 var, need to use val AND set type and value back to undefined
                if (term1.lexeme.compare("var") == 0)
                {
                    if ( runtimeStack.hasVar(term1.token)) {

                        // check defined
                        if ( !(runtimeStack.getVar(term1.token).value.compare("undefined") == 0) )
                        {

                            // obtain term1: val, type
                            //                            test

                            if (term1.type.compare("integer") == 0)
                            {
                                int term1_int = stoi(term1.value);

                                if (term2.lexeme.compare("integer") == 0)
                                {
                                    if (category.compare("and_op") == 0)
                                    {
                                        bool result = ! (term1_int && stoi(term2.token));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("or_op") == 0)
                                    {
                                        bool result = ! (term1_int || stoi(term2.token));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("equal_op") == 0)
                                    {
                                        bool result = ! (term1_int == stoi(term2.token));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("not_equal_op") == 0)
                                    {
                                        bool result = ! (term1_int != stoi(term2.token));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                }
                                else if (term2.lexeme.compare("float") == 0)
                                {
                                    if (category.compare("and_op") == 0)
                                    {
                                        bool result = ! (term1_int && stod(term2.token));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("or_op") == 0)
                                    {
                                        bool result = ! (term1_int || stod(term2.token));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("equal_op") == 0)
                                    {
                                        bool result = ! (term1_int == stod(term2.token));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("not_equal_op") == 0)
                                    {
                                        bool result = ! (term1_int != stod(term2.token));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                }
                                else if (term2.lexeme.compare("string") == 0)
                                {
                                    if (category.compare("and_op") == 0)
                                    {
                                        bool result = ! term1_int;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("or_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("equal_op") == 0)
                                    {
                                        bool result = ! false;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("not_equal_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }

                                }
                                else if (term2.lexeme.compare("var") == 0)
                                {
                                    if ( runtimeStack.hasVar(term2.token)) {

                                        // check defined
                                        if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                        {
                                            if (term2.type.compare("integer") == 0)
                                            {
                                                if (category.compare("and_op") == 0)
                                                {
                                                    bool result = ! (term1_int && stoi(term2.value));
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("or_op") == 0)
                                                {
                                                    bool result = ! (term1_int || stoi(term2.value));
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("equal_op") == 0)
                                                {
                                                    bool result = ! (term1_int == stoi(term2.value));
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("not_equal_op") == 0)
                                                {
                                                    bool result = ! (term1_int != stoi(term2.value));
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                            }
                                            else if (term2.type.compare("float") == 0)
                                            {
                                                if (category.compare("and_op") == 0)
                                                {
                                                    bool result = ! (term1_int && stod(term2.value));
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("or_op") == 0)
                                                {
                                                    bool result = ! (term1_int || stod(term2.value));
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("equal_op") == 0)
                                                {
                                                    bool result = ! (term1_int == stod(term2.value));
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("not_equal_op") == 0)
                                                {
                                                    bool result = ! (term1_int != stod(term2.value));
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                            }
                                            else if (term2.type.compare("string") == 0)
                                            {
                                                if (category.compare("and_op") == 0)
                                                {
                                                    bool result = ! term1_int;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("or_op") == 0)
                                                {
                                                    bool result = ! true;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("equal_op") == 0)
                                                {
                                                    bool result = ! false;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("not_equal_op") == 0)
                                                {
                                                    bool result = ! true;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                            }
                                        }
                                        else
                                        {
                                            throw "ERROR. Variable " + term2.token + " undefined.";
                                        }

                                    }
                                    else
                                    {
                                        // throw unkown var error
                                        throw "ERROR. Unkown variable: " + term2.token;
                                    }
                                }
                            }
                            else if (term1.type.compare("float") == 0)
                            {
                                double term1_double = stod(term1.value);

                                if (term2.lexeme.compare("integer") == 0)
                                {

                                    if (category.compare("and_op") == 0)
                                    {
                                        bool result = ! false;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                    }
                                    else if (category.compare("or_op") == 0)
                                    {
                                        bool result = ! stoi(term2.token);
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                    }
                                    else if (category.compare("equal_op") == 0)
                                    {
                                        bool result = ! false;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                    }
                                    else if (category.compare("not_equal_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                    }

                                }
                                else if (term2.lexeme.compare("float") == 0)
                                {
                                    if (category.compare("and_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                    }
                                    else if (category.compare("or_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                    }
                                    else if (category.compare("equal_op") == 0)
                                    {
                                        bool result = ! (term1_double == stod(term2.token));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                    }
                                    else if (category.compare("not_equal_op") == 0)
                                    {
                                        bool result = ! (term1_double != stod(term2.token));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                    }
                                }
                                else if (term2.lexeme.compare("string") == 0)
                                {
                                    if (category.compare("and_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                    }
                                    else if (category.compare("or_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                    }
                                    else if (category.compare("equal_op") == 0)
                                    {
                                        bool result = ! false;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                    }
                                    else if (category.compare("not_equal_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                    }
                                }
                                else if (term2.lexeme.compare("var") == 0)
                                {
                                    if ( runtimeStack.hasVar(term2.token)) {

                                        // check defined
                                        if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                        {
                                            if (term2.type.compare("integer") == 0) // integer
                                            {
                                                if (category.compare("and_op") == 0)
                                                {
                                                    bool result = ! stoi(term2.value);
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                }
                                                else if (category.compare("or_op") == 0)
                                                {
                                                    bool result = ! true;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                }
                                                else if (category.compare("equal_op") == 0)
                                                {
                                                    bool result = ! false;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                }
                                                else if (category.compare("not_equal_op") == 0)
                                                {
                                                    bool result = ! true;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                            }
                                            else if (term2.type.compare("float") == 0)
                                            {
                                                if (category.compare("and_op") == 0)
                                                {
                                                    bool result = ! true;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("or_op") == 0)
                                                {
                                                    bool result = ! true;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("equal_op") == 0)
                                                {
                                                    bool result = ! (term1_double == stod(term2.value));
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("not_equal_op") == 0)
                                                {
                                                    bool result = ! (term1_double != stod(term2.value));
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                            }
                                            else if (term2.type.compare("string") == 0)
                                            {
                                                if (category.compare("and_op") == 0)
                                                {
                                                    bool result = ! term1_double;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("or_op") == 0)
                                                {
                                                    bool result = ! true;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("equal_op") == 0)
                                                {
                                                    bool result = ! false;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("not_equal_op") == 0)
                                                {
                                                    bool result = ! true;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                            }
                                        }
                                        else
                                        {
                                            throw "ERROR. Variable " + term2.token + " undefined.";
                                        }

                                    }
                                    else
                                    {
                                        // throw unkown var error
                                        throw "ERROR. Unkown variable: " + term2.token;
                                    }
                                }
                            }
                            else if (term1.type.compare("string") == 0)
                            {
                                string term1_string = term1.value;

                                if (term2.lexeme.compare("integer") == 0)
                                {
                                    if (category.compare("and_op") == 0)
                                    {
                                        bool result = ! stoi(term2.value);
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                    }
                                    else if (category.compare("or_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                    }
                                    else if (category.compare("equal_op") == 0)
                                    {
                                        bool result = ! false;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                    }
                                    else if (category.compare("not_equal_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }

                                }
                                else if (term2.lexeme.compare("float") == 0)
                                {
                                    if (category.compare("and_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("or_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("equal_op") == 0)
                                    {
                                        bool result = ! false;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("not_equal_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                }
                                else if (term2.lexeme.compare("string") == 0)
                                {
                                    if (category.compare("and_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("or_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("equal_op") == 0)
                                    {
                                        bool result = ! (term1.value.compare(term2.token) == 0);
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("not_equal_op") == 0)
                                    {
                                        bool result = ! (term1.value.compare(term2.token) != 0);
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                }
                                else if (term2.lexeme.compare("var") == 0)
                                {
                                    if ( runtimeStack.hasVar(term2.token)) {

                                        // check defined
                                        if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                        {
                                            if (term2.type.compare("integer") == 0) // integer
                                            {
                                                if (category.compare("and_op") == 0)
                                                {
                                                    bool result = ! stoi(term2.value);
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                }
                                                else if (category.compare("or_op") == 0)
                                                {
                                                    bool result = ! true;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                }
                                                else if (category.compare("equal_op") == 0)
                                                {
                                                    bool result = ! false;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                }
                                                else if (category.compare("not_equal_op") == 0)
                                                {
                                                    bool result = ! true;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                            }
                                            else if (term2.type.compare("float") == 0)
                                            {
                                                if (category.compare("and_op") == 0)
                                                {
                                                    bool result = ! true;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("or_op") == 0)
                                                {
                                                    bool result = ! true;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("equal_op") == 0)
                                                {
                                                    bool result = ! false;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("not_equal_op") == 0)
                                                {
                                                    bool result = ! true;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                            }
                                            else if (term2.type.compare("string") == 0) // special
                                            {
                                                if (category.compare("and_op") == 0)
                                                {
                                                    bool result = ! true;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("or_op") == 0)
                                                {
                                                    bool result = ! true;
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("equal_op") == 0)
                                                {
                                                    bool result = ! (term1.value.compare(term2.token) == 0);
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                                else if (category.compare("not_equal_op") == 0)
                                                {
                                                    bool result = ! (term1.value.compare(term2.token) != 0);
                                                    term1.token = to_string(result);
                                                    term1.lexeme = "bool";
                                                    term1.type = "undefined";
                                                    term1.value = "undefined";
                                                }
                                            }
                                        }
                                        else
                                        {
                                            throw "runtime error. Variable " + term2.token + " undefined.";
                                        }

                                    }
                                    else
                                    {
                                        // throw unkown var error
                                        throw "runtime error. Unkown variable: " + term2.token;
                                    }
                                }
                            }
                        }
                        else
                        {
                            throw "runtime error. Variable " + term1.token + " undefined.";
                        }

                    }
                    else
                    {
                        // throw unkown var error
                        throw "runtime error. Unkown variable: " + term1.token;

                    }

                }
                else if (term1.lexeme.compare("integer") == 0)
                {
                    int term1_int = stoi(term1.token);

                    if (term2.lexeme.compare("integer") == 0)
                    {
                        if (category.compare("and_op") == 0)
                        {
                            bool result = ! (term1_int && stoi(term2.token));
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("or_op") == 0)
                        {
                            bool result = ! (term1_int || stoi(term2.token));
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("equal_op") == 0)
                        {
                            bool result = ! (term1_int == stoi(term2.token));
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("not_equal_op") == 0)
                        {
                            bool result = ! (term1_int != stoi(term2.token));
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                    }
                    else if (term2.lexeme.compare("float") == 0)
                    {
                        if (category.compare("and_op") == 0)
                        {
                            bool result = ! (term1_int && stod(term2.token));
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("or_op") == 0)
                        {
                            bool result = ! (term1_int || stod(term2.token));
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("equal_op") == 0)
                        {
                            bool result = ! (term1_int == stod(term2.token));
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("not_equal_op") == 0)
                        {
                            bool result = ! (term1_int != stod(term2.token));
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                    }
                    else if (term2.lexeme.compare("var") == 0) // var
                    {
                        // check if var is double or int

                        //                            CHECK VAR in table
                        //                            check var defined

                        // check in table
                        if ( runtimeStack.hasVar(term2.token)) {

                            // check defined
                            if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) ) { // check that var is defined

                                if (term2.type.compare("integer") == 0)
                                {
                                    if (category.compare("and_op") == 0)
                                    {
                                        bool result = ! (term1_int && stoi(term2.value));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("or_op") == 0)
                                    {
                                        bool result = ! (term1_int || stoi(term2.value));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("equal_op") == 0)
                                    {
                                        bool result = ! (term1_int == stoi(term2.value));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("not_equal_op") == 0)
                                    {
                                        bool result = ! (term1_int != stoi(term2.value));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                }
                                else if (term2.type.compare("float") == 0)
                                {
                                    if (category.compare("and_op") == 0)
                                    {
                                        bool result = ! (term1_int && stod(term2.value));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("or_op") == 0)
                                    {
                                        bool result = ! (term1_int || stod(term2.value));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("equal_op") == 0)
                                    {
                                        bool result = ! (term1_int == stod(term2.value));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("not_equal_op") == 0)
                                    {
                                        bool result = ! (term1_int != stod(term2.value));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                }
                                else if (term2.type.compare("string") == 0)
                                {
                                    if (category.compare("and_op") == 0)
                                    {
                                        bool result = ! term1_int;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("or_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("equal_op") == 0)
                                    {
                                        bool result = ! false;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("not_equal_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                }
                            }
                            else
                            {
                                throw "runtime error. Variable " + term2.token + " undefined.";
                            }

                        }
                        else
                        {
                            // throw unkown var error
                            throw "runtime error. Unkown variable: " + term2.token;

                        }
                    }
                }
                else if (term1.lexeme.compare("float") == 0) // float
                {
                    double term1_double = stoi(term1.token);

                    if (term2.lexeme.compare("integer") == 0)
                    {
                        if (category.compare("and_op") == 0)
                        {
                            bool result = ! (term1_double && stoi(term2.token));
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("or_op") == 0)
                        {
                            bool result = ! (term1_double || stoi(term2.token));
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("equal_op") == 0)
                        {
                            bool result = ! (term1_double == stoi(term2.token));
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("not_equal_op") == 0)
                        {
                            bool result = ! (term1_double != stoi(term2.token));
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                    }
                    else if (term2.lexeme.compare("float") == 0)
                    {
                        if (category.compare("and_op") == 0)
                        {
                            bool result = ! (term1_double && stod(term2.token));
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("or_op") == 0)
                        {
                            bool result = ! (term1_double || stod(term2.token));
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("equal_op") == 0)
                        {
                            bool result = ! (term1_double == stod(term2.token));
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("not_equal_op") == 0)
                        {
                            bool result = ! (term1_double != stod(term2.token));
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                    }
                    else if (term2.lexeme.compare("var") == 0)
                    {
                        // check if var is double or int

                        // check if var in symbol table

                        //                            CHECK VAR in table
                        //                            check var defined

                        // check in table
                        if ( runtimeStack.hasVar(term2.token)) {

                            // check defined
                            if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                            {
                                if (term2.type.compare("integer") == 0)
                                {
                                    if (category.compare("and_op") == 0)
                                    {
                                        bool result = ! (term1_double && stoi(term2.value));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("or_op") == 0)
                                    {
                                        bool result = ! (term1_double || stoi(term2.value));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("equal_op") == 0)
                                    {
                                        bool result = ! (term1_double == stoi(term2.value));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("not_equal_op") == 0)
                                    {
                                        bool result = ! (term1_double != stoi(term2.value));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                }
                                else if (term2.type.compare("float") == 0)
                                {
                                    if (category.compare("and_op") == 0)
                                    {
                                        bool result = ! (term1_double && stod(term2.value));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("or_op") == 0)
                                    {
                                        bool result = ! (term1_double || stod(term2.value));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("equal_op") == 0)
                                    {
                                        bool result = ! (term1_double == stod(term2.value));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("not_equal_op") == 0)
                                    {
                                        bool result = ! (term1_double != stod(term2.value));
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                }
                                else if (term2.type.compare("string") == 0)
                                {
                                    if (category.compare("and_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("or_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("equal_op") == 0)
                                    {
                                        bool result = ! false;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("not_equal_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                }
                            }
                            else
                            {
                                throw "runtime error. Variable " + term2.token + " undefined.";
                            }
                        }
                        else
                        {
                            // throw unkown var error
                            throw "runtime error. Unkown variable: " + term2.token;
                        }
                    }
                }
                else if (term1.lexeme.compare("string") == 0)
                {
                    //                        string term1_string = term1.token;

                    if (term2.lexeme.compare("string") == 0)
                    {
                        //                            term1.token = "1";
                        //                            term1.lexeme = "bool";
                        if (category.compare("and_op") == 0)
                        {
                            bool result = ! true;
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("or_op") == 0)
                        {
                            bool result = ! true;
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("equal_op") == 0)
                        {
                            bool result = ! true;
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("not_equal_op") == 0)
                        {
                            bool result = ! false;
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                    }
                    else if (term2.lexeme.compare("integer") == 0)
                    {
                        if (category.compare("and_op") == 0)
                        {
                            bool result = ! true;
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("or_op") == 0)
                        {
                            bool result = ! true;
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("equal_op") == 0)
                        {
                            bool result = ! true;
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("not_equal_op") == 0)
                        {
                            bool result = ! false;
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                    }
                    else if (term2.lexeme.compare("float") == 0)
                    {
                        //                            bool result = term1_double == stod(term2.token);
                        if (category.compare("and_op") == 0)
                        {
                            bool result = ! true;
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("or_op") == 0)
                        {
                            bool result = ! true;
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("equal_op") == 0)
                        {
                            bool result = ! true;
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }
                        else if (category.compare("not_equal_op") == 0)
                        {
                            bool result = ! false;
                            term1.token = to_string(result);
                            term1.lexeme = "bool";
                            term1.type = "undefined";
                            term1.value = "undefined";
                        }

                    }
                    else if (term2.lexeme.compare("var") == 0)
                    {
                        // check if var is double or int

                        // check if var in symbol table

                        //                            CHECK VAR in table
                        //                            check var defined

                        // check in table
                        if ( runtimeStack.hasVar(term2.token)) {

                            // check defined
                            if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                            {
                                if (term2.type.compare("integer") == 0)
                                {
                                    if (category.compare("and_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("or_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("equal_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("not_equal_op") == 0)
                                    {
                                        bool result = ! false;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                }
                                else if (term2.type.compare("float") == 0)
                                {
                                    if (category.compare("and_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("or_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("equal_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("not_equal_op") == 0)
                                    {
                                        bool result = ! false;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                }
                                else if (term2.type.compare("string") == 0)
                                {
                                    if (category.compare("and_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("or_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("equal_op") == 0)
                                    {
                                        bool result = ! true;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                    else if (category.compare("not_equal_op") == 0)
                                    {
                                        bool result = ! false;
                                        term1.token = to_string(result);
                                        term1.lexeme = "bool";
                                        term1.type = "undefined";
                                        term1.value = "undefined";
                                    }
                                }
                            }
                            else
                            {
                                throw "runtime error. Variable " + term2.token + " undefined.";
                            }
                        }

                    }

                }

                
                if (! isBang) {
                    
                    int temp = stoi(term1.token);
                    
                    if (temp == 1) {
                        temp = 0;
                    }
                    else if (temp == 0)
                    {
                        temp = 1;
                    }
                    
                    term1.token = to_string(temp);
                    
                }
                
            }

        }
        else if ( (category.compare("integer") == 0) ||
                 (category.compare("single_quote") == 0) ||
                 (category.compare("float") == 0) ||
                 (category.compare("minus_op") == 0) ||
                 (category.compare("var") == 0) ||
                 (category.compare("p_left") == 0)  ) // tokens possible for first element in rel_value
        {
            term1 = parseRel_value(toPrint);

            // look ahead for possible  {<bool_op> <rel_value>}
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];

            if ( (category.compare("and_op") == 0)     ||
                (category.compare("or_op") == 0)      ||
                (category.compare("equal_op") == 0)   ||
                (category.compare("not_equal_op") == 0)  ) // bool op
            {
                append(toPrint, "Token Match. Expected possible bool_op and got a " + token + "\n");
                currToken_index++;
                term2 = parseRel_value(toPrint);

                // evalute

                if (category.compare("and_op") == 0)
                {
                    bool result = (stoi(term1.token) && stoi(term2.token));
                    term1.token = to_string(result);
                    term1.lexeme = "bool";
                }
                else if (category.compare("or_op") == 0)
                {
                    bool result = (stoi(term1.token) || stoi(term2.token));
                    term1.token = to_string(result);
                    term1.lexeme = "bool";
                }
                else if (category.compare("equal_op") == 0)
                {
                    bool result = (stoi(term1.token) == stoi(term2.token));
                    term1.token = to_string(result);
                    term1.lexeme = "bool";
                }
                else if (category.compare("not_equal_op") == 0)
                {
                    bool result = (stoi(term1.token) != stoi(term2.token));
                    term1.token = to_string(result);
                    term1.lexeme = "bool";
                }
            }
        }
        else
        {
            // error
            append(toPrint, "Token ERROR. Expected first term in rel_expr token (integer, string, float, minus_op, var) and got a " + token + "\n");

            throw "Token ERROR. Expected first term in rel_expr token (integer, string, float, minus_op, var) and got a " + token + "\n";
            //            return;
        }
        append(toPrint, "Exit Bool_exp\n");
        
    } catch (string error) {
        error;
    }

    return term1;
}

// rel_value
Val parseRel_value(string & toPrint)
{
    try{
        Val term1;
        Val term2;
        Val result;
        
        result.lexeme = "bool";
        
        try {
            //(toPrint, "Enter rel_value\n");
            string token = tokenList[currToken_index][0];
            string category = tokenList[currToken_index][1];
            //    currToken_index++;
            
            
            
            if ( (category.compare("integer") == 0) ||
                (category.compare("single_quote") == 0) ||
                (category.compare("float") == 0) ||
                (category.compare("minus_op") == 0) ||
                (category.compare("var") == 0) ||
                (category.compare("p_left") == 0) ) // tokens possible for first element in rel_expr
            {
                term1 = parseRel_expr(toPrint);
                
                // look ahead
                //currToken_index++;
                
                if ((currToken_index + 1) == tokenList.size())
                {
                    //currToken_index--;
                    return term1;
                    //                string error = "out of index in parseRel_value";
                    //                throw error;
                }
                
                
                // cout << "inside parseRel_value: " << toPrint << endl;
                
                token = tokenList[currToken_index][0];
                category = tokenList[currToken_index][1];
                //        currToken_index--;
                
                // check if next is rel_op
                if ( (category.compare("equal_op") == 0) )
                {
                    //(toPrint, "Token Match. Expected possible equal_op and got a " + token + "\n");
                    //parseRel_op(toPrint);
                    
                    // look ahead
                    currToken_index++;
                    token = tokenList[currToken_index][0];
                    category = tokenList[currToken_index][1];
                    
                    if ( (category.compare("integer") == 0) ||
                        (category.compare("single_quote") == 0) ||
                        (category.compare("float") == 0) ||
                        (category.compare("minus_op") == 0) ||
                        (category.compare("var") == 0) ||
                        (category.compare("p_left") == 0)  ) // tokens possible for first element in rel_expr
                    {
                        //        append(toPrint, "Token Match. Expected - and got a " + token + "\n");
                        //currToken_index++;
                        term2 = parseRel_expr(toPrint);
                        
                        //                    add var
                        
                        if (term1.lexeme.compare("integer") == 0)
                        {
                            int term1_int = stoi(term1.token);
                            
                            if (term2.lexeme.compare("integer") == 0)
                            {
                                bool result = term1_int == stoi(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                            }
                            else if (term2.lexeme.compare("float") == 0)
                            {
                                bool result = term1_int == stod(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                            }
                            else if (term2.lexeme.compare("var") == 0) // var
                            {
                                // check if var is double or int
                                
                                //                            CHECK VAR in table
                                //                            check var defined
                                
                                // check in table
                                if ( runtimeStack.hasVar(term2.token)) {
                                    
                                    // check defined
                                    if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) ) { // check that var is defined
                                        
                                        if (term2.type.compare("integer") == 0)
                                        {
                                            bool result = term1_int == stoi(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.type.compare("float") == 0)
                                        {
                                            bool result = term1_int == stod(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.type.compare("string") == 0)
                                        {
                                            // throw error
                                            term1.token = "0";
                                            term1.lexeme = "bool";
                                        }
                                    }
                                    else
                                    {
                                        throw "runtime error. cariable " + term2.token + " undefined.";
                                    }
                                    
                                }
                                else
                                {
                                    // throw unkown var error
                                    throw "runtime error. unkown variable: " + term2.token;
                                    
                                }
                            }
                        }
                        else if (term1.lexeme.compare("float") == 0) // float
                        {
                            double term1_double = stoi(term1.token);
                            
                            if (term2.lexeme.compare("integer") == 0)
                            {
                                bool result = term1_double == stoi(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                            }
                            else if (term2.lexeme.compare("float") == 0)
                            {
                                bool result = term1_double == stod(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                            }
                            else if (term2.lexeme.compare("var") == 0)
                            {
                                // check if var is double or int
                                
                                // check if var in symbol table
                                
                                //                            CHECK VAR in table
                                //                            check var defined
                                
                                // check in table
                                if ( runtimeStack.hasVar(term2.token)) {
                                    
                                    // check defined
                                    if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                    {
                                        if (term2.type.compare("integer") == 0)
                                        {
                                            bool result = term1_double == stoi(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.type.compare("float") == 0)
                                        {
                                            bool result = term1_double == stod(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.type.compare("string") == 0)
                                        {
                                            // throw error
                                            term1.token = "0";
                                            term1.lexeme = "bool";
                                        }
                                    }
                                    else
                                    {
                                        throw "runtime error. variable " + term2.token + " undefined.";
                                    }
                                }
                                else
                                {
                                    // throw unkown var error
                                    throw "runtime error. unkown variable: " + term2.token;
                                }
                            }
                        }
                        else if (term1.lexeme.compare("string") == 0)
                        {
                            //                        string term1_string = term1.token;
                            
                            if (term2.lexeme.compare("string") == 0)
                            {
                                if (term2.token.compare(term1.token) == 0)
                                {
                                    term1.token = "1";
                                    term1.lexeme = "bool";
                                }
                                else
                                {
                                    term1.token = "0";
                                    term1.lexeme = "bool";
                                }
                            }
                            else if (term2.lexeme.compare("integer") == 0)
                            {
                                term1.token = "0";
                                term1.lexeme = "bool";
                            }
                            else if (term2.lexeme.compare("float") == 0)
                            {
                                //                            bool result = term1_double == stod(term2.token);
                                term1.token = "0";          // to_string(result);
                                term1.lexeme = "bool";
                            }
                            else if (term2.lexeme.compare("var") == 0)
                            {
                                // check if var is double or int
                                
                                // check if var in symbol table
                                
                                //                            CHECK VAR in table
                                //                            check var defined
                                
                                // check in table
                                if ( runtimeStack.hasVar(term2.token)) {
                                    
                                    // check defined
                                    if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                    {
                                        if (term2.type.compare("integer") == 0)
                                        {
                                            term1.token = "0";
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.type.compare("float") == 0)
                                        {
                                            term1.token = "0";
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.type.compare("string") == 0)
                                        {
                                            if (term2.token.compare(term1.token) == 0)
                                            {
                                                term1.token = "1";
                                                term1.lexeme = "bool";
                                            }
                                            else
                                            {
                                                term1.token = "0";
                                                term1.lexeme = "bool";
                                            }
                                        }
                                    }
                                    else
                                    {
                                        throw "runtime error. variable " + term2.token + " undefined.";
                                    }
                                }
                                else
                                {
                                    throw "runtime error. unkown variable " + term2.token;
                                }
                                
                            }
                            
                        }
                        else if (term1.lexeme.compare("var") == 0)
                        {
                            
                            //                        unkown test
                            
                            if ( runtimeStack.hasVar(term1.token))
                            {
                                if ( ! (runtimeStack.getVar(term1.token).value.compare("undefined") == 0) ) // check that var is defined
                                {
                                    if (term1.type.compare("integer") == 0)
                                    {
                                        int term1_int = stoi(term1.value); // VAL is int
                                        
                                        if (term2.lexeme.compare("integer") == 0)
                                        {
                                            bool result = term1_int == stoi(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                            term1.type = "undefined";
                                            term1.value = "undefined";
                                        }
                                        else if (term2.lexeme.compare("float") == 0)
                                        {
                                            bool result = term1_int == stod(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                            term1.type = "undefined";
                                            term1.value = "undefined";
                                        }
                                        else if (term2.lexeme.compare("var") == 0)
                                        {
                                            // check if var is in symbol table (else throw error
                                            
                                            // then check if var is double or int
                                            
                                            //                                    CHECK VAR in table
                                            //                                    check var defined
                                            
                                            // check in table
                                            if ( runtimeStack.hasVar(term2.token)) {
                                                
                                                // check defined
                                                if ( ! (runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                                {
                                                    if (term2.type.compare("integer") == 0)
                                                    {
                                                        bool result = term1_int == stoi(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                        term1.type = "undefined";
                                                        term1.value = "undefined";
                                                    }
                                                    else if (term2.type.compare("float") == 0)
                                                    {
                                                        bool result = term1_int == stod(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                        term1.type = "undefined";
                                                        term1.value = "undefined";
                                                    }
                                                    else if (term2.type.compare("string") == 0)
                                                    {
                                                        term1.token = "0";
                                                        term1.lexeme = "bool";
                                                        term1.type = "undefined";
                                                        term1.value = "undefined";
                                                    }
                                                }
                                                else
                                                {
                                                    throw "ERROR. Variable " + term2.token + " undefined.";
                                                }
                                                
                                            }
                                            else
                                            {
                                                // throw unkown var error
                                                throw "ERROR. Unkown variable: " + term2.token;
                                                
                                            }
                                        }
                                    }                        // else if type double
                                    else if (term1.type.compare("float") == 0)
                                    {
                                        double term1_double = stoi(term1.token);
                                        
                                        if (term2.lexeme.compare("integer") == 0)
                                        {
                                            bool result = term1_double == stoi(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                            term1.type = "undefined";
                                            term1.value = "undefined";
                                        }
                                        else if (term2.lexeme.compare("float") == 0)
                                        {
                                            bool result = term1_double == stod(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                            term1.type = "undefined";
                                            term1.value = "undefined";
                                        }
                                        else if (term2.lexeme.compare("var") == 0)
                                        {
                                            // check if var is double or int
                                            
                                            //                                    CHECK VAR in table
                                            //                                    check var defined
                                            
                                            // check in table
                                            if ( runtimeStack.hasVar(term2.token)) {
                                                
                                                // check defined
                                                if ( ! (runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                                {
                                                    if (term2.type.compare("integer") == 0)
                                                    {
                                                        bool result = term1_double == stoi(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                        term1.type = "undefined";
                                                        term1.value = "undefined";
                                                    }
                                                    else if (term2.type.compare("float") == 0)
                                                    {
                                                        bool result = term1_double == stod(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                        term1.type = "undefined";
                                                        term1.value = "undefined";
                                                    }
                                                    else if (term2.type.compare("string") == 0)
                                                    {
                                                        // throw error
                                                        term1.token = "0";
                                                        term1.lexeme = "bool";
                                                        term1.type = "undefined";
                                                        term1.value = "undefined";
                                                    }
                                                }
                                                else
                                                {
                                                    throw "ERROR. Variable " + term2.token + " undefined.";
                                                }
                                                
                                            }
                                            else
                                            {
                                                // throw unkown var error
                                                throw "ERROR. Unkown variable: " + term2.token;
                                                
                                            }
                                        }
                                    }
                                    else if (term1.type.compare("string") == 0) // string error?
                                    {
                                        
                                        if (term2.token.compare(term1.token) == 0)
                                        {
                                            term1.token = "1";
                                            term1.lexeme = "bool";
                                            term1.type = "undefined";
                                            term1.value = "undefined";
                                        }
                                        else
                                        {
                                            term1.token = "0";
                                            term1.lexeme = "bool";
                                            term1.type = "undefined";
                                            term1.value = "undefined";
                                        }
                                    }
                                }
                                else
                                {
                                    // throw error
                                    throw "runtime error. variable " + term1.token + " undefined.";
                                }
                            }
                            else
                            {
                                // throw unkown var error
                                throw "runtime error. unkown variable: " + term1.token;
                            }
                        }
                    }
                    else
                    {
                        // error
                        throw "compiler error. Expected first term in rel_expr token (integer, string, float, minus_op, var) and got a " + token + "\n";
                        //                    return;
                    }
                }
            }
            else
            {
                // error
                throw "compiler error. Expected first term in rel_expr token (integer, string, float, minus_op, var) and got a " + token + "\n";
                
                //            return;
            }
            //(toPrint, "Exit rel_value\n");
            
        } catch (string error) {
            throw error;
        }
        
        return term1;
    } catch (string error) {
        throw error;
    }
}

// rel_expr
Val parseRel_expr(string & toPrint)
{
    try {
        //(toPrint, "Enter rel_expr\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        //    currToken_index++;
        
        // cout << "inside parseRel_expr: " << toPrint << endl;
        
        Val term1;
        Val term2;
        
        if ( (category.compare("integer") == 0) ||
            (category.compare("single_quote") == 0) ||
            (category.compare("float") == 0) ||
            (category.compare("minus_op") == 0) ||
            (category.compare("var") == 0) ||
            (category.compare("p_left") == 0) ) // tokens possible for first element in term
        {
            //        append(toPrint, "Token Match. Expected - and got a " + token + "\n");
            
            term1 = parseTerm(toPrint); // assumed: currToken_index incremented
            
            // look ahead
            // currToken_index++;
            
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            //        currToken_index--;
            
            // check if next is rel_op
            if ( (category.compare("less_than") == 0)  ||
                (category.compare("greater_than") == 0)   ||
                (category.compare("less_than_equal") == 0)   ||
                (category.compare("greater_than_equal") == 0)   )
            {
                Val rel_op = parseRel_op(toPrint);
                
                // look ahead
                //currToken_index++;
                token = tokenList[currToken_index][0];
                category = tokenList[currToken_index][1];
                
                if ( (category.compare("integer") == 0) ||
                    (category.compare("single_quote") == 0) ||
                    (category.compare("float") == 0) ||
                    (category.compare("minus_op") == 0) ||
                    (category.compare("var") == 0) ||
                    (category.compare("p_left") == 0) ) // tokens possible for first element in term
                {
                    //        append(toPrint, "Token Match. Expected - and got a " + token + "\n");
                    // currToken_index++;
                    term2 = parseTerm(toPrint);
                    
                    
                    if (rel_op.token.compare("<") == 0)
                    {
                        if (term1.lexeme.compare("integer") == 0)
                        {
                            int term1_int = stoi(term1.token);
                            
                            if (term2.lexeme.compare("integer") == 0)
                            {
                                bool result = term1_int < stoi(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                            }
                            else if (term2.lexeme.compare("float") == 0)
                            {
                                bool result = term1_int < stod(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                            }
                            else if (term2.lexeme.compare("var") == 0)
                            {
                                // check if var is double or int
                                
                                //                            CHECK VAR in table
                                //                            check var defined
                                
                                // check in table
                                if ( runtimeStack.hasVar(term2.token)) {
                                    
                                    // check defined
                                    if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) ) { // check that var is defined
                                        
                                        if (term2.type.compare("integer") == 0)
                                        {
                                            bool result = term1_int < stoi(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.type.compare("float") == 0)
                                        {
                                            bool result = term1_int < stod(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.type.compare("string") == 0)
                                        {
                                            // throw error
                                        }
                                    }
                                    else
                                    {
                                        throw "runtime error. variable " + term2.token + " undefined.";
                                    }
                                    
                                }
                                else
                                {
                                    // throw unkown var error
                                    throw "runtime error. unkown variable: " + term2.token;
                                    
                                }
                            }
                        }
                        else if (term1.lexeme.compare("float") == 0)
                        {
                            double term1_double = stoi(term1.token);
                            
                            if (term2.lexeme.compare("integer") == 0)
                            {
                                bool result = term1_double < stoi(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                            }
                            else if (term2.lexeme.compare("float") == 0)
                            {
                                bool result = term1_double < stod(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                            }
                            else if (term2.lexeme.compare("var") == 0)
                            {
                                // check if var is double or int
                                
                                // check if var in symbol table
                                
                                //                            CHECK VAR in table
                                //                            check var defined
                                
                                // check in table
                                if ( runtimeStack.hasVar(term2.token)) {
                                    
                                    // check defined
                                    if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                    {
                                        if (term2.type.compare("integer") == 0)
                                        {
                                            bool result = term1_double < stoi(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.type.compare("float") == 0)
                                        {
                                            bool result = term1_double < stod(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.type.compare("string") == 0)
                                        {
                                            // throw error
                                        }
                                    }
                                    else
                                    {
                                        throw "runtime error. variable " + term2.token + " undefined.";
                                    }
                                }
                                else
                                {
                                    // throw unkown var error
                                    throw "runtime error. unkown variable: " + term2.token;
                                }
                            }
                        }
                        else if (term1.lexeme.compare("var") == 0)
                        {
                            
                            //                        unkown test
                            
                            if ( runtimeStack.hasVar(term1.token))
                            {
                                if ( ! (runtimeStack.getVar(term1.token).value.compare("undefined") == 0) ) // check that var is defined
                                {
                                    if (term1.type.compare("integer") == 0)
                                    {
                                        int term1_int = stoi(term1.value);
                                        
                                        if (term2.lexeme.compare("integer") == 0)
                                        {
                                            bool result = term1_int < stoi(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.lexeme.compare("float") == 0)
                                        {
                                            bool result = term1_int < stod(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.lexeme.compare("var") == 0)
                                        {
                                            // check if var is in symbol table (else throw error
                                            
                                            // then check if var is double or int
                                            
                                            //                                    CHECK VAR in table
                                            //                                    check var defined
                                            
                                            // check in table
                                            if ( runtimeStack.hasVar(term2.token)) {
                                                
                                                // check defined
                                                if ( ! (runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                                {
                                                    if (term2.type.compare("integer") == 0)
                                                    {
                                                        bool result = term1_int < stoi(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                    }
                                                    else if (term2.type.compare("float") == 0)
                                                    {
                                                        bool result = term1_int < stod(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                    }
                                                    else if (term2.type.compare("string") == 0)
                                                    {
                                                        // throw error
                                                    }
                                                }
                                                else
                                                {
                                                    throw "ERROR. Variable " + term2.token + " undefined.";
                                                }
                                                
                                            }
                                            else
                                            {
                                                // throw unkown var error
                                                throw "ERROR. Unkown variable: " + term2.token;
                                                
                                            }
                                        }
                                    }                        // else if type double
                                    else if (term1.type.compare("float") == 0)
                                    {
                                        double term1_double = stoi(term1.token);
                                        
                                        if (term2.lexeme.compare("integer") == 0)
                                        {
                                            bool result = term1_double < stoi(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.lexeme.compare("float") == 0)
                                        {
                                            bool result = term1_double < stod(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.lexeme.compare("var") == 0)
                                        {
                                            // check if var is double or int
                                            
                                            //                                    CHECK VAR in table
                                            //                                    check var defined
                                            
                                            // check in table
                                            if ( runtimeStack.hasVar(term2.token)) {
                                                
                                                // check defined
                                                if ( ! (runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                                {
                                                    if (term2.type.compare("integer") == 0)
                                                    {
                                                        bool result = term1_double < stoi(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                    }
                                                    else if (term2.type.compare("float") == 0)
                                                    {
                                                        bool result = term1_double < stod(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                    }
                                                    else if (term2.type.compare("string") == 0)
                                                    {
                                                        // throw error
                                                    }
                                                }
                                                else
                                                {
                                                    throw "ERROR. Variable " + term2.token + " undefined.";
                                                }
                                                
                                            }
                                            else
                                            {
                                                // throw unkown var error
                                                throw "ERROR. Unkown variable: " + term2.token;
                                                
                                            }
                                            
                                            
                                        }
                                    }
                                    else if (term1.type.compare("string") == 0) // string error?
                                    {
                                        
                                    }
                                }
                                else
                                {
                                    // throw error
                                    throw "runtime error. variable " + term1.token + " undefined.";
                                }
                            }
                            else
                            {
                                // throw unkown var error
                                throw "runtime error. unkown variable: " + term1.token;
                            }
                            
                            
                        }
                    }
                    else if (rel_op.token.compare(">") == 0)
                    {
                        if (term1.lexeme.compare("integer") == 0)
                        {
                            int term1_int = stoi(term1.token);
                            
                            if (term2.lexeme.compare("integer") == 0)
                            {
                                bool result = term1_int > stoi(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                            }
                            else if (term2.lexeme.compare("float") == 0)
                            {
                                bool result = term1_int > stod(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                            }
                            else if (term2.lexeme.compare("var") == 0)
                            {
                                // check if var is double or int
                                
                                //                            CHECK VAR in table
                                //                            check var defined
                                
                                // check in table
                                if ( runtimeStack.hasVar(term2.token)) {
                                    
                                    // check defined
                                    if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) ) { // check that var is defined
                                        
                                        if (term2.type.compare("integer") == 0)
                                        {
                                            bool result = term1_int > stoi(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.type.compare("float") == 0)
                                        {
                                            bool result = term1_int > stod(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.type.compare("string") == 0)
                                        {
                                            // throw error
                                        }
                                    }
                                    else
                                    {
                                        throw "runtime error. variable " + term2.token + " undefined.";
                                    }
                                    
                                }
                                else
                                {
                                    // throw unkown var error
                                    throw "runtime error. unkown variable: " + term2.token;
                                    
                                }
                                
                                
                            }
                        }
                        else if (term1.lexeme.compare("float") == 0)
                        {
                            double term1_double = stoi(term1.token);
                            
                            if (term2.lexeme.compare("integer") == 0)
                            {
                                bool result = term1_double > stoi(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                            }
                            else if (term2.lexeme.compare("float") == 0)
                            {
                                bool result = term1_double > stod(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                            }
                            else if (term2.lexeme.compare("var") == 0)
                            {
                                // check if var is double or int
                                
                                // check if var in symbol table
                                
                                //                            CHECK VAR in table
                                //                            check var defined
                                
                                // check in table
                                if ( runtimeStack.hasVar(term2.token)) {
                                    
                                    // check defined
                                    if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                    {
                                        if (term2.type.compare("integer") == 0)
                                        {
                                            bool result = term1_double > stoi(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.type.compare("float") == 0)
                                        {
                                            bool result = term1_double > stod(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.type.compare("string") == 0)
                                        {
                                            // throw error
                                        }
                                    }
                                    else
                                    {
                                        throw "runtime error. variable " + term2.token + " undefined.";
                                    }
                                    
                                }
                                else
                                {
                                    // throw unkown var error
                                    throw "runtime error. unkown variable: " + term2.token;
                                    
                                }
                                
                                
                            }
                        }
                        else if (term1.lexeme.compare("var") == 0)
                        {
                            //                        unkown test
                            
                            if ( runtimeStack.hasVar(term1.token))
                            {
                                if ( ! (runtimeStack.getVar(term1.token).value.compare("undefined") == 0) ) // check that var is defined
                                {
                                    if (term1.type.compare("integer") == 0)
                                    {
                                        int term1_int = stoi(term1.value);
                                        
                                        if (term2.lexeme.compare("integer") == 0)
                                        {
                                            bool result = term1_int > stoi(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.lexeme.compare("float") == 0)
                                        {
                                            bool result = term1_int > stod(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.lexeme.compare("var") == 0)
                                        {
                                            // check if var is in symbol table (else throw error
                                            
                                            // then check if var is double or int
                                            
                                            //                                    CHECK VAR in table
                                            //                                    check var definedb
                                            
                                            // check in table
                                            if ( runtimeStack.hasVar(term2.token)) {
                                                
                                                // check defined
                                                if ( ! (runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                                {
                                                    if (term2.type.compare("integer") == 0)
                                                    {
                                                        bool result = term1_int > stoi(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                    }
                                                    else if (term2.type.compare("float") == 0)
                                                    {
                                                        bool result = term1_int > stod(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                    }
                                                    else if (term2.type.compare("string") == 0)
                                                    {
                                                        // throw error
                                                    }
                                                }
                                                else
                                                {
                                                    throw "ERROR. Variable " + term2.token + " undefined.";
                                                }
                                            }
                                            else
                                            {
                                                // throw unkown var error
                                                throw "ERROR. Unkown variable: " + term2.token;
                                                
                                            }
                                            
                                            
                                        }
                                    }                        // else if type double
                                    else if (term1.type.compare("float") == 0)
                                    {
                                        double term1_double = stoi(term1.token);
                                        
                                        if (term2.lexeme.compare("integer") == 0)
                                        {
                                            bool result = term1_double > stoi(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.lexeme.compare("float") == 0)
                                        {
                                            bool result = term1_double > stod(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.lexeme.compare("var") == 0)
                                        {
                                            // check if var is double or int
                                            
                                            //                                    CHECK VAR in table
                                            //                                    check var defined
                                            
                                            // check in table
                                            if ( runtimeStack.hasVar(term2.token)) {
                                                
                                                // check defined
                                                if ( ! (runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                                {
                                                    if (term2.type.compare("integer") == 0)
                                                    {
                                                        bool result = term1_double > stoi(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                    }
                                                    else if (term2.type.compare("float") == 0)
                                                    {
                                                        bool result = term1_double > stod(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                    }
                                                    else if (term2.type.compare("string") == 0)
                                                    {
                                                        // throw error
                                                    }
                                                }
                                                else
                                                {
                                                    throw "ERROR. Variable " + term2.token + " undefined.";
                                                }
                                                
                                            }
                                            else
                                            {
                                                // throw unkown var error
                                                throw "ERROR. Unkown variable: " + term2.token;
                                                
                                            }
                                            
                                            
                                        }
                                    }
                                    else if (term1.type.compare("string") == 0) // string error?
                                    {
                                        
                                    }
                                }
                                else
                                {
                                    // throw error
                                    throw "runtime error. variable " + term1.token + " undefined.";
                                }
                            }
                            else
                            {
                                // throw unkown var error
                                throw "runtime error. unkown variable: " + term1.token;
                            }
                            
                            
                        }
                    }
                    else if (rel_op.token.compare("<=") == 0)
                    {
                        //                    if (term2.lexeme.compare("integer") == 0)
                        //                    {
                        //                        bool result = stoi(term1.token) >= stoi(term2.token);
                        //                        term1.token = to_string(result);
                        //                        term1.lexeme = "bool";
                        //                    }
                        //                    else if (term2.lexeme.compare("float") == 0)
                        //                    {
                        //                        bool result = stod(term1.token) >= stod(term2.token);
                        //                        term1.token = to_string(result);
                        //                    }
                        if (term1.lexeme.compare("integer") == 0)
                        {
                            int term1_int = stoi(term1.token);
                            
                            if (term2.lexeme.compare("integer") == 0)
                            {
                                bool result = term1_int <= stoi(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                            }
                            else if (term2.lexeme.compare("float") == 0)
                            {
                                bool result = term1_int <= stod(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                            }
                            else if (term2.lexeme.compare("var") == 0)
                            {
                                // check if var is double or int
                                
                                //                            CHECK VAR in table
                                //                            check var defined
                                
                                // check in table
                                if ( runtimeStack.hasVar(term2.token)) {
                                    
                                    if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                    { // check that var is defined
                                        
                                        if (term2.type.compare("integer") == 0)
                                        {
                                            bool result = term1_int <= stoi(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.type.compare("float") == 0)
                                        {
                                            bool result = term1_int <= stod(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.type.compare("string") == 0)
                                        {
                                            // throw error
                                        }
                                    }
                                    else
                                    {
                                        throw "runtime error. variable " + term2.token + " undefined.";
                                    }
                                    
                                }
                                else
                                {
                                    // throw unkown var error
                                    throw "runtime error. unkown variable: " + term2.token;
                                }
                            }
                        }
                        else if (term1.lexeme.compare("float") == 0)
                        {
                            double term1_double = stoi(term1.token);
                            
                            if (term2.lexeme.compare("integer") == 0)
                            {
                                bool result = term1_double <= stoi(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                            }
                            else if (term2.lexeme.compare("float") == 0)
                            {
                                bool result = term1_double <= stod(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                            }
                            else if (term2.lexeme.compare("var") == 0)
                            {
                                // check if var is double or int
                                
                                // check if var in symbol table
                                
                                //                            CHECK VAR in table
                                //                            check var defined
                                
                                // check in table
                                if ( runtimeStack.hasVar(term2.token)) {
                                    
                                    // check defined
                                    if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                    {
                                        if (term2.type.compare("integer") == 0)
                                        {
                                            bool result = term1_double <= stoi(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.type.compare("float") == 0)
                                        {
                                            bool result = term1_double <= stod(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                        }
                                        else if (term2.type.compare("string") == 0)
                                        {
                                            // throw error
                                        }
                                    }
                                    else
                                    {
                                        throw "runtime error. variable " + term2.token + " undefined.";
                                    }
                                    
                                }
                                else
                                {
                                    // throw unkown var error
                                    throw "runtime error. unkown variable: " + term2.token;
                                    
                                }
                                
                            }
                        }
                        else if (term1.lexeme.compare("var") == 0)
                        {
                            
                            //                        unkown test
                            
                            if ( runtimeStack.hasVar(term1.token))
                            {
                                if ( ! (runtimeStack.getVar(term1.token).value.compare("undefined") == 0) ) // check that var is defined
                                {
                                    if (term1.type.compare("integer") == 0)
                                    {
                                        int term1_int = stoi(term1.value);
                                        
                                        if (term2.lexeme.compare("integer") == 0)
                                        {
                                            bool result = term1_int <= stoi(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                            term1.type = "undefined";
                                            term1.value = "undefined";
                                        }
                                        else if (term2.lexeme.compare("float") == 0)
                                        {
                                            bool result = term1_int <= stod(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                            term1.type = "undefined";
                                            term1.value = "undefined";
                                        }
                                        else if (term2.lexeme.compare("var") == 0)
                                        {
                                            // check if var is in symbol table (else throw error
                                            
                                            // then check if var is double or int
                                            
                                            //                                    CHECK VAR in table
                                            //                                    check var defined
                                            
                                            // check in table
                                            if ( runtimeStack.hasVar(term2.token)) {
                                                
                                                // check defined
                                                if ( ! (runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                                {
                                                    if (term2.type.compare("integer") == 0)
                                                    {
                                                        bool result = term1_int <= stoi(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                        term1.type = "undefined";
                                                        term1.value = "undefined";
                                                    }
                                                    else if (term2.type.compare("float") == 0)
                                                    {
                                                        bool result = term1_int <= stod(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                        term1.type = "undefined";
                                                        term1.value = "undefined";
                                                    }
                                                    else if (term2.type.compare("string") == 0)
                                                    {
                                                        // throw error
                                                    }
                                                }
                                                else
                                                {
                                                    throw "ERROR. Variable " + term2.token + " undefined.";
                                                }
                                                
                                            }
                                            else
                                            {
                                                // throw unkown var error
                                                throw "ERROR. Unkown variable: " + term2.token;
                                                
                                            }
                                            
                                            
                                        }
                                    }                        // else if type double
                                    else if (term1.type.compare("float") == 0)
                                    {
                                        double term1_double = stoi(term1.token);
                                        
                                        if (term2.lexeme.compare("integer") == 0)
                                        {
                                            bool result = term1_double <= stoi(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                            term1.type = "undefined";
                                            term1.value = "undefined";
                                        }
                                        else if (term2.lexeme.compare("float") == 0)
                                        {
                                            bool result = term1_double <= stod(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                            term1.type = "undefined";
                                            term1.value = "undefined";
                                        }
                                        else if (term2.lexeme.compare("var") == 0)
                                        {
                                            // check if var is double or int
                                            
                                            //                                    CHECK VAR in table
                                            //                                    check var defined
                                            
                                            // check in table
                                            if ( runtimeStack.hasVar(term2.token)) {
                                                
                                                // check defined
                                                if ( ! (runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                                {
                                                    if (term2.type.compare("integer") == 0)
                                                    {
                                                        bool result = term1_double <= stoi(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                        term1.type = "undefined";
                                                        term1.value = "undefined";
                                                    }
                                                    else if (term2.type.compare("float") == 0)
                                                    {
                                                        bool result = term1_double <= stod(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                        term1.type = "undefined";
                                                        term1.value = "undefined";
                                                    }
                                                    else if (term2.type.compare("string") == 0)
                                                    {
                                                        // throw error
                                                    }
                                                }
                                                else
                                                {
                                                    throw "runtime error. variable " + term2.token + " undefined.";
                                                }
                                                
                                            }
                                            else
                                            {
                                                // throw unkown var error
                                                throw "runtime error. unkown variable: " + term2.token;
                                                
                                            }
                                            
                                            
                                        }
                                    }
                                    else if (term1.type.compare("string") == 0) // string error?
                                    {
                                        
                                    }
                                }
                                else
                                {
                                    // throw error
                                    throw "runtime error. variable " + term1.token + " undefined.";
                                }
                                
                            }
                            else
                            {
                                // throw unkown var error
                                throw "runtime error. unkown variable: " + term1.token;
                            }
                            
                        }
                    }
                    else if (rel_op.token.compare(">=") == 0)
                    {
                        if (term1.lexeme.compare("integer") == 0)
                        {
                            int term1_int = stoi(term1.token);
                            
                            if (term2.lexeme.compare("integer") == 0)
                            {
                                bool result = term1_int >= stoi(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                                term1.type = "undefined";
                                term1.value = "undefined";
                            }
                            else if (term2.lexeme.compare("float") == 0)
                            {
                                bool result = term1_int >= stod(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                                term1.type = "undefined";
                                term1.value = "undefined";
                            }
                            else if (term2.lexeme.compare("var") == 0)
                            {
                                // check if var is double or int
                                
                                //                            CHECK VAR in table
                                //                            check var defined
                                
                                // check in table
                                if ( runtimeStack.hasVar(term2.token)) {
                                    
                                    // check defined
                                    if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) ) { // check that var is defined
                                        
                                        if (term2.type.compare("integer") == 0)
                                        {
                                            bool result = term1_int >= stoi(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                            term1.type = "undefined";
                                            term1.value = "undefined";
                                        }
                                        else if (term2.type.compare("float") == 0)
                                        {
                                            bool result = term1_int >= stod(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                            term1.type = "undefined";
                                            term1.value = "undefined";
                                        }
                                        else if (term2.type.compare("string") == 0)
                                        {
                                            // throw error
                                        }
                                    }
                                    else
                                    {
                                        throw "runtime error. variable " + term2.token + " undefined.";
                                    }
                                    
                                }
                                else
                                {
                                    // throw unkown var error
                                    throw "runtime error. unkown variable: " + term2.token;
                                    
                                }
                                
                                
                            }
                        }
                        else if (term1.lexeme.compare("float") == 0)
                        {
                            double term1_double = stoi(term1.token);
                            
                            if (term2.lexeme.compare("integer") == 0)
                            {
                                bool result = term1_double >= stoi(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                                term1.type = "undefined";
                                term1.value = "undefined";
                            }
                            else if (term2.lexeme.compare("float") == 0)
                            {
                                bool result = term1_double >= stod(term2.token);
                                term1.token = to_string(result);
                                term1.lexeme = "bool";
                                term1.type = "undefined";
                                term1.value = "undefined";
                            }
                            else if (term2.lexeme.compare("var") == 0)
                            {
                                // check if var is double or int
                                
                                // check if var in symbol table
                                
                                //                            CHECK VAR in table
                                //                            check var defined
                                
                                // check in table
                                if ( runtimeStack.hasVar(term2.token)) {
                                    
                                    // check defined
                                    if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                    {
                                        if (term2.type.compare("integer") == 0)
                                        {
                                            bool result = term1_double >= stoi(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                            term1.type = "undefined";
                                            term1.value = "undefined";
                                        }
                                        else if (term2.type.compare("float") == 0)
                                        {
                                            bool result = term1_double >= stod(term2.value);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                            term1.type = "undefined";
                                            term1.value = "undefined";
                                        }
                                        else if (term2.type.compare("string") == 0)
                                        {
                                            // throw error
                                        }
                                    }
                                    else
                                    {
                                        throw "runtime error. variable " + term2.token + " undefined.";
                                    }
                                    
                                }
                                else
                                {
                                    // throw unkown var error
                                    throw "runtime error. unkown variable: " + term2.token;
                                    
                                }
                                
                            }
                        }
                        else if (term1.lexeme.compare("var") == 0)
                        {
                            
                            //                        unkown test
                            
                            if ( runtimeStack.hasVar(term1.token))
                            {
                                if ( ! (runtimeStack.getVar(term1.token).value.compare("undefined") == 0) ) // check that var is defined
                                {
                                    if (term1.type.compare("integer") == 0)
                                    {
                                        int term1_int = stoi(term1.value);
                                        
                                        if (term2.lexeme.compare("integer") == 0)
                                        {
                                            bool result = term1_int >= stoi(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                            term1.type = "undefined";
                                            term1.value = "undefined";
                                        }
                                        else if (term2.lexeme.compare("float") == 0)
                                        {
                                            bool result = term1_int >= stod(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                            term1.type = "undefined";
                                            term1.value = "undefined";
                                        }
                                        else if (term2.lexeme.compare("var") == 0)
                                        {
                                            // check if var is in symbol table (else throw error
                                            
                                            // then check if var is double or int
                                            
                                            //                                    CHECK VAR in table
                                            //                                    check var defined
                                            
                                            // check in table
                                            if ( runtimeStack.hasVar(term2.token)) {
                                                
                                                if ( ! (runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                                {
                                                    if (term2.type.compare("integer") == 0)
                                                    {
                                                        bool result = term1_int >= stoi(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                        term1.type = "undefined";
                                                        term1.value = "undefined";
                                                    }
                                                    else if (term2.type.compare("float") == 0)
                                                    {
                                                        bool result = term1_int >= stod(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                        term1.type = "undefined";
                                                        term1.value = "undefined";
                                                    }
                                                    else if (term2.type.compare("string") == 0)
                                                    {
                                                        // throw error
                                                    }
                                                }
                                                else
                                                {
                                                    throw "runtime error. variable " + term2.token + " undefined.";
                                                }
                                                
                                            }
                                            else
                                            {
                                                // throw unkown var error
                                                throw "runtime error. unkown variable: " + term2.token;
                                                
                                            }
                                            
                                            
                                        }
                                    }                        // else if type double
                                    else if (term1.type.compare("float") == 0)
                                    {
                                        double term1_double = stoi(term1.token);
                                        
                                        if (term2.lexeme.compare("integer") == 0)
                                        {
                                            bool result = term1_double >= stoi(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                            term1.type = "undefined";
                                            term1.value = "undefined";
                                        }
                                        else if (term2.lexeme.compare("float") == 0)
                                        {
                                            bool result = term1_double >= stod(term2.token);
                                            term1.token = to_string(result);
                                            term1.lexeme = "bool";
                                            term1.type = "undefined";
                                            term1.value = "undefined";
                                        }
                                        else if (term2.lexeme.compare("var") == 0)
                                        {
                                            // check if var is double or int
                                            
                                            //                                    CHECK VAR in table
                                            //                                    check var defined
                                            
                                            // check in table
                                            if ( runtimeStack.hasVar(term2.token)) {
                                                
                                                if ( ! (runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                                {
                                                    if (term2.type.compare("integer") == 0)
                                                    {
                                                        bool result = term1_double >= stoi(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                        term1.type = "undefined";
                                                        term1.value = "undefined";
                                                    }
                                                    else if (term2.type.compare("float") == 0)
                                                    {
                                                        bool result = term1_double >= stod(term2.value);
                                                        term1.token = to_string(result);
                                                        term1.lexeme = "bool";
                                                        term1.type = "undefined";
                                                        term1.value = "undefined";
                                                    }
                                                    else if (term2.type.compare("string") == 0)
                                                    {
                                                        // throw error
                                                    }
                                                }
                                                else
                                                {
                                                    throw "runtime error. variable " + term2.token + " undefined.";
                                                }
                                                
                                            }
                                            else
                                            {
                                                // throw unkown var error
                                                throw "runtime error. unkown variable: " + term2.token;
                                                
                                            }
                                            
                                            
                                        }
                                    }
                                    else if (term1.type.compare("string") == 0) // string error?
                                    {
                                        
                                    }
                                }
                                else
                                {
                                    // throw error
                                    throw "runtime error. variable " + term1.token + " undefined.";
                                }
                            }
                            else
                            {
                                // throw unkown var error
                                throw "runtime error. unkown variable: " + term1.token;
                            }
                        }
                    }
                }
                else
                {
                    // error
                    throw "compiler error. Expected term token (integer, string, float, minus_op, var) and got a " + token + "\n";
                    //                return;
                }
            }
        }
        else
        {
            // error
            throw "compiler error. Expected term token (integer, string, float, minus_op, var) and got a " + token + "\n";
            //        return;
        }
        //(toPrint, "Exit rel_expr\n");
        
        return term1;
    } catch (string error) {
        throw error;
    }
}

// term
Val parseTerm(string & toPrint)
{
    try {
        //(toPrint, "Enter Term\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        Val result;
        
        if ( (category.compare("var") == 0) )
        {
            
            
            //currToken_index++;
            //token = tokenList[currToken_index][0];
            //category = tokenList[currToken_index][1];
            
            // <function_invoc>
            // <var>(
            if ( (tokenList[currToken_index+1][1].compare("p_left") == 0) )
            {
                result = parseFunction_invoc(toPrint);
            }
            // <object>
            // <var> = new
            else if ( (tokenList[currToken_index+1][1].compare("assign_op") == 0) )
            {
                //currToken_index++;
                //token = tokenList[currToken_index][0];
                //category = tokenList[currToken_index][1];
                
                if ( (tokenList[currToken_index+2][1].compare("new_key") == 0) ) {
                    result = parseObject(toPrint);
                }
                else
                {
                    // error
                    throw "compiler error. Expected new_key and got a " + token + "\n";
                    //                return;
                }
                
            }
            
            // call bool expr
            
            // call math expr
            
            // <math>
            // <var>
            // - <var>
            else
            {
                // error
                result = parseMath(toPrint);
                
            }
        }
        else if ((category.compare("integer") == 0) ||
                 (category.compare("single_quote") == 0) ||
                 (category.compare("float") == 0) ||
                 (category.compare("minus_op") == 0) ||
                 (category.compare("p_left") == 0) /* ||
                                                    (category.compare("var") == 0) */ ) // math pow check
        {
            result = parseMath(toPrint);
        }
        else
        {
            // error
            throw "compiler error. Expected <math>, <function_invoc>, or <object> and got a " + token + "\n";
            //        return;
        }
        
        //(toPrint, "Exit Term\n");
        
        return result;
    } catch (string error) {
        throw error;
    }
}

// function_invoc
Val parseFunction_invoc(string & toPrint)  //
{
    try{
        //(toPrint, "Enter Function_invoc\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        vector<Val> argList;
        vector<Val> params;
        
        string funName;
        //    string returnVarName;
        Val var;
        
        Val result;
        
        if (category.compare("var") == 0)
        {
            //        append(toPrint, "Token Match. Expected - and got a " + token + "\n");
            var = parseVar(toPrint);
            funName = var.token;
            
            
            
            if ((funName.compare("print") == 0) || (funName.compare("copy") == 0)) {
                
                currToken_index++; // skip (
                Val expr = parseExpr(toPrint);
                
                currToken_index++; // skip )
                token = tokenList[currToken_index][0];
                category = tokenList[currToken_index][1];
                
                if (token.compare(";") == 0) {
                    currToken_index++;
                }
                
                if (funName.compare("print") == 0)
                {
                    if (expr.lexeme.compare("var") == 0) {
                        cout << expr.value << endl;
                        return expr;
                    }
                    else
                    {
                        cout << expr.token << endl;
                        return expr;
                    }
                }
                else if (funName.compare("copy") == 0)
                {
                    if (expr.lexeme.compare("var") == 0) {
                        
                        var.hasRefVar = false;
                        return expr;
                    }
                    else
                    {
                        return expr;
                    }
                }
            }
        
            else if (runtimeStack.mode.compare("regular") == 0) {
                
                // CHECK funName is in function table
                if (runtimeStack.hasFun(var.token)  ) { // THIS NEEDS TO BE ADJUSTED FOR OBJECT MANIPULATION
                    
                    // rest of code gets done later in this function
                }
                else
                {
                    throw "runtime error. undefined function: " + funName;
                    
                }
                
            }
            else
            {
                token = tokenList[currToken_index][0];
                category = tokenList[currToken_index][1];
                
                // check for left perens
                if (category.compare("p_left") == 0)
                {
                    //(toPrint, "Token Match. Expected p_left and got a " + token + "\n");
                    
                    currToken_index++;
                    token = tokenList[currToken_index][0];
                    category = tokenList[currToken_index][1];
                    
                    
                    while (category.compare("p_right") != 0) {
                        
                        if (category.compare("comma_token") == 0) { // comma
                            
                            
                        }
                        else if (category.compare("var") == 0) // var
                        {
                            // chack var exists
                            if (runtimeStack.hasVar(token)) {
                                Val newVal = *new Val;
                                newVal.lexeme = "val";
                                newVal.token = token;
                                newVal.value = runtimeStack.getVar(token).value;
                                newVal.type = runtimeStack.getVar(token).type;
                                
                                args.push_back(newVal);
                            }
                            else
                            {
                                throw "runtime error. " + token + " undefined in call to " + var.token;
                            }
                        }
                        else if ((category.compare("int") == 0) || (category.compare("float") == 0) ) // literal
                        {
                            Val newVal = *new Val;
                            newVal.lexeme = category;
                            newVal.token = token;
                            
                            args.push_back(newVal);
                        }
                        else if (category.compare("single_quote") == 0)
                        {
                            
                            currToken_index++;
                            token = tokenList[currToken_index][0];
                            category = tokenList[currToken_index][1];
                            
                            Val newVal = *new Val;
                            newVal.lexeme = category;
                            newVal.token = token;
                            
                            args.push_back(newVal);
                            
                            // skip last single quote
                            currToken_index++;
                        }
                        
                        currToken_index++;
                        token = tokenList[currToken_index][0];
                        category = tokenList[currToken_index][1];
                        
                    }
                    
                    currToken_index++; // skip over p_right
                    
                    
                    // add paramaters to global paramater vector
                    
                    // iterate through tokens (can be vars, literals, or ,) until reach right peren
                    
                    // can be either function call or member variable access?
                    
                    
                }
                else
                {
                    throw "compiler error. Expected p_left and got a " + token + "\n";
                    return result;
                }
                
                return var; // return name of function
            }
            
            runtimeStack.push_on(); // CREATE NEW SCOPE ON RUNTIME STACK
            
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            
            // check for left perens
            if (category.compare("p_left") == 0)
            {
                //(toPrint, "Token Match. Expected p_left and got a " + token + "\n");
                
                // parse params
                currToken_index++;
                token = tokenList[currToken_index][0];
                category = tokenList[currToken_index][1];
                
                if (category.compare("p_right") == 0) // allow for empty paramater list
                {
                    //(toPrint, "Token Match. Expected possible p_right and got a " + token + "\n");
                    currToken_index++;
                }
                else
                {
                    parseParams(toPrint, argList);
                    
                    token = tokenList[currToken_index][0];
                    category = tokenList[currToken_index][1];
                    
                    // check for right perens
                    if (category.compare("p_right") == 0)
                    {
                        //(toPrint, "Token Match. Expected p_right and got a " + token + "\n");
                        currToken_index++;
                    }
                    else
                    {
                        throw "compiler error. Expected p_right and got a " + token + "\n";
                        return result;
                    }
                    
                    // LOAD / BIND ARGUMENTS TO PARAMS IN NEW SCOPE
                    
                    // load function params to var table
                    fun_Table_element temp;
                    
                    params = runtimeStack.getFun(funName).param_list;    // CHECK: params didnt get LOADED --> START HERE
                    //                params = temp.param_list
                    
                    if (params.size() != argList.size()) {
                        throw "runtime error. invalid number of function arguments for: " + funName;
                    }
                    
                    for (int i = 0; i < params.size(); i++) { // load variables from the function's param list
                        
                        // DECIDE HERE?, whether to do by reference or by value
                        
                     //   if (params[i].byReference)  //  by reference, need to assign the referenceName, and set hasRefVar to true
                     //   {
                            
                     //   }
                     //   else
                     //   {
                           // Val newVar = *new Val;
                           // newVar.token = params[i].token;
                           // newVar.lexeme = params[i].lexeme;
                            
                            runtimeStack.top().varTable.add(params[i]);
                      //  }

                        //                    if (mode_interactive)
                        //                    {
                        //                        runtimeStack.global().varTable.add(newVar);
                        //                    }
                        //                    else
                        //                    {
                        //                        runtimeStack.top().varTable.add(newVar);
                        //                    }
                    }
                    
                    // then, go through passed arguments and update corresponding vars in var table accordingly, update types, and values
                    for (int i = 0; i < argList.size(); i++) {
                        
                        // if lexeme var
                        if (argList[i].lexeme.compare("var") == 0) {
                            
                            // update releveant vars of var table of current scope according to the vars (of higher scopes) that they correspond to
                            
                            // HERE - by reference and by value, based on type
                            
                            if (runtimeStack.hasVar(argList[i].token)) {
                                
                                if (params[i].byReference) {
                                    
                                    // update the var passed in, to be pointing at corresponing funcation variable, and update bool
                                    
                                    // PASS BY VALUE STUFF
                                    Val tempVal = runtimeStack.getVar_aboveScopes(argList[i].token);
                                    tempVal.hasRefVar = true;
                                    tempVal.referenceName = params[i].token;
                                    runtimeStack.updateVar(tempVal.token, tempVal);
                                    
                                    // need to assign proper values and types etc. to variabls in scope of this function

                                    
    // NEED TO: keep list of vars that have been updated in such a way, and put them back to the way they were when leaving the scope of the function call
                                    
                                    // so if a var is passed in, then all changes made to that var need to be reflected outside of the function
                                    
                                    // so the var in the arglist
                                    
                                    
                                   // params[i].referenceName = argList[i].token;
                                   // params[i].hasRefVar = true;
                                
                                  //  runtimeStack.top().varTable.update(params[i].token, runtimeStack.getVar_aboveScopes(argList[i].token)); // NEED TO UPDATE THIS TOO
                                    
                                }
                                
                                Val tempVal2 = params[i];
                                tempVal2.type = runtimeStack.getVar_aboveScopes(argList[i].token).type;
                                tempVal2.value = runtimeStack.getVar_aboveScopes(argList[i].token).value;
                                tempVal2.byReference = runtimeStack.getVar_aboveScopes(argList[i].token).byReference;
                                //tempVal2.hasRefVar = runtimeStack.getVar_aboveScopes(argList[i].token).hasRefVar;
                                //tempVal2.referenceName = runtimeStack.getVar_aboveScopes(argList[i].token).referenceName;
                                runtimeStack.updateVar(tempVal2.token, tempVal2);
                                
                                
                                //runtimeStack.top().varTable.get(params[i].token).type = runtimeStack.getVar_aboveScopes(argList[i].token).type; // NEED TO UPDATE THIS TOO
                                // runtimeStack.top().varTable.get(params[i].token).value = runtimeStack.getVar_aboveScopes(argList[i].token).value;
                                
                            }
                            else
                            {
                                throw "runtime error. unkown variable " + argList[i].token + " referenced in invocation of " + funName;
                            }
                        }
                        else //convert between var and non-var
                        {
                            // runtimeStack.top().varTable.get(params[i].token).type = argList[i].lexeme;
                            // runtimeStack.top().varTable.get(params[i].token).value = argList[i].token;
                            
                            Val tempVal2 = params[i];
                            tempVal2.type = argList[i].lexeme;
                            tempVal2.value = argList[i].token;
                            runtimeStack.updateVar(tempVal2.token, tempVal2);
                            
                        }
                        
                    }
                    
                    
                }
                
            }
            else
            {
                throw "compiler error. Expected p_left and got a " + token + "\n";
                return result;
            }
            
            
            // execute code found in function table
            // save tokenList
            vector< vector < string > > temp_tokenList = tokenList;
            // save currToken_index
            int temp_currToken_index = currToken_index;
            // update tokenList
            tokenList = runtimeStack.getFun(funName).code;
            // set currToken_index to 0
            currToken_index = 0;
            // call parse stmt list
            
            result = parseStmt_list(toPrint);
            // restore tokenList
            tokenList = temp_tokenList;
            // restore currToken_index
            currToken_index = temp_currToken_index;
            
            // find return value in local scope
            string returnVarName = runtimeStack.getFun(funName).return_var;
            
            
            result = runtimeStack.getVar(returnVarName);
            
            // turn result into a literal
            
            
            // pop scope
            runtimeStack.pop();
        }
        else
        {
            throw "compiler error. Expected var and got a " + token + "\n";
            return result;
        }
        //(toPrint, "Exit Function_invoc\n");
        
        
        
        Val result_litral;
        result_litral.token = result.value;
        result_litral.lexeme = result.type;
        
        return result_litral;
        
        
        
    } catch (string error) {
        throw error;
    }
}

// math_value
Val parseMath(string & toPrint)
{
    try {
        //(toPrint, "Enter Math\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        Val term1;
        Val term2;
        
        if ((category.compare("integer") == 0) ||
            (category.compare("single_quote") == 0) ||
            (category.compare("float") == 0) ||
            (category.compare("minus_op") == 0) ||
            (category.compare("var") == 0) ||
            (category.compare("p_left") == 0) ) // math pow check
        {
            // append(toPrint, "Token Match. Expected math_pow (integer, string, float, minus_up, or var) and got a " + token + "\n");
            term1 = parseMath_value(toPrint);
            
            // look ahead
            //currToken_index++;
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            if ((category.compare("plus_op") == 0) || (category.compare("minus_op") == 0))
            {
                //(toPrint, "Token Match. Expected plus_op or minus_op and got a " + token + "\n");
                currToken_index++;
                term2 = parseMath(toPrint);
                
                if (category.compare("plus_op") == 0)
                {
                    if (term1.lexeme.compare("integer") == 0)
                    {
                        int term1_int = stoi(term1.token);
                        
                        if (term2.lexeme.compare("integer") == 0)
                        {
                            int result = term1_int + stoi(term2.token);
                            term1.token = to_string(result);
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "integer";
                        }
                        else if (term2.lexeme.compare("float") == 0)
                        {
                            double result = term1_int + stod(term2.token);
                            term1.token = to_string(result);
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "float";
                        }
                        else if (term2.lexeme.compare("string") == 0)
                        {
                            string result = term1.token + term2.token;
                            term1.token = result;
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "string";
                            
                        }
                        else if (term2.lexeme.compare("var") == 0)
                        {
                            // check if var is double or int
                            
                            // check if var in symbol table
                            
                            //                        CHECK VAR in table
                            //                        check var defined
                            
                            // check in table
                            if ( runtimeStack.hasVar(term2.token)) {
                                
                                if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0)) {
                                    
                                    if (term2.type.compare("integer") == 0)
                                    {
                                        int result = term1_int + stoi(term2.value);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "integer";
                                    }
                                    else if (term2.type.compare("float") == 0)
                                    {
                                        double result = term1_int + stod(term2.value);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.type.compare("string") == 0)
                                    {
                                        string result = term1.token + term2.value;
                                        term1.token = result;
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "string";
                                    }
                                }
                                else
                                {
                                    throw "runtime error. variable " + term2.token + " undefined.";
                                }
                                
                            }
                            else
                            {
                                // throw unkown var error
                                throw "runtime error. unkown variable: " + term2.token;
                                
                            }
                            
                            
                        }
                    }
                    else if (term1.lexeme.compare("float") == 0)
                    {
                        double term1_double = stoi(term1.token);
                        
                        if (term2.lexeme.compare("integer") == 0)
                        {
                            double result = term1_double + stoi(term2.token);
                            term1.token = to_string(result);
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "float";
                        }
                        else if (term2.lexeme.compare("float") == 0)
                        {
                            double result = term1_double + stod(term2.token);
                            term1.token = to_string(result);
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "float";
                        }
                        else if (term2.lexeme.compare("string") == 0)
                        {
                            string result = term1.token + term2.token;
                            term1.token = result;
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "string";
                        }
                        else if (term2.lexeme.compare("var") == 0)
                        {
                            // check if var is double or int
                            
                            // check if var in symbol table
                            
                            //                        CHECK VAR in table
                            //                        check var defined
                            
                            // check in table
                            if ( runtimeStack.hasVar(term2.token)) {
                                
                                // check defined
                                if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                {
                                    if (term2.type.compare("integer") == 0)
                                    {
                                        double result = term1_double + stoi(term2.value);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.type.compare("float") == 0)
                                    {
                                        double result = term1_double + stod(term2.value);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.type.compare("string") == 0)
                                    {
                                        string result = term1.token + term2.value;
                                        term1.token = result;
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "string";
                                    }
                                }
                                else
                                {
                                    throw "runtime error. variable " + term2.token + " undefined.";
                                }
                                
                            }
                            else
                            {
                                // throw unkown var error
                                throw "runtime error. unkown variable: " + term2.token;
                                
                            }
                            
                            
                        }
                    }
                    else if (term1.lexeme.compare("string") == 0)
                    {
                        string result = term1.token + term2.token;
                        term1.token = result;
                        term1.value = "undefined";
                        term1.type = "undefined";
                        term1.lexeme = "string";
                        
                        
                        string term1_string = term1.token;
                        
                        if (term2.lexeme.compare("integer") == 0)
                        {
                            string result = term1_string + term2.token;
                            term1.token = result;
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "string";
                        }
                        else if (term2.lexeme.compare("float") == 0)
                        {
                            string result = term1_string + term2.token;
                            term1.token = result;
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "string";
                        }
                        else if (term2.lexeme.compare("string") == 0)
                        {
                            string result = term1.token + term2.token;
                            term1.token = result;
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "string";
                        }
                        else if (term2.lexeme.compare("var") == 0)
                        {
                            
                            // check in table
                            if ( runtimeStack.hasVar(term2.token)) {
                                
                                // check defined
                                if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                {
                                    if (term2.type.compare("integer") == 0)
                                    {
                                        string result = term1_string + term2.value;
                                        term1.token = result;
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "string";
                                    }
                                    else if (term2.type.compare("float") == 0)
                                    {
                                        string result = term1_string + term2.value;
                                        term1.token = result;
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "string";
                                    }
                                    else if (term2.type.compare("string") == 0)
                                    {
                                        string result = term1_string + term2.value;
                                        term1.token = result;
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "string";
                                    }
                                }
                                else
                                {
                                    throw "runtime error. variable " + term2.token + " undefined.";
                                }
                                
                            }
                            else
                            {
                                // throw unkown var error
                                throw "runtime error. unkown variable: " + term2.token;
                                
                            }
                            
                            
                        }
                        
                    }
                    else if (term1.lexeme.compare("var") == 0)
                    {
                        // check if symbol table (else throw undefined error)
                        
                        //                    unkown test
                        
                        if ( runtimeStack.hasVar(term1.token))
                        {
                            if ( !(runtimeStack.getVar(term1.token).value.compare("undefined") == 0) )
                            {
                                // if type int
                                if (term1.type.compare("integer") == 0)
                                {
                                    int term1_int = stoi(term1.value);
                                    
                                    if (term2.lexeme.compare("integer") == 0)
                                    {
                                        int result = term1_int + stoi(term2.token);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "integer";
                                    }
                                    else if (term2.lexeme.compare("float") == 0)
                                    {
                                        double result = term1_int + stod(term2.token);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.lexeme.compare("string") == 0)
                                    {
                                        string result = term1.value + term2.token;
                                        term1.token = result;
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "string";
                                    }
                                    else if (term2.lexeme.compare("var") == 0)
                                    {
                                        // check if var is in symbol table (else throw error
                                        
                                        // then check if var is double or int
                                        
                                        //                                CHECK VAR in table
                                        //                                check var defined
                                        
                                        // check in table
                                        if ( runtimeStack.hasVar(term2.token)) {
                                            
                                            // check defined
                                            if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                            {
                                                if (term2.type.compare("integer") == 0)
                                                {
                                                    int result = term1_int + stoi(term2.value);
                                                    term1.token = to_string(result);
                                                    term1.value = "undefined";
                                                    term1.type = "undefined";
                                                    term1.lexeme = "integer";
                                                }
                                                else if (term2.type.compare("float") == 0)
                                                {
                                                    double result = term1_int + stod(term2.value);
                                                    term1.token = to_string(result);
                                                    term1.value = "undefined";
                                                    term1.type = "undefined";
                                                    term1.lexeme = "float";
                                                }
                                                else if (term2.type.compare("string") == 0)
                                                {
                                                    string result = term1.value + term2.value;
                                                    term1.token = result;
                                                    term1.value = "undefined";
                                                    term1.type = "undefined";
                                                    term1.lexeme = "string";
                                                }
                                            }
                                            else
                                            {
                                                throw "runtime error. variable " + term2.token + " undefined.";
                                            }
                                            
                                        }
                                        else
                                        {
                                            // throw unkown var error
                                            throw "runtime error. unkown variable: " + term2.token;
                                            
                                        }
                                        
                                        
                                    }
                                }                        // else if type double
                                else if (term1.type.compare("float") == 0)
                                {
                                    double term1_double = stoi(term1.token);
                                    
                                    if (term2.lexeme.compare("integer") == 0)
                                    {
                                        double result = term1_double + stoi(term2.token);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.lexeme.compare("float") == 0)
                                    {
                                        double result = term1_double + stod(term2.token);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.lexeme.compare("string") == 0)
                                    {
                                        string result = term1.value + term2.token;
                                        term1.token = result;
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "string";
                                    }
                                    else if (term2.lexeme.compare("var") == 0)
                                    {
                                        // check if var is double or int
                                        
                                        //                                CHECK VAR in table
                                        //                                check var defined
                                        
                                        // check in table
                                        if ( runtimeStack.hasVar(term2.token)) {
                                            
                                            // check defined
                                            if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                            {
                                                if (term2.type.compare("integer") == 0)
                                                {
                                                    double result = term1_double + stoi(term2.value);
                                                    term1.token = to_string(result);
                                                    term1.value = "undefined";
                                                    term1.type = "undefined";
                                                    term1.lexeme = "integer";
                                                }
                                                else if (term2.type.compare("float") == 0)
                                                {
                                                    double result = term1_double + stod(term2.value);
                                                    term1.token = to_string(result);
                                                    term1.value = "undefined";
                                                    term1.type = "undefined";
                                                    term1.lexeme = "float";
                                                }
                                                else if (term2.type.compare("string") == 0)
                                                {
                                                    string result = term1.value + term2.token;
                                                    term1.token = result;
                                                    term1.value = "undefined";
                                                    term1.type = "undefined";
                                                    term1.lexeme = "string";
                                                }
                                            }
                                            else
                                            {
                                                throw "runtime error. variable " + term2.token + " undefined.";
                                            }
                                            
                                        }
                                        else
                                        {
                                            // throw unkown var error
                                            throw "runtime error. unkown variable: " + term2.token;
                                            
                                        }
                                        
                                        
                                    }
                                }
                                // else if type string
                                else if (term1.type.compare("string") == 0)
                                {
                                    // throw error
                                }
                            }
                            else
                            {
                                throw "runtime error. variable " + term2.token + " undefined.";
                            }
                            
                        }
                        else
                        {
                            // throw unkown var error
                            throw "runtime error. unkown variable: " + term1.token;
                        }
                    }
                }
                else if (category.compare("minus_op") == 0)
                {
                    if (term1.lexeme.compare("integer") == 0)
                    {
                        int term1_int = stoi(term1.token);
                        
                        if (term2.lexeme.compare("integer") == 0)
                        {
                            int result = term1_int - stoi(term2.token);
                            term1.token = to_string(result);
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "integer";
                        }
                        else if (term2.lexeme.compare("float") == 0)
                        {
                            double result = term1_int - stod(term2.token);
                            term1.token = to_string(result);
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "float";
                        }
                        else if (term2.lexeme.compare("var") == 0)
                        {
                            // check if var is double or int
                            
                            //                        CHECK VAR in table
                            //                        check var defined
                            
                            // check in table
                            if ( runtimeStack.hasVar(term2.token)) {
                                
                                // check defined
                                if (! (runtimeStack.getVar(term2.token).value.compare("undefined") == 0)) {
                                    // check in table
                                    if (term2.type.compare("integer") == 0)
                                    {
                                        int result = term1_int - stoi(term2.value);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "integer";
                                    }
                                    else if (term2.type.compare("float") == 0)
                                    {
                                        double result = term1_int - stod(term2.value);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.type.compare("string") == 0)
                                    {
                                        // throw error
                                    }
                                    
                                }
                                else
                                {
                                    // throw undefined error
                                    throw "runtime error. variable " + term2.token + " undefined.";
                                }
                                
                            }
                            else
                            {
                                // throw unkown var error
                                throw "runtime error. unkown variable: " + term2.token;
                                
                            }
                            
                        }
                    }
                    else if (term1.lexeme.compare("float") == 0)
                    {
                        double term1_double = stoi(term1.token);
                        
                        if (term2.lexeme.compare("integer") == 0)
                        {
                            double result = term1_double - stoi(term2.token);
                            term1.token = to_string(result);
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "float";
                        }
                        else if (term2.lexeme.compare("float") == 0)
                        {
                            double result = term1_double - stod(term2.token);
                            term1.token = to_string(result);
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "float";
                        }
                        else if (term2.lexeme.compare("var") == 0)
                        {
                            // check if var is double or int
                            
                            //                        CHECK VAR in table
                            //                        check var defined
                            
                            
                            
                            // check in table
                            if ( runtimeStack.hasVar(term2.token)) {
                                
                                // check defined
                                if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                {
                                    if (term2.type.compare("integer") == 0)
                                    {
                                        double result = term1_double - stoi(term2.value);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.type.compare("float") == 0)
                                    {
                                        double result = term1_double - stod(term2.value);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.type.compare("string") == 0)
                                    {
                                        // throw error
                                    }
                                }
                                else
                                {
                                    throw "runtime error. variable " + term2.token + " undefined.";
                                }
                                
                            }
                            else
                            {
                                // throw unkown var error
                                throw "runtime error. unkown variable: " + term2.token;
                                
                            }
                            
                            
                        }
                    }
                    else if (term1.lexeme.compare("var") == 0)
                    {
                        // check if symbol table (else throw undefined error)
                        
                        //                    unkown test
                        
                        if ( runtimeStack.hasVar(term1.token))
                        {
                            if ( !(runtimeStack.getVar(term1.token).value.compare("undefined") == 0) )
                            {
                                // if type int
                                if (term1.type.compare("integer") == 0)
                                {
                                    int term1_int = stoi(term1.value);
                                    
                                    if (term2.lexeme.compare("integer") == 0)
                                    {
                                        int result = term1_int - stoi(term2.token);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "integer";
                                    }
                                    else if (term2.lexeme.compare("float") == 0)
                                    {
                                        double result = term1_int - stod(term2.token);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                        
                                    }
                                    else if (term2.lexeme.compare("var") == 0)
                                    {
                                        // check if var is double or int
                                        
                                        //                                CHECK VAR in table
                                        //                                check var defined
                                        
                                        // check in table
                                        if ( runtimeStack.hasVar(term2.token)) {
                                            
                                            // check defined
                                            if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                            {
                                                if (term2.type.compare("integer") == 0)
                                                {
                                                    int result = term1_int - stoi(term2.value);
                                                    term1.token = to_string(result);
                                                    
                                                    
                                                    
                                                }
                                                else if (term2.type.compare("float") == 0)
                                                {
                                                    double result = term1_int - stod(term2.value);
                                                    term1.token = to_string(result);
                                                }
                                                else if (term2.type.compare("string") == 0)
                                                {
                                                    // throw error
                                                }
                                            }
                                            else
                                            {
                                                throw "runtime error. variable " + term2.token + " undefined.";
                                            }
                                            
                                        }
                                        else
                                        {
                                            // throw unkown var error
                                            throw "runtime error. unkown variable: " + term2.token;
                                            
                                        }
                                        
                                        
                                    }
                                }
                                // else if type double
                                else if (term1.type.compare("float") == 0)
                                {
                                    double term1_double = stoi(term1.token);
                                    
                                    if (term2.lexeme.compare("integer") == 0)
                                    {
                                        double result = term1_double - stoi(term2.token);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.lexeme.compare("float") == 0)
                                    {
                                        double result = term1_double - stod(term2.token);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.lexeme.compare("var") == 0)
                                    {
                                        // check if var is double or int
                                        
                                        //                                CHECK VAR in table
                                        //                                check var defined
                                        
                                        // check in table
                                        if ( runtimeStack.hasVar(term2.token)) {
                                            
                                            // check defined
                                            if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                            {
                                                if (term2.type.compare("integer") == 0)
                                                {
                                                    double result = term1_double - stoi(term2.value);
                                                    term1.token = to_string(result);
                                                    term1.value = "undefined";
                                                    term1.type = "undefined";
                                                    term1.lexeme = "float";
                                                }
                                                else if (term2.type.compare("float") == 0)
                                                {
                                                    double result = term1_double - stod(term2.value);
                                                    term1.token = to_string(result);
                                                    term1.value = "undefined";
                                                    term1.type = "undefined";
                                                    term1.lexeme = "float";
                                                }
                                                else if (term2.type.compare("string") == 0)
                                                {
                                                    // throw error
                                                }
                                            }
                                            else
                                            {
                                                throw "runtime error. variable " + term2.token + " undefined.";
                                            }
                                            
                                        }
                                        else
                                        {
                                            // throw unkown var error
                                            throw "runtime error. unkown variable: " + term2.token;
                                            
                                        }
                                        
                                        
                                    }
                                }
                                // else if type string
                                else if (term1.type.compare("string") == 0)
                                {
                                    // throw error
                                }
                            }
                            else
                            {
                                throw "runtime error. variable " + term2.token + " undefined.";
                            }
                        }
                        else
                        {
                            // throw unkown var error
                            throw "runtime error. unkown variable: " + term1.token;
                        }
                    }
                }
                
            }
        }
        else
        {
            throw "compiler error. Expected math_pow (integer, string, float, minus_up, or var) and got a " + token + "\n";
            //        return;
        }
        //(toPrint, "Exit Math\n");
        
        return term1;
        
    } catch (string error) {
        throw error;
    }
    
}

// math_value
Val parseMath_value(string & toPrint)
{
    try {
        
        
        //(toPrint, "Enter Math_value\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        Val term1;
        Val term2;
        
        if ((category.compare("integer") == 0) ||
            (category.compare("single_quote") == 0) ||
            (category.compare("float") == 0) ||
            (category.compare("minus_op") == 0) ||
            (category.compare("var") == 0) ||
            (category.compare("p_left") == 0) ) // math pow check
        {
            //append(toPrint, "Token Match. Expected math_pow (integer, string, float, minus_up, or var) and got a " + token + "\n");
            term1 = parseMath_term(toPrint);
            
            // look ahead
            //currToken_index++;
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            if ((category.compare("mult_op") == 0) || (category.compare("div_op") == 0))
            {
                //(toPrint, "Token Match. Expected mult_op or div_op and got a " + token + "\n");
                currToken_index++;
                
                term2 = parseMath(toPrint);
                
                if (category.compare("mult_op") == 0)
                {
                    if (term1.lexeme.compare("integer") == 0)
                    {
                        
                        int term1_int = stoi(term1.token);
                        
                        if (term2.lexeme.compare("integer") == 0)
                        {
                            int result = term1_int * stoi(term2.token);
                            term1.token = to_string(result);
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "integer";
                        }
                        else if (term2.lexeme.compare("float") == 0)
                        {
                            double result = term1_int * stod(term2.token);
                            term1.token = to_string(result);
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "float";
                        }
                        else if (term2.lexeme.compare("var") == 0)
                        {
                            // check if var is double or int
                            
                            //                        CHECK VAR in table
                            //                        check var defined
                            
                            // check in table
                            if ( runtimeStack.hasVar(term2.token)) {
                                
                                // check defined
                                if (! (runtimeStack.getVar(term2.token).value.compare("undefined") == 0)) {
                                    // check in table
                                    
                                    if (term2.type.compare("integer") == 0)
                                    {
                                        int result = term1_int * stoi(term2.value);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "integer";
                                    }
                                    else if (term2.type.compare("float") == 0)
                                    {
                                        double result = term1_int * stod(term2.value);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.type.compare("string") == 0)
                                    {
                                        // throw error
                                    }
                                }
                                else
                                {
                                    // throw undefined error
                                    throw "runtime error. variable " + term2.token + " undefined.";
                                }
                                
                            }
                            else
                            {
                                // throw unkown var error
                                throw "runtime error. unkown variable: " + term2.token;
                                
                            }
                            
                            
                        }
                    }
                    else if (term1.lexeme.compare("float") == 0)
                    {
                        double term1_double = stoi(term1.token);
                        
                        if (term2.lexeme.compare("integer") == 0)
                        {
                            double result = term1_double * stoi(term2.token);
                            term1.token = to_string(result);
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "integer";
                        }
                        else if (term2.lexeme.compare("float") == 0)
                        {
                            double result = term1_double * stod(term2.token);
                            term1.token = to_string(result);
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "float";
                        }
                        else if (term2.lexeme.compare("var") == 0)
                        {
                            // check if var is double or int
                            
                            //                        CHECK VAR in table
                            //                        check var defined
                            
                            // check in table
                            if ( runtimeStack.hasVar(term2.token)) {
                                
                                // check defined
                                if (! (runtimeStack.getVar(term2.token).value.compare("undefined") == 0)) {
                                    // check in table
                                    if (term2.type.compare("integer") == 0)
                                    {
                                        double result = term1_double * stoi(term2.value);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.type.compare("float") == 0)
                                    {
                                        double result = term1_double * stod(term2.value);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.type.compare("string") == 0)
                                    {
                                        // throw error
                                    }
                                    
                                }
                                else
                                {
                                    // throw undefined error
                                    throw "runtime error. variable " + term2.token + " undefined.";
                                }
                                
                            }
                            else
                            {
                                // throw unkown var error
                                throw "runtime error. unkown variable: " + term2.token;
                            }
                        }
                    }
                    else if (term1.lexeme.compare("var") == 0)
                    {
                        // check if symbol table (else throw undefined error)
                        
                        //                    unkown test
                        
                        if ( runtimeStack.hasVar(term1.token))
                        {
                            if ( !(runtimeStack.getVar(term1.token).value.compare("undefined") == 0) )
                            {
                                // if type int
                                if (term1.type.compare("integer") == 0)
                                {
                                    int term1_int = stoi(term1.value);
                                    
                                    if (term2.lexeme.compare("integer") == 0)
                                    {
                                        int result = term1_int * stoi(term2.token);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "integer";
                                    }
                                    else if (term2.lexeme.compare("float") == 0)
                                    {
                                        double result = term1_int * stod(term2.token);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.lexeme.compare("var") == 0)
                                    {
                                        // check if var is in symbol table (else throw error
                                        
                                        // then check if var is double or int
                                        //
                                        //                                CHECK VAR in table
                                        //                                check var defined
                                        
                                        // check in table
                                        if ( runtimeStack.hasVar(term2.token)) {
                                            
                                            // check defined
                                            if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                            {
                                                if (term2.type.compare("integer") == 0)
                                                {
                                                    int result = term1_int * stoi(term2.value);
                                                    term1.token = to_string(result);
                                                    term1.value = "undefined";
                                                    term1.type = "undefined";
                                                    term1.lexeme = "integer";
                                                }
                                                else if (term2.type.compare("float") == 0)
                                                {
                                                    double result = term1_int * stod(term2.value);
                                                    term1.token = to_string(result);
                                                    term1.value = "undefined";
                                                    term1.type = "undefined";
                                                    term1.lexeme = "float";
                                                }
                                                else if (term2.type.compare("string") == 0)
                                                {
                                                    // throw error
                                                }
                                            }
                                            else
                                            {
                                                throw "runtime error. variable " + term2.token + " undefined.";
                                            }
                                            
                                        }
                                        else
                                        {
                                            // throw unkown var error
                                            throw "runtime error. unkown variable: " + term2.token;
                                            
                                        }
                                    }
                                }                        // else if type double
                                else if (term1.type.compare("float") == 0)
                                {
                                    double term1_double = stoi(term1.token);
                                    
                                    if (term2.lexeme.compare("integer") == 0)
                                    {
                                        double result = term1_double * stoi(term2.token);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.lexeme.compare("float") == 0)
                                    {
                                        double result = term1_double * stod(term2.token);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.lexeme.compare("var") == 0)
                                    {
                                        // check if var is double or int
                                        
                                        //                                CHECK VAR in table
                                        //                                check var defined
                                        
                                        // check in table
                                        if ( runtimeStack.hasVar(term2.token)) {
                                            // check defined
                                            if (! (runtimeStack.getVar(term2.token).value.compare("undefined") == 0)) {
                                                // check in table
                                                
                                                if (term2.type.compare("integer") == 0)
                                                {
                                                    double result = term1_double * stoi(term2.value);
                                                    term1.token = to_string(result);
                                                    term1.value = "undefined";
                                                    term1.type = "undefined";
                                                    term1.lexeme = "float";
                                                }
                                                else if (term2.type.compare("float") == 0)
                                                {
                                                    double result = term1_double * stod(term2.value);
                                                    term1.token = to_string(result);
                                                    term1.value = "undefined";
                                                    term1.type = "undefined";
                                                    term1.lexeme = "float";
                                                }
                                                else if (term2.type.compare("string") == 0)
                                                {
                                                    // throw error
                                                }
                                            }
                                            else
                                            {
                                                // throw undefined error
                                                throw "runtime error. variable " + term2.token + " undefined.";
                                            }
                                            
                                        }
                                        else
                                        {
                                            // throw unkown var error
                                            throw "runtime error. unkown variable: " + term2.token;
                                            
                                        }
                                    }
                                }
                                // else if type string
                                else if (term1.type.compare("string") == 0)
                                {
                                    // throw error
                                }
                            }
                            else
                            {
                                throw "runtime error. variable " + term1.token + " undefined.";
                            }
                        }
                        else
                        {
                            // throw unkown var error
                            throw "runtime error. unkown variable: " + term1.token;
                        }
                    }
                }
                else if (category.compare("div_op") == 0)
                {
                    if (term1.lexeme.compare("integer") == 0)
                    {
                        int term1_int = stoi(term1.token);
                        
                        if (term2.lexeme.compare("integer") == 0)
                        {
                            int result = term1_int / stoi(term2.token);
                            term1.token = to_string(result);
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "integer";
                        }
                        else if (term2.lexeme.compare("float") == 0)
                        {
                            double result = term1_int / stod(term2.token);
                            term1.token = to_string(result);
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "float";
                        }
                        else if (term2.lexeme.compare("var") == 0)
                        {
                            // check if var is double or int
                            
                            //                        CHECK VAR in table
                            //                        check var defined
                            
                            // check in table
                            if ( runtimeStack.hasVar(term2.token)) {
                                
                                // check defined
                                if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                {
                                    if (term2.type.compare("integer") == 0)
                                    {
                                        int result = term1_int / stoi(term2.value);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "integer";
                                    }
                                    else if (term2.type.compare("float") == 0)
                                    {
                                        double result = term1_int / stod(term2.value);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.type.compare("string") == 0)
                                    {
                                        // throw error
                                    }
                                }
                                else
                                {
                                    throw "runtime error. variable " + term2.token + " undefined.";
                                }
                                
                            }
                            else
                            {
                                // throw unkown var error
                                throw "runtime error. unkown variable: " + term2.token;
                            }
                        }
                    }
                    else if (term1.lexeme.compare("float") == 0)
                    {
                        double term1_double = stoi(term1.token);
                        
                        if (term2.lexeme.compare("integer") == 0)
                        {
                            double result = term1_double / stoi(term2.token);
                            term1.token = to_string(result);
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "float";
                        }
                        else if (term2.lexeme.compare("float") == 0)
                        {
                            double result = term1_double / stod(term2.token);
                            term1.token = to_string(result);
                            term1.value = "undefined";
                            term1.type = "undefined";
                            term1.lexeme = "float";
                        }
                        else if (term2.lexeme.compare("var") == 0)
                        {
                            // check if var is double or int
                            
                            //                        CHECK VAR in table
                            //                        check var defined
                            
                            // check in table
                            if ( runtimeStack.hasVar(term2.token)) {
                                
                                // check defined
                                if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                {
                                    if (term2.type.compare("integer") == 0)
                                    {
                                        double result = term1_double / stoi(term2.value);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.type.compare("float") == 0)
                                    {
                                        double result = term1_double / stod(term2.value);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.type.compare("string") == 0)
                                    {
                                        // throw error
                                    }
                                }
                                else
                                {
                                    throw "runtime error. variable " + term2.token + " undefined.";
                                }
                                
                            }
                            else
                            {
                                // throw unkown var error
                                throw "runtime error. unkown variable: " + term2.token;
                                
                            }
                        }
                    }
                    else if (term1.lexeme.compare("var") == 0)
                    {
                        // check if symbol table (else throw undefined error)
                        
                        //                    unkown test
                        
                        if ( runtimeStack.hasVar(term1.token))
                        {
                            if ( !(runtimeStack.getVar(term1.token).value.compare("undefined") == 0) )
                            {
                                // if type int
                                if (term1.type.compare("integer") == 0)
                                {
                                    int term1_int = stoi(term1.value);
                                    
                                    if (term2.lexeme.compare("integer") == 0)
                                    {
                                        int result = term1_int / stoi(term2.token);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "integer";
                                    }
                                    else if (term2.lexeme.compare("float") == 0)
                                    {
                                        double result = term1_int / stod(term2.token);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.lexeme.compare("var") == 0)
                                    {
                                        // check if var is double or int
                                        
                                        //                                CHECK VAR in table
                                        //                                check var defined
                                        
                                        // check in table
                                        if ( runtimeStack.hasVar(term2.token)) {
                                            
                                            // check defined
                                            if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                            {
                                                if (term2.type.compare("integer") == 0)
                                                {
                                                    int result = term1_int / stoi(term2.value);
                                                    term1.token = to_string(result);
                                                    term1.value = "undefined";
                                                    term1.type = "undefined";
                                                    term1.lexeme = "integer";
                                                }
                                                else if (term2.type.compare("float") == 0)
                                                {
                                                    double result = term1_int / stod(term2.value);
                                                    term1.token = to_string(result);
                                                    term1.value = "undefined";
                                                    term1.type = "undefined";
                                                    term1.lexeme = "float";
                                                }
                                                else if (term2.type.compare("string") == 0)
                                                {
                                                    // throw error
                                                }
                                            }
                                            else
                                            {
                                                throw "runtime error. variable " + term2.token + " undefined.";
                                            }
                                            
                                        }
                                        else
                                        {
                                            // throw unkown var error
                                            throw "runtime error. unkown variable: " + term2.token;
                                            
                                        }
                                    }
                                }
                                // else if type double
                                else if (term1.type.compare("float") == 0)
                                {
                                    double term1_double = stoi(term1.token);
                                    
                                    if (term2.lexeme.compare("integer") == 0)
                                    {
                                        double result = term1_double / stoi(term2.token);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.lexeme.compare("float") == 0)
                                    {
                                        double result = term1_double / stod(term2.token);
                                        term1.token = to_string(result);
                                        term1.value = "undefined";
                                        term1.type = "undefined";
                                        term1.lexeme = "float";
                                    }
                                    else if (term2.lexeme.compare("var") == 0)
                                    {
                                        // check if var is double or int
                                        
                                        
                                        
                                        //                                CHECK VAR in table
                                        //                                check var defined
                                        
                                        if ( runtimeStack.hasVar(term2.token)) {
                                            
                                            
                                            if ( !(runtimeStack.getVar(term2.token).value.compare("undefined") == 0) )
                                            {
                                                if (term2.type.compare("integer") == 0)
                                                {
                                                    double result = term1_double / stoi(term2.value);
                                                    term1.token = to_string(result);
                                                    term1.value = "undefined";
                                                    term1.type = "undefined";
                                                    term1.lexeme = "float";
                                                }
                                                else if (term2.type.compare("float") == 0)
                                                {
                                                    double result = term1_double / stod(term2.value);
                                                    term1.token = to_string(result);
                                                    term1.value = "undefined";
                                                    term1.type = "undefined";
                                                    term1.lexeme = "float";
                                                }
                                                else if (term2.type.compare("string") == 0)
                                                {
                                                    // throw error
                                                }
                                            }
                                            else
                                            {
                                                throw "runtime error. variable " + term2.token + " undefined.";
                                            }
                                            
                                        }
                                        else
                                        {
                                            // throw unkown var error
                                            throw "runtime error. unkown variable: " + term2.token;
                                            
                                        }
                                        
                                    }
                                }
                                // else if type string
                                else if (term1.type.compare("string") == 0)
                                {
                                    // throw error
                                }
                            }
                            else
                            {
                                throw "runtime error. variable " + term2.token + " undefined.";
                            }
                        }
                        else
                        {
                            // throw unkown var error
                            throw "runtime error. unkown variable: " + term1.token;
                        }
                        
                    }
                }
            }
        }
        else
        {
            throw "compiler error. Expected math_pow (integer, string, float, minus_op, or var) and got a " + token + "\n";
            //        return;
        }
        //(toPrint, "Exit Math_value\n");
        
        return term1;
        
    } catch (string error) {
        throw error;
    }
}

// math_term
Val parseMath_term(string & toPrint)
{
    try {
        //(toPrint, "Enter math_term\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        Val term1;
        Val term2;
        
        if ((category.compare("integer") == 0) ||
            (category.compare("single_quote") == 0) ||
            (category.compare("float") == 0) ||
            (category.compare("minus_op") == 0) ||
            (category.compare("var") == 0) ||
            (category.compare("p_left") == 0) ) // math_pow possiblities
        {
            //(toPrint, "Token Match. Expected math_pow (integer, string, float, minus_op, or var) and got a " + token + "\n");
            term1 = parseMath_pow(toPrint);
            
            // look ahead
            // currToken_index++;
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            
            if (category.compare("pow_op") == 0)
            {
                currToken_index++;
                //(toPrint, "Token Match. Expected possible pow_op and got a " + token + "\n");
                term2 = parseMath(toPrint);
                
                // check if same type, if not same, throw error;
                
                // if string, throw error
                
                // if var, look in symbol table
                
                //
                
                if (term1.lexeme.compare("integer") == 0)
                {
                    
                    int term1_int = stoi(term1.token);
                    
                    if (term2.lexeme.compare("integer") == 0)
                    {
                        int result = pow( term1_int , stoi(term2.token) );
                        term1.token = to_string(result);
                        term1.value = "undefined";
                        term1.type = "undefined";
                        term1.lexeme = "integer";
                    }
                    else if (term2.lexeme.compare("float") == 0)
                    {
                        double result = pow( term1_int , stod(term2.token) );
                        term1.token = to_string(result);
                        term1.value = "undefined";
                        term1.type = "undefined";
                        term1.lexeme = "float";
                    }
                    else if (term2.lexeme.compare("var") == 0)
                    {
                        // check if var is double or int
                        
                        //                    CHECK VAR in table
                        //                    check var defined
                        
                        if ( runtimeStack.hasVar(term2.token)) {
                            
                            
                            if (! (runtimeStack.getVar(term2.token).value.compare("undefined") == 0)) {
                                // check in table
                                if (term2.type.compare("integer") == 0)
                                {
                                    int result = pow( term1_int , stoi(term2.value) );
                                    term1.token = to_string(result);
                                    term1.value = "undefined";
                                    term1.type = "undefined";
                                    term1.lexeme = "integer";
                                }
                                else if (term2.type.compare("float") == 0)
                                {
                                    double result = pow( term1_int , stod(term2.value) );
                                    term1.token = to_string(result);
                                    term1.value = "undefined";
                                    term1.type = "undefined";
                                    term1.lexeme = "float";
                                }
                                else if (term2.type.compare("string") == 0)
                                {
                                    // throw error
                                }
                                
                            }
                            else
                            {
                                // throw undefined error
                                throw "runtime error. variable " + term2.token + " undefined.";
                            }
                            
                        }
                        else
                        {
                            // throw unkown var error
                            throw "runtime error. unkown variable: " + term2.token;
                            
                        }
                        
                    }
                }
                else if (term1.lexeme.compare("float") == 0)
                {
                    double term1_double = stoi(term1.token);
                    
                    if (term2.lexeme.compare("integer") == 0)
                    {
                        double result = pow( term1_double , stoi(term2.token) );
                        term1.token = to_string(result);
                        term1.value = "undefined";
                        term1.type = "undefined";
                        term1.lexeme = "float";
                    }
                    else if (term2.lexeme.compare("float") == 0)
                    {
                        double result = pow( term1_double , stod(term2.token) );
                        term1.token = to_string(result);
                        term1.value = "undefined";
                        term1.type = "undefined";
                        term1.lexeme = "float";
                    }
                    else if (term2.lexeme.compare("var") == 0)
                    {
                        // check if var is double or int
                        
                        
                        if ( runtimeStack.hasVar(term2.token)) {
                            
                            
                            if (! (runtimeStack.getVar(term2.token).value.compare("undefined") == 0)) {
                                // check in table
                                
                                if (term2.type.compare("integer") == 0)
                                {
                                    double result = pow( term1_double , stoi(term2.value) );
                                    term1.token = to_string(result);
                                    term1.value = "undefined";
                                    term1.type = "undefined";
                                    term1.lexeme = "float";
                                }
                                else if (term2.type.compare("float") == 0)
                                {
                                    double result = pow( term1_double , stod(term2.value) );
                                    term1.token = to_string(result);
                                    term1.value = "undefined";
                                    term1.type = "undefined";
                                    term1.lexeme = "float";
                                }
                                else if (term2.type.compare("string") == 0)
                                {
                                    // throw error
                                }
                                
                            }
                            else
                            {
                                // throw undefined error
                                throw "runtime error. variable " + term2.token + " undefined.";
                            }
                            
                        }
                        else
                        {
                            // throw unkown var error
                            throw "runtime error. unkown variable: " + term2.token;
                            
                        }
                    }
                }
                else if (term1.lexeme.compare("var") == 0)
                {
                    // check if symbol table (else throw undefined error)
                    
                    //                unkown test
                    
                    if ( runtimeStack.hasVar(term1.token))
                    {
                        if ( !(runtimeStack.getVar(term1.token).value.compare("undefined") == 0) )
                        {
                            // if type int
                            if (term1.type.compare("integer") == 0)
                            {
                                int term1_int = stoi(term1.value);
                                
                                if (term2.lexeme.compare("integer") == 0)
                                {
                                    int result = pow( term1_int , stoi(term2.token) );
                                    term1.token = to_string(result);
                                    term1.value = "undefined";
                                    term1.type = "undefined";
                                    term1.lexeme = "integer";
                                }
                                else if (term2.lexeme.compare("float") == 0)
                                {
                                    double result = pow( term1_int , stod(term2.token) );
                                    term1.token = to_string(result);
                                    term1.value = "undefined";
                                    term1.type = "undefined";
                                    term1.lexeme = "float";
                                }
                                else if (term2.lexeme.compare("var") == 0)
                                {
                                    // check if var is double or int
                                    
                                    // check in table
                                    if ( runtimeStack.hasVar(term2.token)) {
                                        
                                        // check defined
                                        if (! (runtimeStack.getVar(term2.token).value.compare("undefined") == 0)) {
                                            // check in table
                                            
                                            if (term2.type.compare("integer") == 0)
                                            {
                                                int result = pow( term1_int , stoi(term2.value) );
                                                term1.token = to_string(result);
                                                term1.value = "undefined";
                                                term1.type = "undefined";
                                                term1.lexeme = "integer";
                                            }
                                            else if (term2.type.compare("float") == 0)
                                            {
                                                double result = pow( term1_int , stod(term2.value) );
                                                term1.token = to_string(result);
                                                term1.value = "undefined";
                                                term1.type = "undefined";
                                                term1.lexeme = "float";
                                            }
                                            else if (term2.type.compare("string") == 0)
                                            {
                                                // throw error
                                            }
                                            
                                        }
                                        else
                                        {
                                            // throw undefined error
                                            throw "runtime error. variable " + term2.token + " undefined.";
                                        }
                                        
                                    }
                                    else
                                    {
                                        // throw unkown var error
                                        throw "runtime error. unkown variable: " + term2.token;
                                        
                                    }
                                }
                            }
                            // else if type double
                            else if (term1.type.compare("float") == 0)
                            {
                                double term1_double = stoi(term1.token);
                                
                                if (term2.lexeme.compare("integer") == 0)
                                {
                                    double result = pow( term1_double , stoi(term2.token) );
                                    term1.token = to_string(result);
                                    term1.value = "undefined";
                                    term1.type = "undefined";
                                    term1.lexeme = "float";
                                }
                                else if (term2.lexeme.compare("float") == 0)
                                {
                                    double result = pow( term1_double , stod(term2.token) );
                                    term1.token = to_string(result);
                                    term1.value = "undefined";
                                    term1.type = "undefined";
                                    term1.lexeme = "float";
                                }
                                else if (term2.lexeme.compare("var") == 0)
                                {
                                    // check if var is double or int
                                    
                                    
                                    // check in table
                                    if ( runtimeStack.hasVar(term2.token)) {
                                        
                                        // check defined
                                        if (! (runtimeStack.getVar(term2.token).value.compare("undefined") == 0)) {
                                            // check in table
                                            
                                            if (term2.type.compare("integer") == 0)
                                            {
                                                double result = pow( term1_double , stoi(term2.value) );
                                                term1.token = to_string(result);
                                                term1.value = "undefined";
                                                term1.type = "undefined";
                                                term1.lexeme = "float";
                                            }
                                            else if (term2.type.compare("float") == 0)
                                            {
                                                double result = pow( term1_double , stod(term2.value) );
                                                term1.token = to_string(result);
                                            }
                                            else if (term2.type.compare("string") == 0)
                                            {
                                                // throw error
                                            }
                                        }
                                        else
                                        {
                                            // throw undefined error
                                            throw "runtime error. variable " + term2.token + " undefined.";
                                        }
                                        
                                    }
                                    else
                                    {
                                        // throw unkown var error
                                        throw "runtime error. unkown variable: " + term2.token;
                                        
                                    }
                                    
                                    
                                }
                            }
                            // else if type string
                            else if (term1.type.compare("string") == 0)
                            {
                                // throw error
                            }
                            
                        }
                        else
                        {
                            throw "runtime error. variable " + term1.token + " undefined.";
                        }
                    }
                    else
                    {
                        // throw unkown var error
                        throw "runtime error. unkown variable: " + term1.token;
                    }
                }
            }
        }
        else
        {
            throw "compiler error. Expected math_pow (integer, string, float, minus_op, or var) and got a " + token + "\n";
        }
        //(toPrint, "Exit math_term\n");
        
        return term1;
    } catch (string error) {
        throw error;
    }
}

// math_pow
Val parseMath_pow(string & toPrint)
{
    try {
        //(toPrint, "Enter math_pow\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        Val math_pow;
        
        if (category.compare("minus_op") == 0) // CHANGE THIS
        {
            // ( (category.compare("minus_op") == 0) && (tokenList[currToken_index + 1][1].compare("integer") == 0) ) ||
            // ( (category.compare("minus_op") == 0) && (tokenList[currToken_index + 1][1].compare("integer") == 0) ) )
            
            
            // LOOK AHEAD TO decide on integer, float, or var
            if (tokenList[currToken_index + 1][1].compare("var") == 0)
            {
                math_pow = parseVar(toPrint);
            }
            else if (tokenList[currToken_index + 1][1].compare("integer") == 0)
            {
                math_pow = parseInteger(toPrint);
            }
            else if (tokenList[currToken_index + 1][1].compare("float") == 0)
            {
                math_pow = parseFloat(toPrint);
            }
            else
            {
                throw "compiler error. Expected var, integer or float and got a " + token + "\n";  // START HERE
                //            return;
            }
            
            
        }
        else if (category.compare("p_left") == 0)
        {
            math_pow = parsePerens(toPrint);
        }
        else if (category.compare("var") == 0)
        {
            
            currToken_index++;
            token = tokenList[currToken_index][0]; // check ahead for either ( or .
            category = tokenList[currToken_index][1];
            currToken_index--; // put currToken back to var for both parseFunction_invoc or parseOb_manip
            if ((category.compare("dot") == 0))
            {
                math_pow = parseOb_manip(toPrint);
            }
            else if ((category.compare("p_left") == 0))
            {
                math_pow = parseFunction_invoc(toPrint);
            }
            else
            {
                math_pow = parseVar(toPrint);
            }
        }
        else if ((category.compare("integer") == 0) ||  // literal check
                 (category.compare("single_quote") == 0) ||
                 (category.compare("float") == 0) )
        {
            math_pow = parseLiteral(toPrint);
        }
        else
        {
            throw "compiler error. Expected neg, literal, function_invoc, or ob_manip and got a " + token + "\n";
            //        return;
        }
        //(toPrint, "Exit math_pow\n");
        
        return math_pow;
    } catch (string error) {
        throw error;
    }
}

// neg
/*
 void parseNeg(string & toPrint)
 {
 append(toPrint, "Enter neg\n");
 
 cout << "inside parseNeg: " << toPrint << endl;
 
 string token = tokenList[currToken_index][0];
 string category = tokenList[currToken_index][1];
 //currToken_index++;
 if (category.compare("minus_op") == 0)
 {
 append(toPrint, "Token Match. Expected var or minus_op and got a " + token + "\n");
 currToken_index++;
 
 parseVar(toPrint);
 }
 else if (category.compare("var") == 0)
 {
 parseVar(toPrint);
 }
 else
 {
 throw "compiler error. Expected var or minus_op and got a " + token + "\n");
 }
 append(toPrint, "Exit neg\n");
 }
 */

// var

// NEED TO: assign value and type from global symbol table
// update var value for negative values.

Val parseVar(string & toPrint)
{
    try {
        //(toPrint, "Enter var\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        currToken_index++;
        
        Val var;
        
        // update var function; -- to be placed after all returns from functions
        //  pass val by reference
        
        // if lexeme = "var", then
        // assign value, type from symbol table
        
        if (category.compare("minus_op") == 0)
        {
            //(toPrint, "Token Match. Expected possible minus_op and got a " + token + "\n");
            
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            
            if (category.compare("var") == 0)
            {
                //(toPrint, "Token Match. Expected var and got a " + token + "\n");
                currToken_index++;
                
                var.lexeme = "var";
                var.token = token;
                
                if ( runtimeStack.hasVar(var.token)) {
                    
                    if (! (runtimeStack.getVar(var.token).value.compare("undefined") == 0)) {
                        // check in table
                        
                        var.type = runtimeStack.getVar(var.token).type;
                        var.value = "-" + runtimeStack.getVar(var.token).value;
                    }
                    else
                    {
                        // throw undefined error
                        throw "runtime error. variable " + var.token + " undefined.";
                    }
                }
                else
                {
                    // throw unkown var error
                    throw "runtime error. unkown variable: " + var.token;
                }
                
                //            if ( !var_table_global.has(token) )
                //            {
                //                var_table_global.add(var);
                //            }
            }
            else
            {
                throw "compiler error. Expected var and got a " + token + "\n";
                //            return;
            }
        }
        else if (category.compare("var") == 0)
        {
            //(toPrint, "Token Match. Expected var and got a " + token + "\n");
            
            //        currToken_index++;
            
            var.lexeme = "var";
            var.token = token;
            
            if ( runtimeStack.hasVar(var.token)) {
                
                if (! (runtimeStack.getVar(var.token).value.compare("undefined") == 0)) {
                    // check in table
                    
                    var.type = runtimeStack.getVar(var.token).type;
                    var.value = runtimeStack.getVar(var.token).value;
                }
                
            }
            
            //        if ( !var_table_global.has(token) )
            //        {
            //            var_table_global.add(var);
            //        }
            
        }
        else
        {
            throw "compiler error. Expected var and got a " + token + "\n";
            //        return;
        }
        //(toPrint, "Exit var\n");
        
        return var;
    } catch (string error) {
        throw error;
    }
}

// literal
Val parseLiteral(string & toPrint)
{
    try {
        //(toPrint, "Enter literal\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        //currToken_index++;
        
        Val literal; // = * new Val();
        
        if (category.compare("integer") == 0)
        {
            literal = parseInteger(toPrint);
        }
        else if (category.compare("single_quote") == 0)
        {
            literal = parseString_token(toPrint);
        }
        else if (category.compare("float") == 0)
        {
            literal = parseFloat(toPrint);
        }
        else
        {
            throw "compiler error. Expected integer and got a " + token + "\n";
            //        return;
        }
        //(toPrint, "Exit literal\n");
        
        return literal;
    } catch (string error) {
        throw error;
    }
}

// <integer_sign> → {-}<integer>
/*
 void parseInteger_sign(string & toPrint)
 {
 append(toPrint, "Enter integer_sign\n");
 string token = tokenList[currToken_index][0];
 string category = tokenList[currToken_index][1];
 
 if (category.compare("minus_op") == 0)
 {
 currToken_index++;
 append(toPrint, "Token Match. Expected possible minus_op and got a " + token + "\n");
 }
 else
 {
 throw "compiler error. Expected integer and got a " + token + "\n");
 }
 append(toPrint, "Exit int\n");
 }
 */

Val parseInteger(string & toPrint)
{
    try {
        //(toPrint, "Enter int\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        currToken_index++;
        
        Val _integer = * new Val();
        
        if (category.compare("minus_op") == 0)
        {
            //(toPrint, "Token Match. Expected possible minus_op and got a " + token + "\n");
            
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            if (category.compare("integer") == 0)
            {
                //(toPrint, "Token Match. Expected integer and got a " + token + "\n");
                currToken_index++;
                _integer.lexeme = "integer";
                _integer.token = "-" + token;
                
            }
            else
            {
                throw "compiler error. Expected integer and got a " + token + "\n";
                //            return;
            }
            
        }
        else if (category.compare("integer") == 0)
        {
            //(toPrint, "Token Match. Expected integer and got a " + token + "\n");
            
            _integer.lexeme = "integer";
            _integer.token = token;
        }
        else
        {
            throw "compiler error. Expected integer or minus_op and got a " + token + "\n";
            //        return;
        }
        //(toPrint, "Exit int\n");
        
        return _integer;
    } catch (string error) {
        throw error;
    }
    
}

/*
 void parseFloat_sign(string & toPrint)
 {
 append(toPrint, "Enter float\n");
 string token = tokenList[currToken_index][0];
 string category = tokenList[currToken_index][1];
 currToken_index++;
 if (category.compare("float") == 0)
 {
 append(toPrint, "Token Match. Expected float and got a " + token + "\n");
 }
 else
 {
 throw "compiler error. Expected float and got a " + token + "\n");
 }
 append(toPrint, "Exit float\n");
 }
 */

Val parseFloat(string & toPrint)
{
    try {
        //(toPrint, "Enter float\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        currToken_index++;
        
        Val _float = * new Val();
        
        if (category.compare("minus_op") == 0)
        {
            //(toPrint, "Token Match. Expected possible minus_op and got a " + token + "\n");
            
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            if (category.compare("float") == 0)
            {
                //(toPrint, "Token Match. Expected float and got a " + token + "\n");
                currToken_index++;
                
                _float.lexeme = "float";
                _float.token = "-" + token;
                
            }
            else
            {
                // error
                throw "compiler error. Expected float and got a " + token + "\n";
                //            return;
            }
            
        }
        else if (category.compare("float") == 0)
        {
            //(toPrint, "Token Match. Expected float or minus_op and got a " + token + "\n");
            _float.lexeme = "float";
            _float.token = token;
        }
        else
        {
            throw "compiler error. Expected float or minus_op and got a " + token + "\n";
            //        return;
        }
        //(toPrint, "Exit float\n");
        
        return _float;
    } catch (string error) {
        throw error;
    }
}

Val parseString_token(string & toPrint)
{
    try {
        //(toPrint, "Enter string_token\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        Val _string = * new Val();
        
        if ((category.compare("single_quote") == 0) && (currToken_index < tokenList.size()))
        {
            //(toPrint, "Token Match. Expected single_quote and got a " + token + "\n");
            
            currToken_index++; // go to next
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            
            if ((category.compare("string") == 0) && (currToken_index < tokenList.size()))
            {
                //(toPrint, "Token Match. Expected string and got a " + token + "\n");
                _string.lexeme = "string";
                _string.token = token;
                
            }
            else
            {
                throw "compiler error. Expected string and got a " + token + "\n";
                //            return;
            }
            
            //        term(toPrint);
            
            // go to next
            currToken_index++;
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            
            if (category.compare("single_quote") == 0)
            {
                //(toPrint, "Token Match. Expected single_quote and got a " + token + "\n");
                currToken_index++;
            }
            else
            {
                throw "compiler error. Expected single_quote and got a " + token + "\n";
                //            return;
            }
            
            
        }
        else
        {
            //        Var(toPrint);
            currToken_index++;
            throw "compiler error. Expected single_quote and got a " + token + "\n";
        }
        //(toPrint, "Exit string_token\n");
        
        return _string;
    } catch (string error) {
        throw error;
    }
}

Val parsePerens(string & toPrint)
{
    try {
        //(toPrint, "Enter perens\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        
        Val term;
        
        if ( (category.compare("p_left") == 0) )
        {
            //(toPrint, "Token Match. Expected p_left and got a " + token + "\n");
            
            currToken_index++;
            term = parseTerm(toPrint); // assuming that parseTerm() will increment currToken_index
            
            
            
            token = tokenList[currToken_index][0];
            category = tokenList[currToken_index][1];
            
            if ( (category.compare("p_right") == 0) )
            {
                //(toPrint, "Token Match. Expected p_right and got a " + token + "\n");
                currToken_index++;
                
            }
            else
            {
                throw "compiler error. parsePerens. Expected p_right and got a " + token + "\n";
                //            return;
            }
        }
        else if ( (category.compare("var") == 0) )
        {
            Val var = parseVar(toPrint);
            return var;
        }
        else
        {
            throw "compiler error. Expected p_left or var and got a " + token + "\n";
            //        return;
        }
        //(toPrint, "Exit perens\n");
        
        return term;
    } catch (string error) {
        throw error;
    }
    
}

Val parseRel_op(string & toPrint)
{
    try {
        //(toPrint, "Enter rel_op\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        currToken_index++;
        
        Val rel_op = * new Val();
        
        if ( (category.compare("less_than") == 0)     ||
            (category.compare("greater_than") == 0)      ||
            (category.compare("less_than_equal") == 0)   ||
            (category.compare("greater_than_equal") == 0) )
        {
            //(toPrint, "Token Match. Expected rel_op and got a " + token + "\n");
            rel_op.lexeme = "rel_op";
            rel_op.token = token;
        }
        else
        {
            throw "compiler error. Expected rel_op and got a " + token + "\n";
            //        return;
        }
        //(toPrint, "Exit rel_op\n");
        
        return rel_op;
    } catch (string error) {
        throw error;
    }
}


Val parseBool_op(string & toPrint)
{
    try {
        //(toPrint, "Enter bool_op\n");
        string token = tokenList[currToken_index][0];
        string category = tokenList[currToken_index][1];
        currToken_index++;
        
        Val bool_op = * new Val();
        
        if ( (category.compare("and_op") == 0)     ||
            (category.compare("or_op") == 0)      ||
            (category.compare("equal_op") == 0)   ||
            (category.compare("not_equal_op") == 0) )
        {
            //(toPrint, "Token Match. Expected bool_op and got a " + token + "\n");
            
            bool_op.lexeme = "bool_op";
            bool_op.token = token;
            
        }
        else
        {
            throw "compiler error. Expected bool_op and got a " + token + "\n";
            //        return;
        }
        //(toPrint, "Exit bool_op\n");
        return bool_op;
    } catch (string error) {
        throw error;
    }
}

void printOutput(Val outputVal)
{
    if (outputVal.lexeme.compare("var") == 0)
    {
        cout << "Output: " << outputVal.token << " is " << outputVal.value << endl;
    }
    else
    {
        cout << "Output: " << outputVal.token << endl;
    }
}


int main(int argc, const char * argv[]) {
    
    try {
        int quittingtime = 0;
        
        makeMap();
        
        runtimeStack.push_on();
        
        if(argc > 1){
            
            string inputDataFileName( argv[1] );
            ifstream inFile(inputDataFileName.c_str());
            string line;
            int position = 0;
            string stringToTokenize;
            
            while(getline(inFile, line)){
                
                stringToTokenize += line;
            }
            
            for(int i = 0; i < stringToTokenize.length(); i++){
                
                if(stringToTokenize[i] == '\n' || stringToTokenize[i] == '\t' || stringToTokenize[i] == '\r'){
                    
                    inputleng++;
                    
                }
                
                else if(stringToTokenize[i] == '{' || stringToTokenize[i] == '}'){
                    inputleng++;
                    
                }
                
                else{
                    
                    userinput[i] = stringToTokenize[i];
                    inputleng++;
                }
                
                
            }
            
            inputleng++;
            userinput[inputleng] = ' ';
            inputleng++;
            userinput[inputleng] = '$';
            
            // Do stuff
            // tokenize input
            // tokenList = tokenize();  // your implementation here will likely vary
            //            makeMap();
            tokenize();
            
            cout << endl;
            cout << "Number of Characters read:  " << inputleng << endl;
            cout << "Input:  ";
            for (int i = 0; i <= inputleng;  i++)// mimic input to confirm userinput array is populated.
                cout << userinput[i];
            cout << endl << endl;
            
            currToken_index = 0;
            Val output;
            string stringToPrint;
            
            try {
                output = parseProgram(stringToPrint);
            }
            catch (string error)
            {
                // cout << stringToPrint << endl;
                throw error;
            }
            
            cout << endl;
            //                        cout << stringToPrint << endl;
            //            cout << "Output lexeme: " << output.lexeme << endl;
            //            cout << "Output token: " << output.token << endl;
            //            cout << "Output type: " << output.type << endl;
            //            cout << "Output value: " << output.value << endl;
            printOutput(output);
            
            tokenList.clear();
                     
            while (!quittingtime)
            {
                mode_interactive = true;
                
                userinput[0] = ' ';
                
                reader(); // read input and store globally to userinput
                Val output;
                
                if (matches(pos, 4, "end"))    // quit, rather than use "matches" you can replace this with a condition for the "quit" token
                    quittingtime = 1;
                else
                {
                    if (userinput[inputleng] == ENDOFINPUT) {
                        // Do stuff
                        // tokenize input
                        // tokenList = tokenize();  // your implementation here will likely vary
                        
                        
                        
                        tokenize();
                        
                        string stringToPrint = "";
                        currToken_index = 0;
                        
                        try {
                            output = parseProgram(stringToPrint);
                        }
                        catch (string error)
                        {
                            throw error;
                        }
                        
                        cout << endl;
                        //                        cout << stringToPrint << endl;
                        //                        cout << "Output lexeme: " << output.lexeme << endl;
                        //                        cout << "Output token: " << output.token << endl;
                        //                        cout << "Output type: " << output.type << endl;
                        //                        cout << "Output value: " << output.value << endl;
                        printOutput(output);
                        
                        
                        
                    }
                    else
                    {
                        cout << "Input Error: token found but end of input expected:  " << userinput[inputleng] << endl;
                    }
                }
                
                /************************
                 ******* Print Input *******
                 *************************/ // you will remove this and instead print the token list
                //userinput[inputleng] = '\0'; // for printing
                // tokenize();
              //  cout << "Number of Characters read:  " << inputleng << endl;
              //  cout << "Input:  ";
              //  for (int i = 0; i <= inputleng;  i++)// mimic input to confirm userinput array is populated.
             //       cout << userinput[i];
             //   cout << endl;
                
                tokenList.clear();
            }
            
        } // END read from file
        
        else{
            while (!quittingtime)
            {
                reader(); // read input and store globally to userinput
                Val output;
                
                mode_interactive = true;
                
                if (matches(pos, 4, "quit"))    // quit, rather than use "matches" you can replace this with a condition for the "quit" token
                    quittingtime = 1;
                else
                {
                    if (userinput[inputleng] == ENDOFINPUT) {
                        // Do stuff
                        // tokenize input
                        // tokenList = tokenize();  // your implementation here will likely vary
                        
                        makeMap();
                        
                        cout << endl;
                        tokenize();
                        
                        string stringToPrint = "";
                        currToken_index = 0;
                        
                        try {
                            output = parseProgram(stringToPrint);
                        }
                        catch (string error)
                        {
                            throw error;
                        }
                        
                        cout << endl;
                        //                        cout << stringToPrint << endl;
                        
                        //                        cout << "Output lexeme: " << output.lexeme << endl;
                        //                        cout << "Output token: " << output.token << endl;
                        //                        cout << "Output type: " << output.type << endl;
                        //                        cout << "Output value: " << output.value << endl;
                        
                        printOutput(output);
                        
                        //                    Val test;
                        //                    test.token = "x";
                        //                    test.lexeme = "var";
                        //                    test.value = "3";
                        
                        //                    var_table_global.add(test);
                        //                    cout << "table has x: " << var_table_global.has("x") << endl;
                        //                    cout << "x: " << var_table_global.get("x").value << endl;
                    }
                    else
                    {
                        cout << "Input Error: token found but end of input expected:  " << userinput[inputleng] << endl;
                    }
                }
                
                //            /************************
                //             ******* Print Input *******
                //             *************************/ // you will remove this and instead print the token list
                //            //userinput[inputleng] = '\0'; // for printing
                //            // tokenize();
                //            cout << "Number of Characters read:  " << inputleng << endl;
                //            cout << "Input:  ";
                //            for (int i = 0; i <= inputleng;  i++)// mimic input to confirm userinput array is populated.
                //                cout << userinput[i];
                //            cout << endl;
                
                
                tokenList.clear();
            }
            
            
            runtimeStack.pop();
            return 0;
            
        }
        
    } catch (string error) {
        cout << error << endl;
    }
    
}






