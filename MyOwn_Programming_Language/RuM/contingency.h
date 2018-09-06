////
////  contingency.h
////  RuM
////
////  Created by Konrad Rauscher on 11/24/16.
////  Copyright © 2016 Konrad Rauscher. All rights reserved.
////
//
//#ifndef contingency_h
//#define contingency_h
//
////
////  main.cpp
////  RuM
////
////  Created by Konrad Rauscher on 10/22/16.
////  Copyright © 2016 Konrad Rauscher. All rights reserved.
////
//
//
//// QUESTIONS:
//// what should be the highest-non terminal in a if or while statement? is stmt_list ok?
//
////  --- TESTING ---
//// negative values
//// error (exception?) handling
//// when should var tables be updated? be same from previous runs?
//
//
//
//
//// from Bryce:
//// Inheritance doesn’t look parsed correctly, parses inherits twice, once as inherit_key and once as a var, and from as a var -1
//
//
//// to do:
///*
// 
// FIX: math, bool, relational need to access var values from table
// 
// make .h files? for parsing, tokenization
// 
// Don't use var values from prvious run --> clear var_table_global each time prompt is displayed?
// 
// 
// ****
// build runtime stack class --> pushing back activation recods (another class, that contains var table, function table,
// 
// build symbol table classes --> var, functions,
// 
// implement try catch for error handling, throwing
// 
// 
// account for strings in math operations --> error handling
// 
// 
// add error handling, functionality for strings in math, relational, bool
// 
// add var handling for ALL math, bool, and relational operations
// 
// 
// add bool handling for math operations?
// 
// 
// 
// --- Milestone 2 ---
// don't need to validate code inside if stmt
// ignore scope for if stmts and while loops
// 
// function definition --> each func has own scope
// - bind name of function, and all of its attributes in function table
// 
// Function name, param list, code, name of return variable
// 
// 
// 
// function invocation
// - scope
// - execution of the subprogram
// 
// 
// 
// finish bool handling
// 
// */
//
//#include <iostream>
//#include <cstdlib>
//#include <map>
//#include <string>
//#include <sstream>
//#include <vector>
//#include <stack>
//#include <fstream>
//#include <cstring>
//#include <cmath>
//
//#include "DataTypes.h"
//#include "SymbolTables.h"
//#include "Runtime_Stack.h"
//
//using namespace std;
//
//enum CharType{
//    
//    LETTER,
//    DIGIT,
//    LASTCHAR,
//    UNKNOWN,
//    SPACE,
//    SINGLE_PEREN
//};
//
////enum
//
////constant definitions
//const int  NAMELENG = 100;      /* Maximum length of a name */
//const int  MAXINPUT = 40000;     /* Maximum length of an input */
//const char*   PROMPT = "RuM_interpreter> ";
//const char*   PROMPT2 = "--> ";
//const char  COMMENTCHAR = '%';
//const char  ENDOFINPUT = '$';   // use as sentinel to mark end of input
//
////global variables
//
//int charClass;
//char lexeme[NAMELENG];
//char nextChar;
//int lexemeLength;
////string lexemeString;
//map<string, string> tokenTypes;
//char userinput[MAXINPUT];
//int inputleng, pos;
//int enterIfScope = 0;
//string currentOutput = "Output: No statements";
//int lexemeStartPlace = 0;
//vector<vector<string> > tokenList;
//int currToken_index = 0;  //
//
////function definitions
//
//int isChar(int pos);
//void makeMap();
//void tokenize();
//void getChar();
//int lex();
//void addChar();
//int classifyChars(int pos);
//void skipBlankSpace();
//
////function implementations
//
//
//// NEW for RuM:
//var_Table var_table_global;
//
//
///*********************************
// LOW-LEVEL HELPER METHODS FOR READING INPUT
// *********************************/
//void nextchar(char& c) // pulls next char
//{
//    scanf("%c", &c);
//    if (c == COMMENTCHAR)
//    {
//        while (c != '\n')
//            scanf("%c", &c);
//    }
//}
//
//// skips over blanks
//int skipblanks(int p)
//{
//    while (userinput[p] == ' ')
//        ++p;
//    return p;
//}
//
//int isDelim(char c) // checks char for particular delimiters
//{
//    return ((c == '(') || (c == ')') || (c == ' ') || (c == COMMENTCHAR) || (c == '\n'));
//}
//
//int matches(int s, int leng, char* nm) // lookahead checks input for matching string. Helps to check for keywords.
//{
//    int i = 0;
//    int tester = 0;
//    
//    while (i < leng)
//    {
//        
//        if (userinput[s] != nm[i]) {
//            
//            //  cout << nm << endl;
//            //            cout << userinput[s] << endl;
//            return 0;
//        }
//        ++i;
//        ++s;
//    }
//    if (!isDelim(userinput[s]))
//        return 0;
//    
//    return 1;
//}/* matches */
//
//int matches(int s, int leng, char* nm, bool testing) // lookahead checks input for matching string. Helps to check for keywords.
//{
//    int i = 0;
//    int tester = 0;
//    
//    while (i < leng)
//    {
//        if (userinput[s] != nm[i]) {
//            return 0;
//        }
//        ++i;
//        ++s;
//    }
//    
//    
//    return 1;
//}/* matches */
//
//
//// assures that scanf waits until matching paren {  } is input. Tokenizer will not begin until a match is found.
//// This function is not necessary but allows for easy input of large blocks of code
//void readParens()
//{
//    int parencnt; /* current depth of parentheses */
//    char c = ' ';
//    parencnt = 1; // '(' just read
//    do
//    {
//        if (c == '\n')
//            cout << PROMPT2;
//        nextchar(c);
//        pos++;
//        if (pos == MAXINPUT)
//        {
//            cout << "User input too long\n";
//            exit(1);
//        }
//        if (c == '\n') {
//            userinput[pos] = ' ';
//        }
//        
//        else
//            userinput[pos] = c;
//        if (c == '{') // holds the prompt for blocks of code
//            ++parencnt;
//        if (c == '}')
//            parencnt--;
//    } while (parencnt != 0);
//} //readParens
//
//
//
//// reads input from console and stores in global userinput[]
//void readInput()
//{
//    char  c;
//    cout << PROMPT;
//    pos = 0;
//    do
//    {
//        ++pos;
//        if (pos == MAXINPUT)
//        {
//            cout << "User input too long\n";
//            exit(1);
//        }
//        nextchar(c);
//        if (c == '\n')
//            userinput[pos] = ' ';// or space
//        else
//            userinput[pos] = c;
//        
//        if (userinput[pos] == '{')  // if pos >6 and matches(pos-1,6,"define")
//            readParens();// readDef  ... in def until read key word end
//    } while (c != '\n');
//    inputleng = pos + 1;
//    userinput[pos+1] = ENDOFINPUT; // sentinel
//    // input buffer is now filled, reset pos to 1 (later in reader()) and begin parse
//}
//
//// reads input from console and stores in global userinput[]
//void readInputFromFile()
//{
//    char  c;
//    //cout << PROMPT;
//    pos = 0;
//    do
//    {
//        ++pos;
//        if (pos == MAXINPUT)
//        {
//            cout << "User input too long\n";
//            exit(1);
//        }
//        nextchar(c);
//        if (c == '\n')
//            userinput[pos] = ' ';// or space
//        else
//            userinput[pos] = c;
//        
//        if (userinput[pos] == '{')  // if pos >6 and matches(pos-1,6,"define")
//            readParens();// readDef  ... in def until read key word end
//    } while (c != '\n');
//    inputleng = pos + 1;
//    userinput[pos+1] = ENDOFINPUT; // sentinel
//    // input buffer is now filled, reset pos to 1 (later in reader()) and begin parse
//}
//
//
//// Reads input until end
//void reader()
//{
//    do
//    {
//        readInput();
//        pos = skipblanks(1); // reset pos for tokenizer
//    } while (pos > inputleng-1); // ignore blank lines
//}
//
//void readerFromFile()
//{
//    do
//    {
//        readInputFromFile();
//        pos = skipblanks(1); // reset pos for tokenizer
//    } while (pos > inputleng-1); // ignore blank lines
//}
//
///*********************************
// END OF LOW-LEVEL HELPER METHODS FOR READING INPUT
// *********************************/
//
//
///**************************************************
// **********   Some Tokenizer Helpers below  *********
// * Feel free to insert tokenizer code here (or in other file)*
// ***************************************************/
//
////makeMap function creates a mapping of tokens to their token categories
//
//void makeMap(){
//    
//    tokenTypes.insert(make_pair("begin", "start_key"));
//    tokenTypes.insert(make_pair("end", "stop_key"));
//    tokenTypes.insert(make_pair("inherits from", "inherit_key"));
//    tokenTypes.insert(make_pair("classdef", "class_key"));
//    tokenTypes.insert(make_pair("endclass", "endclass_key"));
//    tokenTypes.insert(make_pair("fundef", "fundef_key"));
//    tokenTypes.insert(make_pair("endfun", "endfun_key"));
//    tokenTypes.insert(make_pair("if", "if_key"));
//    tokenTypes.insert(make_pair("endif", "endif_key"));
//    tokenTypes.insert(make_pair("else", "else_key"));
//    tokenTypes.insert(make_pair("while", "while_key"));
//    tokenTypes.insert(make_pair("endwhile", "endwhile_key"));
//    tokenTypes.insert(make_pair("[", "square_left"));
//    tokenTypes.insert(make_pair("]", "square_right"));
//    tokenTypes.insert(make_pair("(", "p_left"));
//    tokenTypes.insert(make_pair(")", "p_right"));
//    tokenTypes.insert(make_pair("new", "new_key"));
//    tokenTypes.insert(make_pair(".", "dot"));
//    tokenTypes.insert(make_pair("+", "plus_op"));
//    tokenTypes.insert(make_pair("-", "minus_op"));
//    tokenTypes.insert(make_pair("*", "mult_op"));
//    tokenTypes.insert(make_pair("/", "div_op"));
//    tokenTypes.insert(make_pair("^", "pow_op"));
//    tokenTypes.insert(make_pair("&&", "and_op"));
//    tokenTypes.insert(make_pair("&", "reference_op")); //added
//    tokenTypes.insert(make_pair("||", "or_op"));
//    tokenTypes.insert(make_pair("==", "equal_op"));
//    tokenTypes.insert(make_pair("!=", "not_equal_op"));
//    tokenTypes.insert(make_pair("<", "less_than"));
//    tokenTypes.insert(make_pair(">", "greater_than"));
//    tokenTypes.insert(make_pair("<=", "less_than_equal"));
//    tokenTypes.insert(make_pair(">=", "greater_than_equal"));
//    tokenTypes.insert(make_pair("end", "stop_key"));
//    tokenTypes.insert(make_pair("'", "single_quote"));
//    tokenTypes.insert(make_pair("=", "assign_op"));
//    tokenTypes.insert(make_pair("!", "bang_op"));
//    tokenTypes.insert(make_pair(",", "comma_token"));
//    tokenTypes.insert(make_pair(";", "semicolon_token"));
//    
//    
//}
//
////helper functions for lexer
//
//int isDigit(int pos)
//{
//    if ((userinput[pos] < '0') || (userinput[pos] > '9'))
//        return 0;
//    else
//        return 1;
//}
//
//// checks to see if user input at index pos is char
//int isChar(int pos)
//{
//    if ((userinput[pos] >= 'a') && (userinput[pos] <= 'z') ||
//        (userinput[pos] >= 'A') && (userinput[pos] <= 'Z') ||
//        userinput[pos] == '_')
//        return 1;
//    else
//        return 0;
//}
//
//int isNewLine(int pos) {
//    
//    pos = skipblanks(pos);
//    if (userinput[pos] == '\n') {
//        return 1;
//    }
//    else {
//        return 0;
//    }
//}
//
//int isParen(int pos)
//{
//    if ((userinput[pos] == '(') || (userinput[pos] == ')'))
//        return 1;
//    else
//        return 0;
//}
//
////helper functions for lexer implemented by me
//
////getchar function will get the next character of the input and classify it
//
///*int classifyChars(int pos){
// 
// 
// 
// 
// }*/
//
//
//void getChar(int position){
//    
//    nextChar = userinput[position];
//    
//    // cout << "the nextchar variable is equal to: " << nextChar << endl;
//    
//    if(isChar(position)){
//        charClass = LETTER;
//    }
//    
//    else if(isDigit(position)){
//        charClass = DIGIT;
//        
//    }
//    
//    else if(userinput[position] == '$'){
//        charClass = LASTCHAR;
//    }
//    
//    else if((userinput[position] == '\0') || (userinput[position] == ' ')){
//        charClass = SPACE;
//    }
//    else if(userinput[position] == '\'')
//    {
//        charClass = SINGLE_PEREN;
//    }
//    
//    else{
//        
//        charClass = UNKNOWN;
//    }
//    
//    
//} // end getchar function
//
//
////addchar function will add the next character to a lexeme
//
//void addChar(){
//    
//    if(lexemeLength <= (NAMELENG -2)){
//        // cout << "in the add function " << endl;
//        
//        lexeme[lexemeLength++] = nextChar;
//        lexeme[lexemeLength] = 0;
//        //  lexemeLength++;
//        
//        /*  cout << "lexeme added is: " << lexeme[lexemeLength--];
//         cout << "lexeme length is: " << lexemeLength << endl;
//         for(int i = 1; i < lexemeLength; i++){
//         
//         cout << "the lexeme value is: " << lexeme[i] << endl;
//         
//         } */
//    }
//}
//
//int lex(int position){
//    
//    // cout << "|" << nextChar << "| " << "type: " << charClass << endl;
//    
//    lexemeLength = 0;
//    
//    int count = 0;
//    
//    switch(charClass){
//            
//        case SINGLE_PEREN:
//        {
//            
//            vector<string> token;
//            token.push_back("'");
//            token.push_back("single_quote");
//            tokenList.push_back(token);
//            
//            skipblanks(position);
//            
//            addChar();
//            getChar(++position);
//            
//            //           cout << "lexeme[0]: " << lexeme[0] << endl;
//            
//            
//            while(charClass != SINGLE_PEREN){
//                
//                addChar();
//                getChar(++position);
//                
//            }
//            
//            addChar();
//            getChar(++position);
//            
//            string stringLexeme;
//            
//            if(lexeme[0] == '\'')
//            {
//                //                cout << "HERE " << endl;
//                int i = 1;
//                while ((i < lexemeLength - 1))
//                {
//                    //                    cout << "lexemLength: " << lexemeLength << ", lexeme: " << lexeme[i] << endl;
//                    stringLexeme.push_back(lexeme[i]);
//                    i++;
//                }
//                
//                token.clear();
//                token.push_back(stringLexeme);
//                token.push_back("string");
//                tokenList.push_back(token);
//                
//                token.clear();
//                vector<string> token;
//                token.push_back("'");
//                token.push_back("single_quote");
//                tokenList.push_back(token);
//                
//                memset(&lexeme[0], 0, sizeof(lexeme));
//                
//            }
//            
//            break;
//        }
//            
//        case LETTER:
//        {
//            skipblanks(position);
//            //   cout << "the character is a letter" << endl;
//            while(charClass == LETTER || charClass == DIGIT){
//                
//                addChar();
//                getChar(++position);
//                
//            }
//            
//            string stringLexeme;
//            for(int i = 0; i < lexemeLength; i++){
//                
//                stringLexeme += lexeme[i];
//            }
//            
//            //     cout << "string is " << stringLexeme << endl;
//            
//            if(tokenTypes.count(stringLexeme) > 0){
//                //        cout << "in here" << endl;
//                
//                //     cout << "lexeme is a keyword" << endl;
//                
//                vector<string> tokens;
//                tokens.push_back(stringLexeme);
//                tokens.push_back(tokenTypes.find(stringLexeme)->second);
//                tokenList.push_back(tokens);
//                
//                memset(&lexeme[0], 0, sizeof(lexeme));
//                //lexeme.clear
//            }
//            
//            else{
//                
//                vector<string> token;
//                token.push_back(stringLexeme);
//                token.push_back("var");
//                
//                tokenList.push_back(token);
//                
//                memset(&lexeme[0], 0, sizeof(lexeme));
//                
//            }
//            
//            break;
//        }
//        case DIGIT:
//        {
//            skipblanks(position);
//            
//            bool decimalEncountered = false;
//            
//            //   cout << "in the digit case" << endl;
//            position = skipblanks(position);
//            
//            while(charClass == DIGIT){
//                
//                addChar();
//                getChar(++position);
//                
//            }
//            
//            if(nextChar == '.' && decimalEncountered == false){
//                
//                decimalEncountered = true;
//                
//                //                vector<string> token;
//                //                token.push_back(".");
//                //                token.push_back(tokenTypes.find(".")->second);
//                //                tokenList.push_back(token);
//                
//                addChar();
//                
//                position = skipblanks(position);
//                getChar(++position);
//                
//                while(charClass == DIGIT){
//                    addChar();
//                    getChar(++position);
//                    
//                }
//                
//                string stringLexeme;
//                for(int i = 0; i < lexemeLength; i++){
//                    stringLexeme += lexeme[i];
//                }
//                
//                vector<string> token;
//                token.push_back(stringLexeme);
//                token.push_back("float");
//                
//                tokenList.push_back(token);
//                
//                memset(&lexeme[0], 0, sizeof(lexeme));
//                
//            }
//            
//            else{
//                
//                string stringLexeme;
//                for(int i = 0; i < lexemeLength; i++){
//                    stringLexeme += lexeme[i];
//                }
//                
//                vector<string> token;
//                token.push_back(stringLexeme);
//                token.push_back("integer");
//                
//                tokenList.push_back(token);
//                
//                memset(&lexeme[0], 0, sizeof(lexeme));
//                
//            }
//            
//            break;
//            
//        }
//            
//        case SPACE:
//        {
//            position++;
//            break;
//        }
//            
//        case UNKNOWN:{
//            
//            skipblanks(position);
//            
//            
//            if ((nextChar == '&') || (nextChar == '|') || (nextChar == '<') || (nextChar == '>') || (nextChar == '=') || (nextChar == '!')   )
//            {
//                
//                
//                while( (charClass == UNKNOWN) && ( (nextChar == '&') || (nextChar == '|') || (nextChar == '<') || (nextChar == '>') || (nextChar == '=') || (nextChar == '!')  ) ){
//                    
//                    // cout << nextChar << endl;
//                    addChar();
//                    //      cout << nextChar << endl;
//                    getChar(++position);
//                    
//                }
//            }
//            else
//            {
//                addChar();
//                //cout << nextChar << endl;
//                getChar(++position);
//            }
//            
//            string stringLexeme;
//            
//            //cout << "lexemLength: " << lexemeLength << endl;
//            for(int i = 0; i < lexemeLength; i++){
//                
//                //         cout << "in this for loop" << endl;
//                
//                //ensure surrounding space is not included
//                
//                if(lexeme[i] != ' '){
//                    stringLexeme += lexeme[i];
//                }
//                
//                //       cout << "string lexeme is " << stringLexeme << endl;
//                
//            }
//            
//            if(stringLexeme.length() > 1){ // if lexeme length is more than 1
//                
//                if(tokenTypes.count(stringLexeme) == 0){ // if lexeme not in tokenTypes
//                    
//                    for(int i = 0; i < lexemeLength-1; i++){
//                        
//                        string s;
//                        s.push_back(lexeme[i]);
//                        vector<string> token;
//                        token.push_back(s);
//                        if(tokenTypes.count(s)) {
//                            token.push_back(tokenTypes.find(s)->second);
//                            tokenList.push_back(token);
//                            
//                        }
//                    }
//                    
//                    memset(&lexeme[0], 0, sizeof(lexeme));
//                    
//                } // END if double token is not in the thing
//                
//                else{
//                    
//                    vector<string> token;
//                    token.push_back(stringLexeme);
//                    token.push_back(tokenTypes.find(stringLexeme)->second);
//                    tokenList.push_back(token);
//                    memset(&lexeme[0], 0, sizeof(lexeme));
//                    
//                }
//                
//            } // end check for double unknown characters
//            
//            else{ // lexeme length is 1
//                
//                // check if token is '
//                
//                // cout << stringLexeme << endl;
//                
//                vector<string> token;
//                token.push_back(stringLexeme);
//                
//                if(tokenTypes.count(stringLexeme))
//                {
//                    // cout << tokenTypes.find(stringLexeme)->second << endl;
//                    token.push_back(tokenTypes.find(stringLexeme)->second);
//                }
//                
//                tokenList.push_back(token);
//                memset(&lexeme[0], 0, sizeof(lexeme));
//                
//            }
//            
//            break;
//        }
//            
//            
//        case LASTCHAR:
//            position++;
//            break;
//    }
//    
//    return position;
//    
//}
//
//void tokenize(){
//    
//    int count = 0;
//    while(count < inputleng){
//        
//        //    cout << "in this while loop where input length is : " << inputleng;
//        
//        getChar(count);
//        count = lex(count);
//        
//    }
//    
//    for(int i = 0; i < tokenList.size(); i++){ // remove empty elements
//        
//        if((tokenList[i][1] == "") || (tokenList[i][0] == ""))
//        {
//            tokenList.erase(tokenList.begin() + i);
//        }
//        
//    }
//    
//    for(int i = 0; i < tokenList.size(); i++)
//    {
//        cout << "Token is: " << tokenList[i][0] << ". Category is: " << tokenList[i][1] << "." << endl;
//    }
//    
//    // tokenList.clear();
//    
//    // count ++;
//}
//
//// BEGIN PARSER CODE
//
//// prototypes
//Val parseVar(string & toPrint);
//Val parseInteger(string & toPrint);
//Val parseString_token(string & toPrint);
//Val parseTerm(string & toPrint);
//Val parseMath_pow(string & toPrint);
//Val parseFloat(string &);
//Val parseNeg(string & toPrint);
//Val parseLiteral(string & toPrint);
//Val parseMath(string & toPrint);
//Val parseMath_value(string & toPrint);
//Val parseMath_term(string & toPrint);
//Val parseRel_value(string &);
//Val parseRel_expr(string &);
//Val parseRel_op(string &);
//void parseFunction_invoc(string &);
//void parseOb_create(string &);
//void parseOb_manip(string &);
//void parseList_element(string &);
//Val parseBool_exp(string &);
//Val parseVar_assign(string &);
//void parseList(string &);
//Val parseIf_stmt(string &);
//Val parseWhile(string &);
//Val parseExpr(string &);
//Val parseStmt(string &);
//void parseParams(string &);
//Val parseStmt_list(string &);
//void parseFunction_def(string &);
//void parseClass_def(string &);
//Val parsePerens(string &);
//void parseList_contents(string &);
//
//
//void append(string & stringA , string stringB)
//{
//    stringA += stringB;
//}
//
//// program
//Val parseProgram(string & toPrint)
//{
//    Val prog;
//    
//    try {
//        append(toPrint, "Enter Program\n");
//        string token = tokenList[currToken_index][0];
//        string category = tokenList[currToken_index][1];
//        
//        if (category.compare("start_key") == 0)
//        {
//            append(toPrint, "Token Match. Expected start_key and got a " + token + "\n");
//            
//            
//            //for (int i = 0; i < tokenList.size(); i++)
//            //{
//            //  cout << tokenList[i][0] << " " << tokenList[i][1] << endl;
//            //}
//            
//            currToken_index++;
//            token = tokenList[currToken_index][0];
//            category = tokenList[currToken_index][1];
//            
//            if (category.compare("class_key") == 0)         // check for possible class_def
//            {
//                parseClass_def(toPrint);
//            }
//            
//            token = tokenList[currToken_index][0];
//            category = tokenList[currToken_index][1];
//            if (category.compare("fundef_key") == 0)          // check for possible function_def
//            {
//                parseFunction_def(toPrint);
//            }
//            
//            token = tokenList[currToken_index][0];
//            category = tokenList[currToken_index][1];   // check for possible stmt_list
//            if ( (category.compare("integer") == 0) ||  // stmt --> !, var, math, if, while, square_left, p_left as first term
//                (category.compare("single_quote") == 0) ||
//                (category.compare("float") == 0) ||
//                (category.compare("minus_op") == 0)  ||
//                (category.compare("var") == 0) ||
//                (category.compare("bang_op") == 0) ||
//                (category.compare("if_key") == 0) ||
//                (category.compare("while_key") == 0) ||
//                (category.compare("square_left") == 0) ||
//                (category.compare("p_left") == 0) )
//            {
//                prog = parseStmt_list(toPrint);
//            }
//            
//            // currToken_index--; // go back ?
//            if (currToken_index >= tokenList.size())
//            {
//                return prog;
//            }
//            
//            token = tokenList[currToken_index][0];
//            category = tokenList[currToken_index][1];
//            if (category.compare("stop_key") == 0)         // check for end
//            {
//                append(toPrint, "Token Match. Expected stop_key and got a " + token + "\n");
//                currToken_index++;
//            }
//            else
//            {
//                // error
//                append(toPrint, "Token Error. Expected stop_key and got a " + token + "\n");
//                //                return;
//            }
//            
//        }
//        else
//        {
//            // error
//            append(toPrint, "Token Error. Expected start_key and got a " + token + "\n");
//            //            return;
//        }
//        append(toPrint, "Exit Program\n");
//        
//        
//        
//        
//    } catch (string error) {
//        error;
//    }
//    
//    return prog;
//}
//
//// class_def
//void parseClass_def(string & toPrint)
//{
//    
//    try {
//        append(toPrint, "Enter class_def\n");
//        string token = tokenList[currToken_index][0];
//        string category = tokenList[currToken_index][1];
//        
//        if (category.compare("class_key") == 0)
//        {
//            append(toPrint, "Token Match. Expected class_key and got a " + token + "\n");
//            
//            currToken_index++; // get next
//            token = tokenList[currToken_index][0];
//            category = tokenList[currToken_index][1];
//            
//            // check for var
//            if(category.compare("var") == 0)
//            {
//                
//                currToken_index++; // get next
//                token = tokenList[currToken_index][0];
//                category = tokenList[currToken_index][1];
//                
//                if (category.compare("inherit_key") == 0)
//                {
//                    append(toPrint, "Token Match. Expected possible inherit_key and got a " + token + "\n");
//                    
//                    currToken_index++; // get next
//                    parseVar(toPrint);
//                    
//                }
//                
//                // update
//                token = tokenList[currToken_index][0];
//                category = tokenList[currToken_index][1];
//                
//                // check for optional stmt_list
//                if ( (category.compare("integer") == 0) ||  // stmt --> !, var, math, if, while, square_left as first term
//                    (category.compare("single_quote") == 0) ||
//                    (category.compare("float") == 0) ||
//                    (category.compare("minus_op") == 0)  ||
//                    (category.compare("var") == 0) ||
//                    (category.compare("bang_op") == 0) ||
//                    (category.compare("if_key") == 0) ||
//                    (category.compare("while_key") == 0) ||
//                    (category.compare("square_left") == 0) ||
//                    (category.compare("p_left") == 0) )
//                {
//                    append(toPrint, "Token Match. Expected possible stmt_list term and got a " + token + "\n");
//                    
//                    parseStmt_list(toPrint);
//                    
//                }
//                
//                // update
//                token = tokenList[currToken_index][0];
//                category = tokenList[currToken_index][1];
//                
//                // check for optional function_def
//                if (category.compare("fundef_key") == 0) // check for optional inherits from
//                {
//                    append(toPrint, "Token Match. Expected possible fundef_key and got a " + token + "\n");
//                    
//                    parseFunction_def(toPrint);
//                    
//                }
//                
//                token = tokenList[currToken_index][0];
//                category = tokenList[currToken_index][1];
//                // check for endclass
//                if (category.compare("endclass_key") == 0) // check for optional inherits from
//                {
//                    append(toPrint, "Token Match. Expected endclass_key and got a " + token + "\n");
//                    currToken_index++;
//                    
//                }
//                else
//                {
//                    append(toPrint, "Token Error. Expected endclass_key and got a " + token + "\n");
//                    return;
//                }
//                
//            }
//            else
//            {
//                // error
//                append(toPrint, "Token Error. Expected var and got a " + token + "\n");
//                return;
//            }
//            
//            
//        }
//        else
//        {
//            // error
//            append(toPrint, "Token Error. Expected class_key and got a " + token + "\n");
//            return;
//            
//        }
//        append(toPrint, "Exit class_def\n");
//    } catch (string error) {
//        error;
//    }
//}
//
//// function_def
//void parseFunction_def(string & toPrint)
//{
//    
//    try {
//        append(toPrint, "Enter function_def\n");
//        string token = tokenList[currToken_index][0];
//        string category = tokenList[currToken_index][1];
//        
//        if (category.compare("fundef_key") == 0)
//        {
//            
//            append(toPrint, "Token Match. Expected fundef_key and got a " + token + "\n");
//            
//            currToken_index++; // go to next token
//            token = tokenList[currToken_index][0];
//            category = tokenList[currToken_index][1];
//            
//            if (category.compare("var") == 0)
//            {
//                parseVar(toPrint);
//                
//                token = tokenList[currToken_index][0];
//                category = tokenList[currToken_index][1];
//                
//                // check next
//                if (category.compare("assign_op") == 0)
//                {
//                    
//                    append(toPrint, "Token Match. Expected assign_op and got a " + token + "\n");
//                    
//                    currToken_index++; // go to next token
//                    
//                    parseVar(toPrint);
//                    
//                    
//                    token = tokenList[currToken_index][0];
//                    category = tokenList[currToken_index][1];
//                    
//                    
//                    // check for left peren
//                    
//                    if (category.compare("p_left") == 0)
//                    {
//                        
//                        append(toPrint, "Token Match. Expected p_left and got a " + token + "\n");
//                        
//                        currToken_index++; // go to next token
//                        token = tokenList[currToken_index][0];
//                        category = tokenList[currToken_index][1];
//                        
//                        // check for optional params inside perens
//                        if ( (category.compare("integer") == 0) ||  // params --> expr --> !, var, or math as first term
//                            (category.compare("single_quote") == 0) ||
//                            (category.compare("float") == 0) ||
//                            (category.compare("minus_op") == 0)  ||
//                            (category.compare("var") == 0) ||
//                            (category.compare("bang_op") == 0) ||
//                            (category.compare("square_left") == 0) ||
//                            (category.compare("p_left") == 0) ||
//                            (category.compare("reference_op") == 0) )
//                        {
//                            // parse params
//                            parseParams(toPrint);
//                        }
//                        
//                        token = tokenList[currToken_index][0];
//                        category = tokenList[currToken_index][1];
//                        
//                        // check for p_right
//                        if (category.compare("p_right") == 0)
//                        {
//                            append(toPrint, "Token Match. Expected p_right and got a " + token + "\n");
//                            currToken_index++;
//                            
//                            token = tokenList[currToken_index][0];
//                            category = tokenList[currToken_index][1];
//                            
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
//                            
//                            token = tokenList[currToken_index][0];
//                            category = tokenList[currToken_index][1];
//                            
//                            // check for endfun
//                            if (category.compare("endfun_key") == 0)
//                            {
//                                append(toPrint, "Token Match. Expected endfun_key and got a " + token + "\n");
//                                currToken_index++;
//                            }
//                            else
//                            {
//                                // error
//                                append(toPrint, "Token Error. Expected endfun_key and got a " + token + "\n");
//                                return;
//                            }
//                            
//                        }
//                        else
//                        {
//                            append(toPrint, "Token Error. Expected p_Right and got a " + token + "\n");
//                            return;
//                        }
//                        
//                    }
//                    else
//                    {
//                        // error
//                        append(toPrint, "Token Error. Expected p_left and got a " + token + "\n");
//                        return;
//                    }
//                    
//                }
//                
//            }
//            else
//            {
//                // error
//                append(toPrint, "Token Error. Expected var and got a " + token + "\n");
//                return;
//            }
//        }
//        else
//        {
//            // error
//            append(toPrint, "Token Error. Expected fundef_key and got a " + token + "\n");
//            return;
//            
//        }
//        append(toPrint, "Exit function_def\n");
//    } catch (string error) {
//        error;
//    }
//}
//
//// stmt_list
//Val parseStmt_list(string & toPrint)
//{
//    
//    Val stmt;
//    Val empty;
//    
//    try {
//        append(toPrint, "Enter stmt_list\n");
//        string token = tokenList[currToken_index][0];
//        string category = tokenList[currToken_index][1];
//        
//        
//        
//        if ( (category.compare("integer") == 0) ||  // stmt --> !, var, math, if, while, square_left as first term
//            (category.compare("single_quote") == 0) ||
//            (category.compare("float") == 0) ||
//            (category.compare("minus_op") == 0)  ||
//            (category.compare("var") == 0) ||
//            (category.compare("bang_op") == 0) ||
//            (category.compare("if_key") == 0) ||
//            (category.compare("while_key") == 0) ||
//            (category.compare("square_left") == 0) ||
//            (category.compare("p_left") == 0) )
//        {
//            //        append(toPrint, "Token Match. Expected - and got a " + token + "\n");
//            stmt = parseStmt(toPrint); // assumed: currToken_index incremented
//            
//            token = tokenList[currToken_index][0];
//            category = tokenList[currToken_index][1];
//            
//            if ( (category.compare("semicolon_token") == 0) ) {
//                
//                append(toPrint, "Token Match. Expected possible semicolon_token and got a " + token + "\n");
//                
//                currToken_index++;
//                
//                if (currToken_index >= tokenList.size())
//                {
//                    
//                    //                    string error = "Error: out of bounds";
//                    //                    throw error;
//                    return stmt;
//                }
//                
//                token = tokenList[currToken_index][0];
//                category = tokenList[currToken_index][1];
//                
//                if ( (category.compare("stop_key") == 0) )
//                {
//                    return stmt;
//                }
//                
//            }
//            
//            if ( (category.compare("integer") == 0) ||  // stmt --> !, var, math, if, while, square_left as first term
//                (category.compare("single_quote") == 0) ||
//                (category.compare("float") == 0) ||
//                (category.compare("minus_op") == 0)  ||
//                (category.compare("var") == 0) ||
//                (category.compare("bang_op") == 0) ||
//                (category.compare("if_key") == 0) ||
//                (category.compare("while_key") == 0) ||
//                (category.compare("square_left") == 0) ||
//                (category.compare("p_left") == 0) )
//            {
//                return parseStmt_list(toPrint);  // NEED TO DEAL WITH THIS
//            }
//        }
//        else
//        {
//            // error
//            append(toPrint, "Token Error. Expected possible integer, string, float, minus_op, var, bang_op, if_key, or while_key and got a " + token + "\n");
//            //            return;
//            throw "Token Error. Expected possible integer, string, float, minus_op, var, bang_op, if_key, or while_key and got a " + token + "\n";
//            
//        }
//        
//        append(toPrint, "Exit stmt_list\n");
//        
//        return stmt;
//        
//    } catch (string error) {
//        throw error;
//    }
//}
//
//// stmt
//Val parseStmt(string & toPrint)
//{
//    
//    Val result;
//    
//    try {
//        append(toPrint, "Enter stmt\n");
//        string token = tokenList[currToken_index][0];
//        string category = tokenList[currToken_index][1];
//        // currToken_index++;
//        
//        if (category.compare("if_key") == 0) // if_stmt
//        {
//            // parse if_stmt
//            result = parseIf_stmt(toPrint);
//            
//        }
//        else if (category.compare("while_key") == 0) // while_stmt
//        {
//            // parse while_stmt
//            result = parseWhile(toPrint);
//            
//        }
//        else if ( (category.compare("integer") == 0) ||  // expr --> !, var, or math as first term
//                 (category.compare("single_quote") == 0) ||
//                 (category.compare("float") == 0) ||
//                 (category.compare("minus_op") == 0)  ||
//                 (category.compare("var") == 0) ||
//                 (category.compare("bang_op") == 0) ||
//                 (category.compare("square_left") == 0) ||
//                 (category.compare("p_left") == 0) )
//        {
//            // parse expr
//            result = parseExpr(toPrint);
//            
//        }
//        else
//        {
//            // error
//            append(toPrint, "Token error. Expected <var_assign>, <bool_exp>, <ob_create>, or <list> and got a " + token + "\n");
//            //            return;
//            throw "Token error. Expected <var_assign>, <bool_exp>, <ob_create>, or <list> and got a " + token + "\n";
//            
//            
//        }
//        append(toPrint, "Exit stmt\n");
//        
//        
//        
//    } catch (string error) {
//        error;
//    }
//    
//    return result;
//}
//
//// expr
//Val parseExpr(string & toPrint) // poasible issue -->
//{
//    
//    Val bool_exp;
//    Val empty;
//    Val var;
//    
//    try {
//        append(toPrint, "Enter expr\n");
//        
//        // cout << toPrint << endl;
//        currToken_index++; // use 1st look ahead --> check for '='
//        string token = tokenList[currToken_index][0];
//        string category = tokenList[currToken_index][1];
//        
//        
//        if (category.compare("assign_op") == 0)// var_assign or ob_create test
//        {
//            
//            // check for ob_Create
//            
//            currToken_index++; // use 2nd look a ahead --> check for 'new'
//            token = tokenList[currToken_index][0];
//            category = tokenList[currToken_index][1];
//            
//            if (category.compare("new_key") == 0)
//            {
//                currToken_index--;
//                currToken_index--; // put back to var
//                
//                parseOb_create(toPrint);
//                return empty;
//            }
//            
//            else
//            {
//                currToken_index--; // put back to var
//                currToken_index--;
//                
//                var = parseVar_assign(toPrint);
//                return var;
//            }
//            
//        }
//        else
//        {
//            currToken_index--; // put back to first look ahead
//            token = tokenList[currToken_index][0];
//            category = tokenList[currToken_index][1];
//        }
//        
//        if (category.compare("square_left") == 0) // list[;]
//        {
//            // check list --> [
//            parseList(toPrint);
//            
//        }
//        else if  ( (category.compare("integer") == 0)    ||  // bool_exp --> ! or math as first term
//                  (category.compare("single_quote") == 0)    ||
//                  (category.compare("float") == 0)     ||
//                  (category.compare("minus_op") == 0)  ||
//                  (category.compare("var") == 0)       ||
//                  (category.compare("bang_op") == 0) ||
//                  (category.compare("p_left") == 0) )
//        {
//            
//            bool_exp = parseBool_exp(toPrint);
//            
//        }
//        else
//        {
//            // error
//            append(toPrint, "Token error. Expected <var_assign>, <bool_exp>, <ob_create>, or <list> and got a " + token + "\n");
//            //            return;
//            throw "Token error. Expected <var_assign>, <bool_exp>, <ob_create>, or <list> and got a " + token + "\n";
//        }
//        
//        
//        append(toPrint, "Exit expr\n");
//        
//    } catch (string error) {
//        error;
//    }
//    
//    return bool_exp;
//}
//
//// if_stmt
//Val parseIf_stmt(string & toPrint)
//{
//    
//    append(toPrint, "Enter if_stmt\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    
//    Val result;
//    
//    //currToken_index++;
//    if (category.compare("if_key") == 0)
//    {
//        append(toPrint, "Token Match. Expected if_key and got a " + token + "\n");
//        currToken_index++;
//        token = tokenList[currToken_index][0];
//        category = tokenList[currToken_index][1];
//        
//        if (category.compare("p_left") == 0)
//        {
//            append(toPrint, "Token Match. Expected p_left and got a " + token + "\n");
//            
//            currToken_index++;
//            Val myBool = parseBool_exp(toPrint); // assume currToken_index incremented
//            
//            // SKIPING p_right
//            
//            // lookahead
//            token = tokenList[currToken_index][0];
//            category = tokenList[currToken_index][1];
//            
//            if (category.compare("p_right") == 0)
//            {
//                append(toPrint, "Token Match. Expected p_right and got a " + token + "\n");
//                
//                currToken_index++;
//                
//                // if myBool == 1, parseStmt_list
//                // else, skip over all tokens until end_if key
//                
//                if (stoi(myBool.token) == 1)
//                {
//                    result = parseStmt_list(toPrint);
//                    token = tokenList[currToken_index][0];
//                    category = tokenList[currToken_index][1];
//                }
//                else
//                {
//                    token = tokenList[currToken_index][0];
//                    category = tokenList[currToken_index][1];
//                    while (category.compare("else_key") != 0)
//                    {
//                        currToken_index++;
//                        token = tokenList[currToken_index][0];
//                        category = tokenList[currToken_index][1];
//                    }
//                }
//                
//                if (category.compare("endif_key") == 0)
//                {
//                    append(toPrint, "Token Match. Expected endif_key and got a " + token + "\n");
//                    
//                    currToken_index++;
//                    
//                }
//                else if ((category.compare("else_key") == 0 ) && (stoi(myBool.token) == 0))
//                {
//                    append(toPrint, "Token Match. Expected else_key and got a " + token + "\n");
//                    
//                    currToken_index++;
//                    result = parseStmt_list(toPrint);
//                    
//                    token = tokenList[currToken_index][0];
//                    category = tokenList[currToken_index][1];
//                    
//                    // check for endif_key
//                    if (category.compare("endif_key") == 0)
//                    {
//                        append(toPrint, "Token Match. Expected endif_key and got a " + token + "\n");
//                        
//                        currToken_index++;
//                    }
//                    else
//                    {
//                        // error
//                        append(toPrint, "Token Error. Expected endif_key and got a " + token + "\n");
//                    }
//                    
//                }
//                else
//                {
//                    // error
//                    append(toPrint, "Token Error. Expected else_key and got a " + token + "\n");
//                    return result;
//                }
//                
//            }
//            else
//            {
//                // error
//                append(toPrint, "Token Error. Expected p_right and got a " + token + "\n");
//                return result;
//            }
//            
//        }
//        else
//        {
//            // error
//            append(toPrint, "Token Error. Expected p_left and got a " + token + "\n");
//            return result;
//        }
//        
//    }
//    else
//    {
//        // error
//    }
//    append(toPrint, "Exit if_stmt\n");
//    return result;
//}
//
//// while
//Val parseWhile(string & toPrint)
//{
//    append(toPrint, "Enter while\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    
//    Val result;
//    
//    if (category.compare("while_key") == 0)
//    {
//        append(toPrint, "Token Match. Expected while_key and got a " + token + "\n");
//        
//        // check for left peren
//        currToken_index++;
//        string token = tokenList[currToken_index][0];
//        string category = tokenList[currToken_index][1];
//        if (category.compare("p_left") == 0)
//        {
//            
//            currToken_index++;
//            
//            int index_bool_exp = currToken_index;
//            Val myBool = parseBool_exp(toPrint); // assumed: currtoken incremented
//            
//            // check for <p_right>
//            token = tokenList[currToken_index][0];
//            category = tokenList[currToken_index][1];
//            
//            if (category.compare("p_right") == 0)
//            {
//                
//                append(toPrint, "Token Match. Expected p_right and got a " + token + "\n");
//                
//                currToken_index++;
//                
//                // ALSO need to save the starting index for bool_exp, as need to evaluate this boolean expression after each while iteration
//                
//                int before_stmt_list = currToken_index;
//                
//                while (stoi(myBool.token) == 1)
//                {
//                    currToken_index = before_stmt_list; // set back to begining of stmt list
//                    result = parseStmt_list(toPrint);
//                    int after_stmt_list = currToken_index;
//                    
//                    currToken_index = index_bool_exp; // update bool val
//                    myBool = parseBool_exp(toPrint);
//                    
//                    currToken_index = after_stmt_list; // set position in token list back to after the stmt list
//                    
//                }
//                
//                // check for end_if or else
//                token = tokenList[currToken_index][0];
//                category = tokenList[currToken_index][1];
//                
//                // check for endwhile_key
//                
//                if (category.compare("endwhile_key") == 0)
//                {
//                    append(toPrint, "Token Match. Expected endwhile_key and got a " + token + "\n");
//                    currToken_index++;
//                }
//                else
//                {
//                    // error
//                    append(toPrint, "Token Error. Expected endwhile_key and got a " + token + "\n");
//                    return result;
//                }
//                
//            }
//            else
//            {
//                // error
//                append(toPrint, "Token Error. Expected p_right and got a " + token + "\n");
//                return result;
//            }
//            
//        }
//        else
//        {
//            // error
//            append(toPrint, "Token Error. Expected p_left and got a " + token + "\n");
//            return result;
//        }
//        
//    }
//    else
//    {
//        // error
//        append(toPrint, "Token Error. Expected while_key and got a " + token + "\n");
//        return result;
//    }
//    append(toPrint, "Exit while\n");
//    
//    return result;
//}
//
//// var_assign
//Val parseVar_assign(string & toPrint)
//{
//    append(toPrint, "Enter var_assign\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    
//    Val var;
//    Val expr;
//    
//    if (category.compare("var") == 0)
//    {
//        //        append(toPrint, "Token Match. Expected - and got a " + token + "\n");
//        var = parseVar(toPrint);
//        
//        token = tokenList[currToken_index][0];
//        category = tokenList[currToken_index][1];
//        if (category.compare("assign_op") == 0)
//        {
//            append(toPrint, "Token Match. Expected assign_op and got a " + token + "\n");
//            
//            currToken_index++;
//            expr = parseExpr(toPrint);
//            
//            if (expr.lexeme.compare("var") == 0)
//            {
//                // check if in table
//                
//                if ( var_table_global.has(expr.token)) {
//                    
//                    
//                    if (! (var_table_global.get(expr.token).value.compare("undefined") == 0)) {
//                        // check in table
//                        
//                        var.type = expr.type;
//                        var.value = expr.value;
//                    }
//                    else
//                    {
//                        // throw undefined error
//                        throw "ERROR. Variable " + expr.token + " undefined.";
//                    }
//                }
//                else
//                {
//                    // throw unkown var error
//                    throw "ERROR. Unkown variable: " + expr.token;
//                }
//            }
//            else // string, int, float
//            {
//                var.value = expr.token;
//                var.type = expr.lexeme;
//                
//            }
//            if ( var_table_global.has(var.token) ) // check that val in table; if exists, update
//            {
//                
//                var_table_global.get(var.token).type = var.type;
//                var_table_global.get(var.token).value = var.value;
//            }
//            else if ( ! var_table_global.has(var.token) ) //
//            {
//                var_table_global.add(var); // if not, then add
//            }
//        }
//        else
//        {
//            // error
//            append(toPrint, "Token Match. Expected assign_op and got a " + token + "\n");
//            currToken_index++;
//        }
//    }
//    else
//    {
//        // error
//        append(toPrint, "Token Error. Expected var and got a " + token + "\n");
//        return var;
//        
//    }
//    append(toPrint, "Exit var_assign\n");
//    
//    return var;
//}
//
//// list
//void parseList(string & toPrint)
//{
//    append(toPrint, "Enter List\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    
//    if (category.compare("square_left") == 0)
//    {
//        append(toPrint, "Token Match. Expected square_left and got a " + token + "\n");
//        currToken_index++;
//        
//        parseList_contents(toPrint);
//        //        append(toPrint, "Token Match. Expected - and got a " + token + "\n");
//        
//        
//        string token = tokenList[currToken_index][0];
//        string category = tokenList[currToken_index][1];
//        if (category.compare("square_right") == 0)
//        {
//            append(toPrint, "Token Match. Expected square_right and got a " + token + "\n");
//            currToken_index++;
//        }
//        else
//        {
//            // error
//            append(toPrint, "Token Error. Expected square_right and got a " + token + "\n");
//            currToken_index++;
//        }
//        
//    }
//    else
//    {
//        append(toPrint, "Token Error. Expected square_left and got a " + token + "\n");
//        return;
//    }
//    append(toPrint, "Exit List\n");
//}
//
//void parseList_contents(string & toPrint)
//{
//    
//    append(toPrint, "Enter list_contents\n");
//    // string token = tokenList[currToken_index][0];
//    // string category = tokenList[currToken_index][1];
//    
//    //   if (category.compare("comma_token") == 0) // if coma
//    // { // parseList_contents
//    //     parseList_contents(toPrint);
//    // }
//    // else // parseList_element
//    // {
//    //    parseList_element(toPrint);
//    // }
//    
//    // append(toPrint, "Exit list_element\n");
//    
//    // parseList_element
//    parseList_element(toPrint);
//    
//    string token = tokenList[currToken_index][0]; // check for optional comma
//    string category = tokenList[currToken_index][1];
//    if (category.compare("comma_token") == 0)  // if comma, then parseList_contents
//    {
//        append(toPrint, "Token Match. Expected possible comma_token and got a " + token + "\n");
//        currToken_index++;
//        parseList_contents(toPrint);
//    }
//}
//
//// list_element
//void parseList_element(string & toPrint)
//{
//    append(toPrint, "Enter list_element\n");
//    
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    
//    // decide between list, var, and empty string
//    if (category.compare("var") == 0) // var
//    {
//        parseVar(toPrint);
//    }
//    else if ((category.compare("integer") == 0) ||  // literal check
//             (category.compare("single_quote") == 0) ||
//             (category.compare("float") == 0) )
//    {
//        parseLiteral(toPrint);
//    }
//    else if (category.compare("square_left") == 0)
//    {
//        parseList(toPrint);
//    }
//    else // empty string condition ???
//    {
//        append(toPrint, "Token Match. Expected possible empty string and got a " + token + "\n");
//        currToken_index++;
//    }
//    
//    append(toPrint, "Exit list_element\n");
//}
//
//// object
//void parseObject(string & toPrint)
//{
//    append(toPrint, "Enter Ojbect\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    
//    
//    if (category.compare("var") == 0)
//    {
//        parseVar(toPrint); // assume that parseVar increment currToken_index
//        string token = tokenList[currToken_index][0];
//        string category = tokenList[currToken_index][1];
//        
//        // decide between ob_create and ob_manip
//        if (category.compare("assign_op") == 0) // ob_create
//        {
//            currToken_index--; // put back to <var>
//            parseOb_create(toPrint);
//        }
//        else if (category.compare("dot") == 0) // ob_manip
//        {
//            currToken_index--; // put back to <var>
//            parseOb_manip(toPrint);
//        }
//        else
//        {
//            // error
//            append(toPrint, "Token Error. Expected assign_op or dot and got a " + token + "\n");
//            return;
//        }
//    }
//    else
//    {
//        // error
//        append(toPrint, "Token Error. Expected var and got a " + token + "\n");
//        return;
//    }
//    append(toPrint, "Exit Object\n");
//}
//
//// ob_create
//void parseOb_create(string & toPrint)
//{
//    append(toPrint, "Enter Ob_create\n");
//    
//    
//    parseVar(toPrint);
//    
//    //currToken_index++;
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    
//    if (category.compare("assign_op") == 0)
//    {
//        append(toPrint, "Token Match. Expected assign_op and got a " + token + "\n");
//        
//        // lookahead
//        currToken_index++;
//        token = tokenList[currToken_index][0];
//        category = tokenList[currToken_index][1];
//        if (category.compare("new_key") == 0)
//        {
//            currToken_index++;
//            parsePerens(toPrint);
//        }
//        else
//        {
//            append(toPrint, "Token Error. Expected new_key and got a " + token + "\n");
//            return;
//        }
//    }
//    else
//    {
//        append(toPrint, "Token Error. Expected dot and got a " + token + "\n");
//        return;
//    }
//    append(toPrint, "Exit Ob_create\n");
//}
//
//// ob_manip
//void parseOb_manip(string & toPrint)
//{
//    append(toPrint, "Enter Bool_ex\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    //    currToken_index++;
//    if (category.compare("var") == 0)
//    {
//        parseVar(toPrint);
//        
//        // look ahead
//        //currToken_index++;
//        token = tokenList[currToken_index][0];
//        category = tokenList[currToken_index][1];
//        
//        if (category.compare("dot") == 0)
//        {
//            append(toPrint, "Token Match. Expected possible dot and got a " + token + "\n");
//            currToken_index++;
//            
//            parseFunction_invoc(toPrint);
//        }
//        else
//        {
//            append(toPrint, "Token Error. Expected dot and got a " + token + "\n");
//            return;
//        }
//        
//    }
//    else
//    {
//        
//    }
//    append(toPrint, "Exit Bool_ex\n");
//}
//
//// params
//void parseParams(string & toPrint)
//{
//    append(toPrint, "Enter params\n");
//    
//    
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    
//    if (category.compare("reference_op") == 0)
//    {
//        append(toPrint, "Token Match. Expected possible reference_op and got a " + token + "\n");
//        
//        currToken_index++; // go to expr
//        parseExpr(toPrint); // assumed that parseExpr increases currToken_index
//    }
//    else
//    {
//        parseExpr(toPrint);
//    }
//    
//    token = tokenList[currToken_index][0];
//    category = tokenList[currToken_index][1];
//    
//    if (category.compare("comma_token") == 0) // allow for infinate number expression
//    {
//        append(toPrint, "Token Match. Expected possible comma_token and got a " + token + "\n");
//        
//        currToken_index++; // go to expr
//        
//        parseParams(toPrint); // assumed that parseExpr increases currToken_index
//        
//    }
//    
//    append(toPrint, "Exit params\n");
//}
//
//// bool_exp
//Val parseBool_exp(string & toPrint)
//{
//    
//    //    add var handling
//    
//    Val term1;
//    Val term2;
//    
//    try {
//        append(toPrint, "Enter Bool_exp\n");
//        string token = tokenList[currToken_index][0];
//        string category = tokenList[currToken_index][1];
//        
//        
//        
//        if (category.compare("bang_op") == 0)
//        {
//            append(toPrint, "Token Match. Expected possible bang_op and got a " + token + "\n");
//            currToken_index++;
//            
//            term1 = parseRel_expr(toPrint);
//            
//            // look ahead for possible  {<bool_op> <rel_value>}
//            token = tokenList[currToken_index][0];
//            category = tokenList[currToken_index][1];
//            
//            //            bool bang_op = true;
//            
//            if ( (category.compare("and_op") == 0)     ||
//                (category.compare("or_op") == 0)      ||
//                (category.compare("equal_op") == 0)   ||
//                (category.compare("not_equal_op") == 0)  ) // bool op
//            {
//                
//                //                bool_op.lexeme = category;
//                //                bool_op.token = token;
//                append(toPrint, "Token Match. Expected possible bool_op and got a " + token + "\n");
//                currToken_index++;
//                
//                term2 = parseRel_expr(toPrint);
//                
//                if ( var_table_global.has(term2.token)) {
//                    
//                    // check defined
//                    if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                    {
//                        
//                    }
//                    else
//                    {
//                        throw "ERROR. Variable " + term2.token + " undefined.";
//                    }
//                    
//                }
//                else
//                {
//                    // throw unkown var error
//                    throw "ERROR. Unkown variable: " + term2.token;
//                    
//                }
//                
//                
//                // if term 1 var, need to use val AND set type and value back to undefined
//                if (term1.lexeme.compare("var") == 0)
//                {
//                    if ( var_table_global.has(term1.token)) {
//                        
//                        // check defined
//                        if ( !(var_table_global.get(term1.token).value.compare("undefined") == 0) )
//                        {
//                            
//                            // obtain term1: val, type
//                            //                            test
//                            
//                            if (term1.type.compare("integer") == 0)
//                            {
//                                int term1_int = stoi(term1.value);
//                                
//                                if (term2.lexeme.compare("integer") == 0)
//                                {
//                                    if (category.compare("and_op") == 0)
//                                    {
//                                        bool result = ! (term1_int && stoi(term2.token));
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (category.compare("or_op") == 0)
//                                    {
//                                        bool result = ! (term1_int || stoi(term2.token));
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (category.compare("equal_op") == 0)
//                                    {
//                                        bool result = ! (term1_int == stoi(term2.token));
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (category.compare("not_equal_op") == 0)
//                                    {
//                                        bool result = ! (term1_int != stoi(term2.token));
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                }
//                                else if (term2.lexeme.compare("float") == 0)
//                                {
//                                    if (category.compare("and_op") == 0)
//                                    {
//                                        bool result = ! (term1_int && stod(term2.token));
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (category.compare("or_op") == 0)
//                                    {
//                                        bool result = ! (term1_int || stod(term2.token));
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (category.compare("equal_op") == 0)
//                                    {
//                                        bool result = ! (term1_int == stod(term2.token));
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (category.compare("not_equal_op") == 0)
//                                    {
//                                        bool result = ! (term1_int != stod(term2.token));
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                }
//                                else if (term2.lexeme.compare("string") == 0)
//                                {
//                                    if (category.compare("and_op") == 0)
//                                    {
//                                        bool result = ! term1_int;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (category.compare("or_op") == 0)
//                                    {
//                                        bool result = ! true;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (category.compare("equal_op") == 0)
//                                    {
//                                        bool result = ! false;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (category.compare("not_equal_op") == 0)
//                                    {
//                                        bool result = ! true;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    
//                                }
//                                else if (term2.lexeme.compare("var") == 0)
//                                {
//                                    if ( var_table_global.has(term2.token)) {
//                                        
//                                        // check defined
//                                        if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                        {
//                                            if (term2.type.compare("integer") == 0)
//                                            {
//                                                if (category.compare("and_op") == 0)
//                                                {
//                                                    bool result = ! (term1_int && stoi(term2.value));
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("or_op") == 0)
//                                                {
//                                                    bool result = ! (term1_int || stoi(term2.value));
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("equal_op") == 0)
//                                                {
//                                                    bool result = ! (term1_int == stoi(term2.value));
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("not_equal_op") == 0)
//                                                {
//                                                    bool result = ! (term1_int != stoi(term2.value));
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                            }
//                                            else if (term2.type.compare("float") == 0)
//                                            {
//                                                if (category.compare("and_op") == 0)
//                                                {
//                                                    bool result = ! (term1_int && stod(term2.value));
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("or_op") == 0)
//                                                {
//                                                    bool result = ! (term1_int || stod(term2.value));
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("equal_op") == 0)
//                                                {
//                                                    bool result = ! (term1_int == stod(term2.value));
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("not_equal_op") == 0)
//                                                {
//                                                    bool result = ! (term1_int != stod(term2.value));
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                            }
//                                            else if (term2.type.compare("string") == 0)
//                                            {
//                                                if (category.compare("and_op") == 0)
//                                                {
//                                                    bool result = ! term1_int;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("or_op") == 0)
//                                                {
//                                                    bool result = ! true;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("equal_op") == 0)
//                                                {
//                                                    bool result = ! false;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("not_equal_op") == 0)
//                                                {
//                                                    bool result = ! true;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                            }
//                                        }
//                                        else
//                                        {
//                                            throw "ERROR. Variable " + term2.token + " undefined.";
//                                        }
//                                        
//                                    }
//                                    else
//                                    {
//                                        // throw unkown var error
//                                        throw "ERROR. Unkown variable: " + term2.token;
//                                    }
//                                }
//                            }
//                            else if (term1.type.compare("float") == 0)
//                            {
//                                double term1_double = stod(term1.value);
//                                
//                                if (term2.lexeme.compare("integer") == 0)
//                                {
//                                    
//                                    if (category.compare("and_op") == 0)
//                                    {
//                                        bool result = ! false;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (category.compare("or_op") == 0)
//                                    {
//                                        bool result = ! stoi(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (category.compare("equal_op") == 0)
//                                    {
//                                        bool result = ! false;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (category.compare("not_equal_op") == 0)
//                                    {
//                                        bool result = ! true;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    
//                                }
//                                else if (term2.lexeme.compare("float") == 0)
//                                {
//                                    if (category.compare("and_op") == 0)
//                                    {
//                                        bool result = ! true;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (category.compare("or_op") == 0)
//                                    {
//                                        bool result = ! true;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (category.compare("equal_op") == 0)
//                                    {
//                                        bool result = ! (term1_double == stod(term2.token));
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (category.compare("not_equal_op") == 0)
//                                    {
//                                        bool result = ! (term1_double != stod(term2.token));
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                }
//                                else if (term2.lexeme.compare("string") == 0)
//                                {
//                                    if (category.compare("and_op") == 0)
//                                    {
//                                        bool result = ! true;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (category.compare("or_op") == 0)
//                                    {
//                                        bool result = ! true;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (category.compare("equal_op") == 0)
//                                    {
//                                        bool result = ! false;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (category.compare("not_equal_op") == 0)
//                                    {
//                                        bool result = ! true;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                }
//                                else if (term2.lexeme.compare("var") == 0)
//                                {
//                                    if ( var_table_global.has(term2.token)) {
//                                        
//                                        // check defined
//                                        if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                        {
//                                            if (term2.type.compare("integer") == 0) // integer
//                                            {
//                                                if (category.compare("and_op") == 0)
//                                                {
//                                                    bool result = ! stoi(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                }
//                                                else if (category.compare("or_op") == 0)
//                                                {
//                                                    bool result = ! true;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                }
//                                                else if (category.compare("equal_op") == 0)
//                                                {
//                                                    bool result = ! false;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                }
//                                                else if (category.compare("not_equal_op") == 0)
//                                                {
//                                                    bool result = ! true;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                            }
//                                            else if (term2.type.compare("float") == 0)
//                                            {
//                                                if (category.compare("and_op") == 0)
//                                                {
//                                                    bool result = ! true;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("or_op") == 0)
//                                                {
//                                                    bool result = ! true;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("equal_op") == 0)
//                                                {
//                                                    bool result = ! (term1_double == stod(term2.value));
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("not_equal_op") == 0)
//                                                {
//                                                    bool result = ! (term1_double != stod(term2.value));
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                            }
//                                            else if (term2.type.compare("string") == 0)
//                                            {
//                                                if (category.compare("and_op") == 0)
//                                                {
//                                                    bool result = ! term1_double;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("or_op") == 0)
//                                                {
//                                                    bool result = ! true;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("equal_op") == 0)
//                                                {
//                                                    bool result = ! false;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("not_equal_op") == 0)
//                                                {
//                                                    bool result = ! true;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                            }
//                                        }
//                                        else
//                                        {
//                                            throw "ERROR. Variable " + term2.token + " undefined.";
//                                        }
//                                        
//                                    }
//                                    else
//                                    {
//                                        // throw unkown var error
//                                        throw "ERROR. Unkown variable: " + term2.token;
//                                    }
//                                }
//                            }
//                            else if (term1.type.compare("string") == 0)
//                            {
//                                string term1_string = term1.value;
//                                
//                                if (term2.lexeme.compare("integer") == 0)
//                                {
//                                    if (category.compare("and_op") == 0)
//                                    {
//                                        bool result = ! stoi(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (category.compare("or_op") == 0)
//                                    {
//                                        bool result = ! true;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (category.compare("equal_op") == 0)
//                                    {
//                                        bool result = ! false;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (category.compare("not_equal_op") == 0)
//                                    {
//                                        bool result = ! true;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    
//                                }
//                                else if (term2.lexeme.compare("float") == 0)
//                                {
//                                    if (category.compare("and_op") == 0)
//                                    {
//                                        bool result = ! true;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (category.compare("or_op") == 0)
//                                    {
//                                        bool result = ! true;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (category.compare("equal_op") == 0)
//                                    {
//                                        bool result = ! false;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (category.compare("not_equal_op") == 0)
//                                    {
//                                        bool result = ! true;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                }
//                                else if (term2.lexeme.compare("string") == 0)
//                                {
//                                    if (category.compare("and_op") == 0)
//                                    {
//                                        bool result = ! true;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (category.compare("or_op") == 0)
//                                    {
//                                        bool result = ! true;
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (category.compare("equal_op") == 0)
//                                    {
//                                        bool result = ! (term1.value.compare(term2.token) == 0);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (category.compare("not_equal_op") == 0)
//                                    {
//                                        bool result = ! (term1.value.compare(term2.token) != 0);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                }
//                                else if (term2.lexeme.compare("var") == 0)
//                                {
//                                    if ( var_table_global.has(term2.token)) {
//                                        
//                                        // check defined
//                                        if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                        {
//                                            if (term2.type.compare("integer") == 0) // integer
//                                            {
//                                                if (category.compare("and_op") == 0)
//                                                {
//                                                    bool result = ! stoi(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                }
//                                                else if (category.compare("or_op") == 0)
//                                                {
//                                                    bool result = ! true;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                }
//                                                else if (category.compare("equal_op") == 0)
//                                                {
//                                                    bool result = ! false;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                }
//                                                else if (category.compare("not_equal_op") == 0)
//                                                {
//                                                    bool result = ! true;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                            }
//                                            else if (term2.type.compare("float") == 0)
//                                            {
//                                                if (category.compare("and_op") == 0)
//                                                {
//                                                    bool result = ! true;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("or_op") == 0)
//                                                {
//                                                    bool result = ! true;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("equal_op") == 0)
//                                                {
//                                                    bool result = ! false;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("not_equal_op") == 0)
//                                                {
//                                                    bool result = ! true;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                            }
//                                            else if (term2.type.compare("string") == 0) // special
//                                            {
//                                                if (category.compare("and_op") == 0)
//                                                {
//                                                    bool result = ! true;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("or_op") == 0)
//                                                {
//                                                    bool result = ! true;
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("equal_op") == 0)
//                                                {
//                                                    bool result = ! (term1.value.compare(term2.token) == 0);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (category.compare("not_equal_op") == 0)
//                                                {
//                                                    bool result = ! (term1.value.compare(term2.token) != 0);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                            }
//                                        }
//                                        else
//                                        {
//                                            throw "ERROR. Variable " + term2.token + " undefined.";
//                                        }
//                                        
//                                    }
//                                    else
//                                    {
//                                        // throw unkown var error
//                                        throw "ERROR. Unkown variable: " + term2.token;
//                                    }
//                                }
//                            }
//                        }
//                        else
//                        {
//                            throw "ERROR. Variable " + term1.token + " undefined.";
//                        }
//                        
//                    }
//                    else
//                    {
//                        // throw unkown var error
//                        throw "ERROR. Unkown variable: " + term1.token;
//                        
//                    }
//                    
//                }
//                else if (term1.lexeme.compare("integer") == 0)
//                {
//                    int term1_int = stoi(term1.token);
//                    
//                    if (term2.lexeme.compare("integer") == 0)
//                    {
//                        
//                    }
//                    else if (term2.lexeme.compare("float") == 0)
//                    {
//                        
//                    }
//                    else if (term2.lexeme.compare("var") == 0) // var
//                    {
//                        // check if var is double or int
//                        
//                        //                            CHECK VAR in table
//                        //                            check var defined
//                        
//                        // check in table
//                        if ( var_table_global.has(term2.token)) {
//                            
//                            // check defined
//                            if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) ) { // check that var is defined
//                                
//                                if (term2.type.compare("integer") == 0)
//                                {
//                                    
//                                }
//                                else if (term2.type.compare("float") == 0)
//                                {
//                                    
//                                }
//                                else if (term2.type.compare("string") == 0)
//                                {
//                                    
//                                }
//                            }
//                            else
//                            {
//                                throw "ERROR. Variable " + term2.token + " undefined.";
//                            }
//                            
//                        }
//                        else
//                        {
//                            // throw unkown var error
//                            throw "ERROR. Unkown variable: " + term2.token;
//                            
//                        }
//                    }
//                }
//                else if (term1.lexeme.compare("float") == 0) // float
//                {
//                    double term1_double = stoi(term1.token);
//                    
//                    if (term2.lexeme.compare("integer") == 0)
//                    {
//                        
//                    }
//                    else if (term2.lexeme.compare("float") == 0)
//                    {
//                        
//                    }
//                    else if (term2.lexeme.compare("var") == 0)
//                    {
//                        // check if var is double or int
//                        
//                        // check if var in symbol table
//                        
//                        //                            CHECK VAR in table
//                        //                            check var defined
//                        
//                        // check in table
//                        if ( var_table_global.has(term2.token)) {
//                            
//                            // check defined
//                            if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                            {
//                                if (term2.type.compare("integer") == 0)
//                                {
//                                    
//                                }
//                                else if (term2.type.compare("float") == 0)
//                                {
//                                    
//                                }
//                                else if (term2.type.compare("string") == 0)
//                                {
//                                    
//                                }
//                            }
//                            else
//                            {
//                                throw "ERROR. Variable " + term2.token + " undefined.";
//                            }
//                        }
//                        else
//                        {
//                            // throw unkown var error
//                            throw "ERROR. Unkown variable: " + term2.token;
//                        }
//                    }
//                }
//                else if (term1.lexeme.compare("string") == 0)
//                {
//                    //                        string term1_string = term1.token;
//                    
//                    if (term2.lexeme.compare("string") == 0)
//                    {
//                        //                            term1.token = "1";
//                        //                            term1.lexeme = "bool";
//                        
//                    }
//                    else if (term2.lexeme.compare("integer") == 0)
//                    {
//                        
//                    }
//                    else if (term2.lexeme.compare("float") == 0)
//                    {
//                        //                            bool result = term1_double == stod(term2.token);
//                        
//                    }
//                    else if (term2.lexeme.compare("var") == 0)
//                    {
//                        // check if var is double or int
//                        
//                        // check if var in symbol table
//                        
//                        //                            CHECK VAR in table
//                        //                            check var defined
//                        
//                        // check in table
//                        if ( var_table_global.has(term2.token)) {
//                            
//                            // check defined
//                            if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                            {
//                                if (term2.type.compare("integer") == 0)
//                                {
//                                    
//                                }
//                                else if (term2.type.compare("float") == 0)
//                                {
//                                    
//                                }
//                                else if (term2.type.compare("string") == 0)
//                                {
//                                    if (term2.token.compare(term1.token) == 0)
//                                    {
//                                        
//                                    }
//                                    else
//                                    {
//                                        
//                                    }
//                                }
//                            }
//                            else
//                            {
//                                throw "ERROR. Variable " + term2.token + " undefined.";
//                            }
//                        }
//                        
//                    }
//                    
//                }
//                
//            }
//            
//        }
//        else if ( (category.compare("integer") == 0) ||
//                 (category.compare("single_quote") == 0) ||
//                 (category.compare("float") == 0) ||
//                 (category.compare("minus_op") == 0) ||
//                 (category.compare("var") == 0) ||
//                 (category.compare("p_left") == 0)  ) // tokens possible for first element in rel_value
//        {
//            term1 = parseRel_value(toPrint);
//            
//            // look ahead for possible  {<bool_op> <rel_value>}
//            token = tokenList[currToken_index][0];
//            category = tokenList[currToken_index][1];
//            
//            if ( (category.compare("and_op") == 0)     ||
//                (category.compare("or_op") == 0)      ||
//                (category.compare("equal_op") == 0)   ||
//                (category.compare("not_equal_op") == 0)  ) // bool op
//            {
//                append(toPrint, "Token Match. Expected possible bool_op and got a " + token + "\n");
//                currToken_index++;
//                term2 = parseRel_value(toPrint);
//                
//                // evalute
//                
//                if (category.compare("and_op") == 0)
//                {
//                    bool result = (stoi(term1.token) && stoi(term2.token));
//                    term1.token = to_string(result);
//                    term1.lexeme = "bool";
//                }
//                else if (category.compare("or_op") == 0)
//                {
//                    bool result = (stoi(term1.token) || stoi(term2.token));
//                    term1.token = to_string(result);
//                    term1.lexeme = "bool";
//                }
//                else if (category.compare("equal_op") == 0)
//                {
//                    bool result = (stoi(term1.token) == stoi(term2.token));
//                    term1.token = to_string(result);
//                    term1.lexeme = "bool";
//                }
//                else if (category.compare("not_equal_op") == 0)
//                {
//                    bool result = (stoi(term1.token) != stoi(term2.token));
//                    term1.token = to_string(result);
//                    term1.lexeme = "bool";
//                }
//            }
//        }
//        else
//        {
//            // error
//            append(toPrint, "Token ERROR. Expected first term in rel_expr token (integer, string, float, minus_op, var) and got a " + token + "\n");
//            
//            throw "Token ERROR. Expected first term in rel_expr token (integer, string, float, minus_op, var) and got a " + token + "\n";
//            //            return;
//        }
//        append(toPrint, "Exit Bool_exp\n");
//        
//        
//        
//    } catch (string error) {
//        error;
//    }
//    
//    return term1;
//}
//
//
//// rel_value
//Val parseRel_value(string & toPrint)
//{
//    Val term1;
//    Val term2;
//    Val result;
//    
//    result.lexeme = "bool";
//    
//    try {
//        append(toPrint, "Enter rel_value\n");
//        string token = tokenList[currToken_index][0];
//        string category = tokenList[currToken_index][1];
//        //    currToken_index++;
//        
//        
//        
//        if ( (category.compare("integer") == 0) ||
//            (category.compare("single_quote") == 0) ||
//            (category.compare("float") == 0) ||
//            (category.compare("minus_op") == 0) ||
//            (category.compare("var") == 0) ||
//            (category.compare("p_left") == 0) ) // tokens possible for first element in rel_expr
//        {
//            term1 = parseRel_expr(toPrint);
//            
//            // look ahead
//            //currToken_index++;
//            
//            if ((currToken_index + 1) == tokenList.size())
//            {
//                //currToken_index--;
//                return term1;
//                //                string error = "out of index in parseRel_value";
//                //                throw error;
//            }
//            
//            
//            // cout << "inside parseRel_value: " << toPrint << endl;
//            
//            token = tokenList[currToken_index][0];
//            category = tokenList[currToken_index][1];
//            //        currToken_index--;
//            
//            // check if next is rel_op
//            if ( (category.compare("equal_op") == 0) )
//            {
//                append(toPrint, "Token Match. Expected possible equal_op and got a " + token + "\n");
//                //parseRel_op(toPrint);
//                
//                // look ahead
//                currToken_index++;
//                token = tokenList[currToken_index][0];
//                category = tokenList[currToken_index][1];
//                
//                if ( (category.compare("integer") == 0) ||
//                    (category.compare("single_quote") == 0) ||
//                    (category.compare("float") == 0) ||
//                    (category.compare("minus_op") == 0) ||
//                    (category.compare("var") == 0) ||
//                    (category.compare("p_left") == 0)  ) // tokens possible for first element in rel_expr
//                {
//                    //        append(toPrint, "Token Match. Expected - and got a " + token + "\n");
//                    //currToken_index++;
//                    term2 = parseRel_expr(toPrint);
//                    
//                    //                    add var
//                    
//                    if (term1.lexeme.compare("integer") == 0)
//                    {
//                        int term1_int = stoi(term1.token);
//                        
//                        if (term2.lexeme.compare("integer") == 0)
//                        {
//                            bool result = term1_int == stoi(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                        }
//                        else if (term2.lexeme.compare("float") == 0)
//                        {
//                            bool result = term1_int == stod(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                        }
//                        else if (term2.lexeme.compare("var") == 0) // var
//                        {
//                            // check if var is double or int
//                            
//                            //                            CHECK VAR in table
//                            //                            check var defined
//                            
//                            // check in table
//                            if ( var_table_global.has(term2.token)) {
//                                
//                                // check defined
//                                if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) ) { // check that var is defined
//                                    
//                                    if (term2.type.compare("integer") == 0)
//                                    {
//                                        bool result = term1_int == stoi(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.type.compare("float") == 0)
//                                    {
//                                        bool result = term1_int == stod(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.type.compare("string") == 0)
//                                    {
//                                        // throw error
//                                        term1.token = "0";
//                                        term1.lexeme = "bool";
//                                    }
//                                }
//                                else
//                                {
//                                    throw "ERROR. Variable " + term2.token + " undefined.";
//                                }
//                                
//                            }
//                            else
//                            {
//                                // throw unkown var error
//                                throw "ERROR. Unkown variable: " + term2.token;
//                                
//                            }
//                        }
//                    }
//                    else if (term1.lexeme.compare("float") == 0) // float
//                    {
//                        double term1_double = stoi(term1.token);
//                        
//                        if (term2.lexeme.compare("integer") == 0)
//                        {
//                            bool result = term1_double == stoi(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                        }
//                        else if (term2.lexeme.compare("float") == 0)
//                        {
//                            bool result = term1_double == stod(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                        }
//                        else if (term2.lexeme.compare("var") == 0)
//                        {
//                            // check if var is double or int
//                            
//                            // check if var in symbol table
//                            
//                            //                            CHECK VAR in table
//                            //                            check var defined
//                            
//                            // check in table
//                            if ( var_table_global.has(term2.token)) {
//                                
//                                // check defined
//                                if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                {
//                                    if (term2.type.compare("integer") == 0)
//                                    {
//                                        bool result = term1_double == stoi(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.type.compare("float") == 0)
//                                    {
//                                        bool result = term1_double == stod(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.type.compare("string") == 0)
//                                    {
//                                        // throw error
//                                        term1.token = "0";
//                                        term1.lexeme = "bool";
//                                    }
//                                }
//                                else
//                                {
//                                    throw "ERROR. Variable " + term2.token + " undefined.";
//                                }
//                            }
//                            else
//                            {
//                                // throw unkown var error
//                                throw "ERROR. Unkown variable: " + term2.token;
//                            }
//                        }
//                    }
//                    else if (term1.lexeme.compare("string") == 0)
//                    {
//                        //                        string term1_string = term1.token;
//                        
//                        if (term2.lexeme.compare("string") == 0)
//                        {
//                            if (term2.token.compare(term1.token) == 0)
//                            {
//                                term1.token = "1";
//                                term1.lexeme = "bool";
//                            }
//                            else
//                            {
//                                term1.token = "0";
//                                term1.lexeme = "bool";
//                            }
//                        }
//                        else if (term2.lexeme.compare("integer") == 0)
//                        {
//                            term1.token = "0";
//                            term1.lexeme = "bool";
//                        }
//                        else if (term2.lexeme.compare("float") == 0)
//                        {
//                            //                            bool result = term1_double == stod(term2.token);
//                            term1.token = "0";          // to_string(result);
//                            term1.lexeme = "bool";
//                        }
//                        else if (term2.lexeme.compare("var") == 0)
//                        {
//                            // check if var is double or int
//                            
//                            // check if var in symbol table
//                            
//                            //                            CHECK VAR in table
//                            //                            check var defined
//                            
//                            // check in table
//                            if ( var_table_global.has(term2.token)) {
//                                
//                                // check defined
//                                if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                {
//                                    if (term2.type.compare("integer") == 0)
//                                    {
//                                        term1.token = "0";
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.type.compare("float") == 0)
//                                    {
//                                        term1.token = "0";
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.type.compare("string") == 0)
//                                    {
//                                        if (term2.token.compare(term1.token) == 0)
//                                        {
//                                            term1.token = "1";
//                                            term1.lexeme = "bool";
//                                        }
//                                        else
//                                        {
//                                            term1.token = "0";
//                                            term1.lexeme = "bool";
//                                        }
//                                    }
//                                }
//                                else
//                                {
//                                    throw "ERROR. Variable " + term2.token + " undefined.";
//                                }
//                            }
//                            
//                        }
//                        
//                    }
//                    else if (term1.lexeme.compare("var") == 0)
//                    {
//                        
//                        //                        unkown test
//                        
//                        if ( var_table_global.has(term1.token))
//                        {
//                            if ( ! (var_table_global.get(term1.token).value.compare("undefined") == 0) ) // check that var is defined
//                            {
//                                if (term1.type.compare("integer") == 0)
//                                {
//                                    int term1_int = stoi(term1.value); // VAL is int
//                                    
//                                    if (term2.lexeme.compare("integer") == 0)
//                                    {
//                                        bool result = term1_int == stoi(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (term2.lexeme.compare("float") == 0)
//                                    {
//                                        bool result = term1_int == stod(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (term2.lexeme.compare("var") == 0)
//                                    {
//                                        // check if var is in symbol table (else throw error
//                                        
//                                        // then check if var is double or int
//                                        
//                                        //                                    CHECK VAR in table
//                                        //                                    check var defined
//                                        
//                                        // check in table
//                                        if ( var_table_global.has(term2.token)) {
//                                            
//                                            // check defined
//                                            if ( ! (var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                            {
//                                                if (term2.type.compare("integer") == 0)
//                                                {
//                                                    bool result = term1_int == stoi(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (term2.type.compare("float") == 0)
//                                                {
//                                                    bool result = term1_int == stod(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (term2.type.compare("string") == 0)
//                                                {
//                                                    term1.token = "0";
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                            }
//                                            else
//                                            {
//                                                throw "ERROR. Variable " + term2.token + " undefined.";
//                                            }
//                                            
//                                        }
//                                        else
//                                        {
//                                            // throw unkown var error
//                                            throw "ERROR. Unkown variable: " + term2.token;
//                                            
//                                        }
//                                    }
//                                }                        // else if type double
//                                else if (term1.type.compare("float") == 0)
//                                {
//                                    double term1_double = stoi(term1.token);
//                                    
//                                    if (term2.lexeme.compare("integer") == 0)
//                                    {
//                                        bool result = term1_double == stoi(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (term2.lexeme.compare("float") == 0)
//                                    {
//                                        bool result = term1_double == stod(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (term2.lexeme.compare("var") == 0)
//                                    {
//                                        // check if var is double or int
//                                        
//                                        //                                    CHECK VAR in table
//                                        //                                    check var defined
//                                        
//                                        // check in table
//                                        if ( var_table_global.has(term2.token)) {
//                                            
//                                            // check defined
//                                            if ( ! (var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                            {
//                                                if (term2.type.compare("integer") == 0)
//                                                {
//                                                    bool result = term1_double == stoi(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (term2.type.compare("float") == 0)
//                                                {
//                                                    bool result = term1_double == stod(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (term2.type.compare("string") == 0)
//                                                {
//                                                    // throw error
//                                                    term1.token = "0";
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                            }
//                                            else
//                                            {
//                                                throw "ERROR. Variable " + term2.token + " undefined.";
//                                            }
//                                            
//                                        }
//                                        else
//                                        {
//                                            // throw unkown var error
//                                            throw "ERROR. Unkown variable: " + term2.token;
//                                            
//                                        }
//                                    }
//                                }
//                                else if (term1.type.compare("string") == 0) // string error?
//                                {
//                                    
//                                    if (term2.token.compare(term1.token) == 0)
//                                    {
//                                        term1.token = "1";
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else
//                                    {
//                                        term1.token = "0";
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                }
//                            }
//                            else
//                            {
//                                // throw error
//                                throw "ERROR. Variable " + term1.token + " undefined.";
//                            }
//                        }
//                        else
//                        {
//                            // throw unkown var error
//                            throw "ERROR. Unkown variable: " + term1.token;
//                        }
//                    }
//                }
//                else
//                {
//                    // error
//                    append(toPrint, "Token ERROR. Expected first term in rel_expr token (integer, string, float, minus_op, var) and got a " + token + "\n");
//                    string error = "Token ERROR. Expected first term in rel_expr token (integer, string, float, minus_op, var) and got a " + token + "\n";
//                    
//                    throw error;
//                    //                    return;
//                }
//            }
//        }
//        else
//        {
//            // error
//            append(toPrint, "Token ERROR. Expected first term in rel_expr token (integer, string, float, minus_op, var) and got a " + token + "\n");
//            
//            throw "Token ERROR. Expected first term in rel_expr token (integer, string, float, minus_op, var) and got a " + token + "\n";
//            //            return;
//        }
//        append(toPrint, "Exit rel_value\n");
//        
//    } catch (string error) {
//        error;
//    }
//    
//    return term1;
//    
//}
//
//// rel_expr
//Val parseRel_expr(string & toPrint)
//{
//    append(toPrint, "Enter rel_expr\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    //    currToken_index++;
//    
//    // cout << "inside parseRel_expr: " << toPrint << endl;
//    
//    Val term1;
//    Val term2;
//    
//    if ( (category.compare("integer") == 0) ||
//        (category.compare("single_quote") == 0) ||
//        (category.compare("float") == 0) ||
//        (category.compare("minus_op") == 0) ||
//        (category.compare("var") == 0) ||
//        (category.compare("p_left") == 0) ) // tokens possible for first element in term
//    {
//        //        append(toPrint, "Token Match. Expected - and got a " + token + "\n");
//        
//        term1 = parseTerm(toPrint); // assumed: currToken_index incremented
//        
//        // look ahead
//        // currToken_index++;
//        
//        token = tokenList[currToken_index][0];
//        category = tokenList[currToken_index][1];
//        //        currToken_index--;
//        
//        // check if next is rel_op
//        if ( (category.compare("less_than") == 0)  ||
//            (category.compare("greater_than") == 0)   ||
//            (category.compare("less_than_equal") == 0)   ||
//            (category.compare("greater_than_equal") == 0)   )
//        {
//            Val rel_op = parseRel_op(toPrint);
//            
//            // look ahead
//            //currToken_index++;
//            token = tokenList[currToken_index][0];
//            category = tokenList[currToken_index][1];
//            
//            if ( (category.compare("integer") == 0) ||
//                (category.compare("single_quote") == 0) ||
//                (category.compare("float") == 0) ||
//                (category.compare("minus_op") == 0) ||
//                (category.compare("var") == 0) ||
//                (category.compare("p_left") == 0) ) // tokens possible for first element in term
//            {
//                //        append(toPrint, "Token Match. Expected - and got a " + token + "\n");
//                // currToken_index++;
//                term2 = parseTerm(toPrint);
//                
//                
//                if (rel_op.token.compare("<") == 0)
//                {
//                    if (term1.lexeme.compare("integer") == 0)
//                    {
//                        int term1_int = stoi(term1.token);
//                        
//                        if (term2.lexeme.compare("integer") == 0)
//                        {
//                            bool result = term1_int < stoi(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                        }
//                        else if (term2.lexeme.compare("float") == 0)
//                        {
//                            bool result = term1_int < stod(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                        }
//                        else if (term2.lexeme.compare("var") == 0)
//                        {
//                            // check if var is double or int
//                            
//                            //                            CHECK VAR in table
//                            //                            check var defined
//                            
//                            // check in table
//                            if ( var_table_global.has(term2.token)) {
//                                
//                                // check defined
//                                if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) ) { // check that var is defined
//                                    
//                                    if (term2.type.compare("integer") == 0)
//                                    {
//                                        bool result = term1_int < stoi(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.type.compare("float") == 0)
//                                    {
//                                        bool result = term1_int < stod(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.type.compare("string") == 0)
//                                    {
//                                        // throw error
//                                    }
//                                }
//                                else
//                                {
//                                    throw "ERROR. Variable " + term2.token + " undefined.";
//                                }
//                                
//                            }
//                            else
//                            {
//                                // throw unkown var error
//                                throw "ERROR. Unkown variable: " + term2.token;
//                                
//                            }
//                        }
//                    }
//                    else if (term1.lexeme.compare("float") == 0)
//                    {
//                        double term1_double = stoi(term1.token);
//                        
//                        if (term2.lexeme.compare("integer") == 0)
//                        {
//                            bool result = term1_double < stoi(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                        }
//                        else if (term2.lexeme.compare("float") == 0)
//                        {
//                            bool result = term1_double < stod(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                        }
//                        else if (term2.lexeme.compare("var") == 0)
//                        {
//                            // check if var is double or int
//                            
//                            // check if var in symbol table
//                            
//                            //                            CHECK VAR in table
//                            //                            check var defined
//                            
//                            // check in table
//                            if ( var_table_global.has(term2.token)) {
//                                
//                                // check defined
//                                if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                {
//                                    if (term2.type.compare("integer") == 0)
//                                    {
//                                        bool result = term1_double < stoi(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.type.compare("float") == 0)
//                                    {
//                                        bool result = term1_double < stod(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.type.compare("string") == 0)
//                                    {
//                                        // throw error
//                                    }
//                                }
//                                else
//                                {
//                                    throw "ERROR. Variable " + term2.token + " undefined.";
//                                }
//                            }
//                            else
//                            {
//                                // throw unkown var error
//                                throw "ERROR. Unkown variable: " + term2.token;
//                            }
//                        }
//                    }
//                    else if (term1.lexeme.compare("var") == 0)
//                    {
//                        
//                        //                        unkown test
//                        
//                        if ( var_table_global.has(term1.token))
//                        {
//                            if ( ! (var_table_global.get(term1.token).value.compare("undefined") == 0) ) // check that var is defined
//                            {
//                                if (term1.type.compare("integer") == 0)
//                                {
//                                    int term1_int = stoi(term1.value);
//                                    
//                                    if (term2.lexeme.compare("integer") == 0)
//                                    {
//                                        bool result = term1_int < stoi(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.lexeme.compare("float") == 0)
//                                    {
//                                        bool result = term1_int < stod(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.lexeme.compare("var") == 0)
//                                    {
//                                        // check if var is in symbol table (else throw error
//                                        
//                                        // then check if var is double or int
//                                        
//                                        //                                    CHECK VAR in table
//                                        //                                    check var defined
//                                        
//                                        // check in table
//                                        if ( var_table_global.has(term2.token)) {
//                                            
//                                            // check defined
//                                            if ( ! (var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                            {
//                                                if (term2.type.compare("integer") == 0)
//                                                {
//                                                    bool result = term1_int < stoi(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                }
//                                                else if (term2.type.compare("float") == 0)
//                                                {
//                                                    bool result = term1_int < stod(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                }
//                                                else if (term2.type.compare("string") == 0)
//                                                {
//                                                    // throw error
//                                                }
//                                            }
//                                            else
//                                            {
//                                                throw "ERROR. Variable " + term2.token + " undefined.";
//                                            }
//                                            
//                                        }
//                                        else
//                                        {
//                                            // throw unkown var error
//                                            throw "ERROR. Unkown variable: " + term2.token;
//                                            
//                                        }
//                                    }
//                                }                        // else if type double
//                                else if (term1.type.compare("float") == 0)
//                                {
//                                    double term1_double = stoi(term1.token);
//                                    
//                                    if (term2.lexeme.compare("integer") == 0)
//                                    {
//                                        bool result = term1_double < stoi(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.lexeme.compare("float") == 0)
//                                    {
//                                        bool result = term1_double < stod(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.lexeme.compare("var") == 0)
//                                    {
//                                        // check if var is double or int
//                                        
//                                        //                                    CHECK VAR in table
//                                        //                                    check var defined
//                                        
//                                        // check in table
//                                        if ( var_table_global.has(term2.token)) {
//                                            
//                                            // check defined
//                                            if ( ! (var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                            {
//                                                if (term2.type.compare("integer") == 0)
//                                                {
//                                                    bool result = term1_double < stoi(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                }
//                                                else if (term2.type.compare("float") == 0)
//                                                {
//                                                    bool result = term1_double < stod(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                }
//                                                else if (term2.type.compare("string") == 0)
//                                                {
//                                                    // throw error
//                                                }
//                                            }
//                                            else
//                                            {
//                                                throw "ERROR. Variable " + term2.token + " undefined.";
//                                            }
//                                            
//                                        }
//                                        else
//                                        {
//                                            // throw unkown var error
//                                            throw "ERROR. Unkown variable: " + term2.token;
//                                            
//                                        }
//                                        
//                                        
//                                    }
//                                }
//                                else if (term1.type.compare("string") == 0) // string error?
//                                {
//                                    
//                                }
//                            }
//                            else
//                            {
//                                // throw error
//                                throw "ERROR. Variable " + term1.token + " undefined.";
//                            }
//                        }
//                        else
//                        {
//                            // throw unkown var error
//                            throw "ERROR. Unkown variable: " + term1.token;
//                        }
//                        
//                        
//                    }
//                }
//                else if (rel_op.token.compare(">") == 0)
//                {
//                    if (term1.lexeme.compare("integer") == 0)
//                    {
//                        int term1_int = stoi(term1.token);
//                        
//                        if (term2.lexeme.compare("integer") == 0)
//                        {
//                            bool result = term1_int > stoi(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                        }
//                        else if (term2.lexeme.compare("float") == 0)
//                        {
//                            bool result = term1_int > stod(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                        }
//                        else if (term2.lexeme.compare("var") == 0)
//                        {
//                            // check if var is double or int
//                            
//                            //                            CHECK VAR in table
//                            //                            check var defined
//                            
//                            // check in table
//                            if ( var_table_global.has(term2.token)) {
//                                
//                                // check defined
//                                if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) ) { // check that var is defined
//                                    
//                                    if (term2.type.compare("integer") == 0)
//                                    {
//                                        bool result = term1_int > stoi(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.type.compare("float") == 0)
//                                    {
//                                        bool result = term1_int > stod(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.type.compare("string") == 0)
//                                    {
//                                        // throw error
//                                    }
//                                }
//                                else
//                                {
//                                    throw "ERROR. Variable " + term2.token + " undefined.";
//                                }
//                                
//                            }
//                            else
//                            {
//                                // throw unkown var error
//                                throw "ERROR. Unkown variable: " + term2.token;
//                                
//                            }
//                            
//                            
//                        }
//                    }
//                    else if (term1.lexeme.compare("float") == 0)
//                    {
//                        double term1_double = stoi(term1.token);
//                        
//                        if (term2.lexeme.compare("integer") == 0)
//                        {
//                            bool result = term1_double > stoi(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                        }
//                        else if (term2.lexeme.compare("float") == 0)
//                        {
//                            bool result = term1_double > stod(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                        }
//                        else if (term2.lexeme.compare("var") == 0)
//                        {
//                            // check if var is double or int
//                            
//                            // check if var in symbol table
//                            
//                            //                            CHECK VAR in table
//                            //                            check var defined
//                            
//                            // check in table
//                            if ( var_table_global.has(term2.token)) {
//                                
//                                // check defined
//                                if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                {
//                                    if (term2.type.compare("integer") == 0)
//                                    {
//                                        bool result = term1_double > stoi(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.type.compare("float") == 0)
//                                    {
//                                        bool result = term1_double > stod(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.type.compare("string") == 0)
//                                    {
//                                        // throw error
//                                    }
//                                }
//                                else
//                                {
//                                    throw "ERROR. Variable " + term2.token + " undefined.";
//                                }
//                                
//                            }
//                            else
//                            {
//                                // throw unkown var error
//                                throw "ERROR. Unkown variable: " + term2.token;
//                                
//                            }
//                            
//                            
//                        }
//                    }
//                    else if (term1.lexeme.compare("var") == 0)
//                    {
//                        //                        unkown test
//                        
//                        if ( var_table_global.has(term1.token))
//                        {
//                            if ( ! (var_table_global.get(term1.token).value.compare("undefined") == 0) ) // check that var is defined
//                            {
//                                if (term1.type.compare("integer") == 0)
//                                {
//                                    int term1_int = stoi(term1.value);
//                                    
//                                    if (term2.lexeme.compare("integer") == 0)
//                                    {
//                                        bool result = term1_int > stoi(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.lexeme.compare("float") == 0)
//                                    {
//                                        bool result = term1_int > stod(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.lexeme.compare("var") == 0)
//                                    {
//                                        // check if var is in symbol table (else throw error
//                                        
//                                        // then check if var is double or int
//                                        
//                                        //                                    CHECK VAR in table
//                                        //                                    check var definedb
//                                        
//                                        // check in table
//                                        if ( var_table_global.has(term2.token)) {
//                                            
//                                            // check defined
//                                            if ( ! (var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                            {
//                                                if (term2.type.compare("integer") == 0)
//                                                {
//                                                    bool result = term1_int > stoi(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                }
//                                                else if (term2.type.compare("float") == 0)
//                                                {
//                                                    bool result = term1_int > stod(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                }
//                                                else if (term2.type.compare("string") == 0)
//                                                {
//                                                    // throw error
//                                                }
//                                            }
//                                            else
//                                            {
//                                                throw "ERROR. Variable " + term2.token + " undefined.";
//                                            }
//                                        }
//                                        else
//                                        {
//                                            // throw unkown var error
//                                            throw "ERROR. Unkown variable: " + term2.token;
//                                            
//                                        }
//                                        
//                                        
//                                    }
//                                }                        // else if type double
//                                else if (term1.type.compare("float") == 0)
//                                {
//                                    double term1_double = stoi(term1.token);
//                                    
//                                    if (term2.lexeme.compare("integer") == 0)
//                                    {
//                                        bool result = term1_double > stoi(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.lexeme.compare("float") == 0)
//                                    {
//                                        bool result = term1_double > stod(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.lexeme.compare("var") == 0)
//                                    {
//                                        // check if var is double or int
//                                        
//                                        //                                    CHECK VAR in table
//                                        //                                    check var defined
//                                        
//                                        // check in table
//                                        if ( var_table_global.has(term2.token)) {
//                                            
//                                            // check defined
//                                            if ( ! (var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                            {
//                                                if (term2.type.compare("integer") == 0)
//                                                {
//                                                    bool result = term1_double > stoi(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                }
//                                                else if (term2.type.compare("float") == 0)
//                                                {
//                                                    bool result = term1_double > stod(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                }
//                                                else if (term2.type.compare("string") == 0)
//                                                {
//                                                    // throw error
//                                                }
//                                            }
//                                            else
//                                            {
//                                                throw "ERROR. Variable " + term2.token + " undefined.";
//                                            }
//                                            
//                                        }
//                                        else
//                                        {
//                                            // throw unkown var error
//                                            throw "ERROR. Unkown variable: " + term2.token;
//                                            
//                                        }
//                                        
//                                        
//                                    }
//                                }
//                                else if (term1.type.compare("string") == 0) // string error?
//                                {
//                                    
//                                }
//                            }
//                            else
//                            {
//                                // throw error
//                                throw "ERROR. Variable " + term1.token + " undefined.";
//                            }
//                        }
//                        else
//                        {
//                            // throw unkown var error
//                            throw "ERROR. Unkown variable: " + term1.token;
//                        }
//                        
//                        
//                    }
//                }
//                else if (rel_op.token.compare("<=") == 0)
//                {
//                    //                    if (term2.lexeme.compare("integer") == 0)
//                    //                    {
//                    //                        bool result = stoi(term1.token) >= stoi(term2.token);
//                    //                        term1.token = to_string(result);
//                    //                        term1.lexeme = "bool";
//                    //                    }
//                    //                    else if (term2.lexeme.compare("float") == 0)
//                    //                    {
//                    //                        bool result = stod(term1.token) >= stod(term2.token);
//                    //                        term1.token = to_string(result);
//                    //                    }
//                    if (term1.lexeme.compare("integer") == 0)
//                    {
//                        int term1_int = stoi(term1.token);
//                        
//                        if (term2.lexeme.compare("integer") == 0)
//                        {
//                            bool result = term1_int <= stoi(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                        }
//                        else if (term2.lexeme.compare("float") == 0)
//                        {
//                            bool result = term1_int <= stod(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                        }
//                        else if (term2.lexeme.compare("var") == 0)
//                        {
//                            // check if var is double or int
//                            
//                            //                            CHECK VAR in table
//                            //                            check var defined
//                            
//                            // check in table
//                            if ( var_table_global.has(term2.token)) {
//                                
//                                if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                { // check that var is defined
//                                    
//                                    if (term2.type.compare("integer") == 0)
//                                    {
//                                        bool result = term1_int <= stoi(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.type.compare("float") == 0)
//                                    {
//                                        bool result = term1_int <= stod(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.type.compare("string") == 0)
//                                    {
//                                        // throw error
//                                    }
//                                }
//                                else
//                                {
//                                    throw "ERROR. Variable " + term2.token + " undefined.";
//                                }
//                                
//                            }
//                            else
//                            {
//                                // throw unkown var error
//                                throw "ERROR. Unkown variable: " + term2.token;
//                                
//                            }
//                            
//                            
//                        }
//                    }
//                    else if (term1.lexeme.compare("float") == 0)
//                    {
//                        double term1_double = stoi(term1.token);
//                        
//                        if (term2.lexeme.compare("integer") == 0)
//                        {
//                            bool result = term1_double <= stoi(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                        }
//                        else if (term2.lexeme.compare("float") == 0)
//                        {
//                            bool result = term1_double <= stod(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                        }
//                        else if (term2.lexeme.compare("var") == 0)
//                        {
//                            // check if var is double or int
//                            
//                            // check if var in symbol table
//                            
//                            //                            CHECK VAR in table
//                            //                            check var defined
//                            
//                            // check in table
//                            if ( var_table_global.has(term2.token)) {
//                                
//                                // check defined
//                                if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                {
//                                    if (term2.type.compare("integer") == 0)
//                                    {
//                                        bool result = term1_double <= stoi(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.type.compare("float") == 0)
//                                    {
//                                        bool result = term1_double <= stod(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                    }
//                                    else if (term2.type.compare("string") == 0)
//                                    {
//                                        // throw error
//                                    }
//                                }
//                                else
//                                {
//                                    throw "ERROR. Variable " + term2.token + " undefined.";
//                                }
//                                
//                            }
//                            else
//                            {
//                                // throw unkown var error
//                                throw "ERROR. Unkown variable: " + term2.token;
//                                
//                            }
//                            
//                        }
//                    }
//                    else if (term1.lexeme.compare("var") == 0)
//                    {
//                        
//                        //                        unkown test
//                        
//                        if ( var_table_global.has(term1.token))
//                        {
//                            if ( ! (var_table_global.get(term1.token).value.compare("undefined") == 0) ) // check that var is defined
//                            {
//                                if (term1.type.compare("integer") == 0)
//                                {
//                                    int term1_int = stoi(term1.value);
//                                    
//                                    if (term2.lexeme.compare("integer") == 0)
//                                    {
//                                        bool result = term1_int <= stoi(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (term2.lexeme.compare("float") == 0)
//                                    {
//                                        bool result = term1_int <= stod(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (term2.lexeme.compare("var") == 0)
//                                    {
//                                        // check if var is in symbol table (else throw error
//                                        
//                                        // then check if var is double or int
//                                        
//                                        //                                    CHECK VAR in table
//                                        //                                    check var defined
//                                        
//                                        // check in table
//                                        if ( var_table_global.has(term2.token)) {
//                                            
//                                            // check defined
//                                            if ( ! (var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                            {
//                                                if (term2.type.compare("integer") == 0)
//                                                {
//                                                    bool result = term1_int <= stoi(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (term2.type.compare("float") == 0)
//                                                {
//                                                    bool result = term1_int <= stod(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (term2.type.compare("string") == 0)
//                                                {
//                                                    // throw error
//                                                }
//                                            }
//                                            else
//                                            {
//                                                throw "ERROR. Variable " + term2.token + " undefined.";
//                                            }
//                                            
//                                        }
//                                        else
//                                        {
//                                            // throw unkown var error
//                                            throw "ERROR. Unkown variable: " + term2.token;
//                                            
//                                        }
//                                        
//                                        
//                                    }
//                                }                        // else if type double
//                                else if (term1.type.compare("float") == 0)
//                                {
//                                    double term1_double = stoi(term1.token);
//                                    
//                                    if (term2.lexeme.compare("integer") == 0)
//                                    {
//                                        bool result = term1_double <= stoi(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (term2.lexeme.compare("float") == 0)
//                                    {
//                                        bool result = term1_double <= stod(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (term2.lexeme.compare("var") == 0)
//                                    {
//                                        // check if var is double or int
//                                        
//                                        //                                    CHECK VAR in table
//                                        //                                    check var defined
//                                        
//                                        // check in table
//                                        if ( var_table_global.has(term2.token)) {
//                                            
//                                            // check defined
//                                            if ( ! (var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                            {
//                                                if (term2.type.compare("integer") == 0)
//                                                {
//                                                    bool result = term1_double <= stoi(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (term2.type.compare("float") == 0)
//                                                {
//                                                    bool result = term1_double <= stod(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (term2.type.compare("string") == 0)
//                                                {
//                                                    // throw error
//                                                }
//                                            }
//                                            else
//                                            {
//                                                throw "ERROR. Variable " + term2.token + " undefined.";
//                                            }
//                                            
//                                        }
//                                        else
//                                        {
//                                            // throw unkown var error
//                                            throw "ERROR. Unkown variable: " + term2.token;
//                                            
//                                        }
//                                        
//                                        
//                                    }
//                                }
//                                else if (term1.type.compare("string") == 0) // string error?
//                                {
//                                    
//                                }
//                            }
//                            else
//                            {
//                                // throw error
//                                throw "ERROR. Variable " + term1.token + " undefined.";
//                            }
//                            
//                        }
//                        else
//                        {
//                            // throw unkown var error
//                            throw "ERROR. Unkown variable: " + term1.token;
//                        }
//                        
//                    }
//                }
//                else if (rel_op.token.compare(">=") == 0)
//                {
//                    if (term1.lexeme.compare("integer") == 0)
//                    {
//                        int term1_int = stoi(term1.token);
//                        
//                        if (term2.lexeme.compare("integer") == 0)
//                        {
//                            bool result = term1_int >= stoi(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                            term1.type = "undefined";
//                            term1.value = "undefined";
//                        }
//                        else if (term2.lexeme.compare("float") == 0)
//                        {
//                            bool result = term1_int >= stod(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                            term1.type = "undefined";
//                            term1.value = "undefined";
//                        }
//                        else if (term2.lexeme.compare("var") == 0)
//                        {
//                            // check if var is double or int
//                            
//                            //                            CHECK VAR in table
//                            //                            check var defined
//                            
//                            // check in table
//                            if ( var_table_global.has(term2.token)) {
//                                
//                                // check defined
//                                if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) ) { // check that var is defined
//                                    
//                                    if (term2.type.compare("integer") == 0)
//                                    {
//                                        bool result = term1_int >= stoi(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (term2.type.compare("float") == 0)
//                                    {
//                                        bool result = term1_int >= stod(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (term2.type.compare("string") == 0)
//                                    {
//                                        // throw error
//                                    }
//                                }
//                                else
//                                {
//                                    throw "ERROR. Variable " + term2.token + " undefined.";
//                                }
//                                
//                            }
//                            else
//                            {
//                                // throw unkown var error
//                                throw "ERROR. Unkown variable: " + term2.token;
//                                
//                            }
//                            
//                            
//                        }
//                    }
//                    else if (term1.lexeme.compare("float") == 0)
//                    {
//                        double term1_double = stoi(term1.token);
//                        
//                        if (term2.lexeme.compare("integer") == 0)
//                        {
//                            bool result = term1_double >= stoi(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                            term1.type = "undefined";
//                            term1.value = "undefined";
//                        }
//                        else if (term2.lexeme.compare("float") == 0)
//                        {
//                            bool result = term1_double >= stod(term2.token);
//                            term1.token = to_string(result);
//                            term1.lexeme = "bool";
//                            term1.type = "undefined";
//                            term1.value = "undefined";
//                        }
//                        else if (term2.lexeme.compare("var") == 0)
//                        {
//                            // check if var is double or int
//                            
//                            // check if var in symbol table
//                            
//                            //                            CHECK VAR in table
//                            //                            check var defined
//                            
//                            // check in table
//                            if ( var_table_global.has(term2.token)) {
//                                
//                                // check defined
//                                if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                {
//                                    if (term2.type.compare("integer") == 0)
//                                    {
//                                        bool result = term1_double >= stoi(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (term2.type.compare("float") == 0)
//                                    {
//                                        bool result = term1_double >= stod(term2.value);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (term2.type.compare("string") == 0)
//                                    {
//                                        // throw error
//                                    }
//                                }
//                                else
//                                {
//                                    throw "ERROR. Variable " + term2.token + " undefined.";
//                                }
//                                
//                            }
//                            else
//                            {
//                                // throw unkown var error
//                                throw "ERROR. Unkown variable: " + term2.token;
//                                
//                            }
//                            
//                        }
//                    }
//                    else if (term1.lexeme.compare("var") == 0)
//                    {
//                        
//                        //                        unkown test
//                        
//                        if ( var_table_global.has(term1.token))
//                        {
//                            if ( ! (var_table_global.get(term1.token).value.compare("undefined") == 0) ) // check that var is defined
//                            {
//                                if (term1.type.compare("integer") == 0)
//                                {
//                                    int term1_int = stoi(term1.value);
//                                    
//                                    if (term2.lexeme.compare("integer") == 0)
//                                    {
//                                        bool result = term1_int >= stoi(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (term2.lexeme.compare("float") == 0)
//                                    {
//                                        bool result = term1_int >= stod(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (term2.lexeme.compare("var") == 0)
//                                    {
//                                        // check if var is in symbol table (else throw error
//                                        
//                                        // then check if var is double or int
//                                        
//                                        //                                    CHECK VAR in table
//                                        //                                    check var defined
//                                        
//                                        // check in table
//                                        if ( var_table_global.has(term2.token)) {
//                                            
//                                            if ( ! (var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                            {
//                                                if (term2.type.compare("integer") == 0)
//                                                {
//                                                    bool result = term1_int >= stoi(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (term2.type.compare("float") == 0)
//                                                {
//                                                    bool result = term1_int >= stod(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (term2.type.compare("string") == 0)
//                                                {
//                                                    // throw error
//                                                }
//                                            }
//                                            else
//                                            {
//                                                throw "ERROR. Variable " + term2.token + " undefined.";
//                                            }
//                                            
//                                        }
//                                        else
//                                        {
//                                            // throw unkown var error
//                                            throw "ERROR. Unkown variable: " + term2.token;
//                                            
//                                        }
//                                        
//                                        
//                                    }
//                                }                        // else if type double
//                                else if (term1.type.compare("float") == 0)
//                                {
//                                    double term1_double = stoi(term1.token);
//                                    
//                                    if (term2.lexeme.compare("integer") == 0)
//                                    {
//                                        bool result = term1_double >= stoi(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (term2.lexeme.compare("float") == 0)
//                                    {
//                                        bool result = term1_double >= stod(term2.token);
//                                        term1.token = to_string(result);
//                                        term1.lexeme = "bool";
//                                        term1.type = "undefined";
//                                        term1.value = "undefined";
//                                    }
//                                    else if (term2.lexeme.compare("var") == 0)
//                                    {
//                                        // check if var is double or int
//                                        
//                                        //                                    CHECK VAR in table
//                                        //                                    check var defined
//                                        
//                                        // check in table
//                                        if ( var_table_global.has(term2.token)) {
//                                            
//                                            if ( ! (var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                            {
//                                                if (term2.type.compare("integer") == 0)
//                                                {
//                                                    bool result = term1_double >= stoi(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (term2.type.compare("float") == 0)
//                                                {
//                                                    bool result = term1_double >= stod(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.lexeme = "bool";
//                                                    term1.type = "undefined";
//                                                    term1.value = "undefined";
//                                                }
//                                                else if (term2.type.compare("string") == 0)
//                                                {
//                                                    // throw error
//                                                }
//                                            }
//                                            else
//                                            {
//                                                throw "ERROR. Variable " + term2.token + " undefined.";
//                                            }
//                                            
//                                        }
//                                        else
//                                        {
//                                            // throw unkown var error
//                                            throw "ERROR. Unkown variable: " + term2.token;
//                                            
//                                        }
//                                        
//                                        
//                                    }
//                                }
//                                else if (term1.type.compare("string") == 0) // string error?
//                                {
//                                    
//                                }
//                            }
//                            else
//                            {
//                                // throw error
//                                throw "ERROR. Variable " + term1.token + " undefined.";
//                            }
//                        }
//                        else
//                        {
//                            // throw unkown var error
//                            throw "ERROR. Unkown variable: " + term1.token;
//                        }
//                    }
//                }
//            }
//            else
//            {
//                // error
//                append(toPrint, "Token ERROR. Expected term token (integer, string, float, minus_op, var) and got a " + token + "\n");
//                //                return;
//            }
//        }
//    }
//    else
//    {
//        // error
//        append(toPrint, "Token ERROR. Expected term token (integer, string, float, minus_op, var) and got a " + token + "\n");
//        //        return;
//    }
//    append(toPrint, "Exit rel_expr\n");
//    
//    return term1;
//}
//
//// term
//Val parseTerm(string & toPrint)
//{
//    append(toPrint, "Enter Term\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    
//    Val math;
//    
//    if ( (category.compare("var") == 0) )
//    {
//        
//        
//        //currToken_index++;
//        //token = tokenList[currToken_index][0];
//        //category = tokenList[currToken_index][1];
//        
//        // <function_invoc>
//        // <var>(
//        if ( (tokenList[currToken_index+1][1].compare("p_left") == 0) )
//        {
//            parseFunction_invoc(toPrint);
//        }
//        // <object>
//        // <var> = new
//        else if ( (tokenList[currToken_index+1][1].compare("assign_op") == 0) )
//        {
//            //currToken_index++;
//            //token = tokenList[currToken_index][0];
//            //category = tokenList[currToken_index][1];
//            
//            if ( (tokenList[currToken_index+2][1].compare("new_key") == 0) ) {
//                parseObject(toPrint);
//            }
//            else
//            {
//                // error
//                append(toPrint, "Token ERROR. Expected new_key and got a " + token + "\n");
//                //                return;
//            }
//            
//        }
//        
//        // call bool expr
//        
//        // call math expr
//        
//        // <math>
//        // <var>
//        // - <var>
//        else
//        {
//            // error
//            math = parseMath(toPrint);
//            
//        }
//    }
//    else if ((category.compare("integer") == 0) ||
//             (category.compare("single_quote") == 0) ||
//             (category.compare("float") == 0) ||
//             (category.compare("minus_op") == 0) ||
//             (category.compare("p_left") == 0) /* ||
//                                                (category.compare("var") == 0) */ ) // math pow check
//    {
//        math = parseMath(toPrint);
//    }
//    else
//    {
//        // error
//        append(toPrint, "Token ERROR. Expected <math>, <function_invoc>, or <object> and got a " + token + "\n");
//        //        return;
//    }
//    
//    append(toPrint, "Exit Term\n");
//    
//    return math;
//}
//
//// function_invoc
//void parseFunction_invoc(string & toPrint)  //
//{
//    append(toPrint, "Enter Function_invoc\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    
//    if (category.compare("var") == 0)
//    {
//        //        append(toPrint, "Token Match. Expected - and got a " + token + "\n");
//        parseVar(toPrint);
//        
//        token = tokenList[currToken_index][0];
//        category = tokenList[currToken_index][1];
//        
//        // check for left perens
//        if (category.compare("p_left") == 0)
//        {
//            append(toPrint, "Token Match. Expected p_left and got a " + token + "\n");
//            
//            // parse params
//            currToken_index++;
//            token = tokenList[currToken_index][0];
//            category = tokenList[currToken_index][1];
//            
//            
//            if (category.compare("p_right") == 0) // allow for empty paramater list
//            {
//                append(toPrint, "Token Match. Expected possible p_right and got a " + token + "\n");
//                currToken_index++;
//            }
//            else
//            {
//                parseParams(toPrint);
//                
//                token = tokenList[currToken_index][0];
//                category = tokenList[currToken_index][1];
//                
//                // check for right perens
//                if (category.compare("p_right") == 0)
//                {
//                    append(toPrint, "Token Match. Expected p_right and got a " + token + "\n");
//                    currToken_index++;
//                }
//                else
//                {
//                    append(toPrint, "Token ERROR. Expected p_right and got a " + token + "\n");
//                    return;
//                }
//            }
//            
//        }
//        else
//        {
//            append(toPrint, "Token ERROR. Expected p_left and got a " + token + "\n");
//            return;
//        }
//    }
//    else
//    {
//        append(toPrint, "Token Error. Expected var and got a " + token + "\n");
//        return;
//    }
//    append(toPrint, "Exit Function_invoc\n");
//}
//
//// math_value
//Val parseMath(string & toPrint)
//{
//    append(toPrint, "Enter Math\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    
//    Val term1;
//    Val term2;
//    
//    if ((category.compare("integer") == 0) ||
//        (category.compare("single_quote") == 0) ||
//        (category.compare("float") == 0) ||
//        (category.compare("minus_op") == 0) ||
//        (category.compare("var") == 0) ||
//        (category.compare("p_left") == 0) ) // math pow check
//    {
//        // append(toPrint, "Token Match. Expected math_pow (integer, string, float, minus_up, or var) and got a " + token + "\n");
//        term1 = parseMath_value(toPrint);
//        
//        // look ahead
//        //currToken_index++;
//        token = tokenList[currToken_index][0];
//        category = tokenList[currToken_index][1];
//        if ((category.compare("plus_op") == 0) || (category.compare("minus_op") == 0))
//        {
//            append(toPrint, "Token Match. Expected plus_op or minus_op and got a " + token + "\n");
//            currToken_index++;
//            term2 = parseMath(toPrint);
//            
//            if (category.compare("plus_op") == 0)
//            {
//                if (term1.lexeme.compare("integer") == 0)
//                {
//                    int term1_int = stoi(term1.token);
//                    
//                    if (term2.lexeme.compare("integer") == 0)
//                    {
//                        int result = term1_int + stoi(term2.token);
//                        term1.token = to_string(result);
//                        term1.value = "undefined";
//                        term1.type = "undefined";
//                        term1.lexeme = "integer";
//                    }
//                    else if (term2.lexeme.compare("float") == 0)
//                    {
//                        double result = term1_int + stod(term2.token);
//                        term1.token = to_string(result);
//                        term1.value = "undefined";
//                        term1.type = "undefined";
//                        term1.lexeme = "float";
//                    }
//                    else if (term2.lexeme.compare("var") == 0)
//                    {
//                        // check if var is double or int
//                        
//                        // check if var in symbol table
//                        
//                        //                        CHECK VAR in table
//                        //                        check var defined
//                        
//                        // check in table
//                        if ( var_table_global.has(term2.token)) {
//                            
//                            if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0)) {
//                                
//                                if (term2.type.compare("integer") == 0)
//                                {
//                                    int result = term1_int + stoi(term2.value);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "integer";
//                                }
//                                else if (term2.type.compare("float") == 0)
//                                {
//                                    double result = term1_int + stod(term2.value);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.type.compare("string") == 0)
//                                {
//                                    // throw error
//                                }
//                            }
//                            else
//                            {
//                                throw "ERROR. Variable " + term2.token + " undefined.";
//                            }
//                            
//                        }
//                        else
//                        {
//                            // throw unkown var error
//                            throw "ERROR. Unkown variable: " + term2.token;
//                            
//                        }
//                        
//                        
//                    }
//                }
//                else if (term1.lexeme.compare("float") == 0)
//                {
//                    double term1_double = stoi(term1.token);
//                    
//                    if (term2.lexeme.compare("integer") == 0)
//                    {
//                        double result = term1_double + stoi(term2.token);
//                        term1.token = to_string(result);
//                        term1.value = "undefined";
//                        term1.type = "undefined";
//                        term1.lexeme = "float";
//                    }
//                    else if (term2.lexeme.compare("float") == 0)
//                    {
//                        double result = term1_double + stod(term2.token);
//                        term1.token = to_string(result);
//                        term1.value = "undefined";
//                        term1.type = "undefined";
//                        term1.lexeme = "float";
//                    }
//                    else if (term2.lexeme.compare("var") == 0)
//                    {
//                        // check if var is double or int
//                        
//                        // check if var in symbol table
//                        
//                        //                        CHECK VAR in table
//                        //                        check var defined
//                        
//                        // check in table
//                        if ( var_table_global.has(term2.token)) {
//                            
//                            // check defined
//                            if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                            {
//                                if (term2.type.compare("integer") == 0)
//                                {
//                                    double result = term1_double + stoi(term2.value);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.type.compare("float") == 0)
//                                {
//                                    double result = term1_double + stod(term2.value);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.type.compare("string") == 0)
//                                {
//                                    // throw error
//                                }
//                            }
//                            else
//                            {
//                                throw "ERROR. Variable " + term2.token + " undefined.";
//                            }
//                            
//                        }
//                        else
//                        {
//                            // throw unkown var error
//                            throw "ERROR. Unkown variable: " + term2.token;
//                            
//                        }
//                        
//                        
//                    }
//                }
//                else if (term1.lexeme.compare("var") == 0)
//                {
//                    // check if symbol table (else throw undefined error)
//                    
//                    //                    unkown test
//                    
//                    if ( var_table_global.has(term1.token))
//                    {
//                        if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                        {
//                            // if type int
//                            if (term1.type.compare("integer") == 0)
//                            {
//                                int term1_int = stoi(term1.value);
//                                
//                                if (term2.lexeme.compare("integer") == 0)
//                                {
//                                    int result = term1_int + stoi(term2.token);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "integer";
//                                }
//                                else if (term2.lexeme.compare("float") == 0)
//                                {
//                                    double result = term1_int + stod(term2.token);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.lexeme.compare("var") == 0)
//                                {
//                                    // check if var is in symbol table (else throw error
//                                    
//                                    // then check if var is double or int
//                                    
//                                    //                                CHECK VAR in table
//                                    //                                check var defined
//                                    
//                                    // check in table
//                                    if ( var_table_global.has(term2.token)) {
//                                        
//                                        // check defined
//                                        if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                        {
//                                            if (term2.type.compare("integer") == 0)
//                                            {
//                                                int result = term1_int + stoi(term2.value);
//                                                term1.token = to_string(result);
//                                                term1.value = "undefined";
//                                                term1.type = "undefined";
//                                                term1.lexeme = "integer";
//                                            }
//                                            else if (term2.type.compare("float") == 0)
//                                            {
//                                                double result = term1_int + stod(term2.value);
//                                                term1.token = to_string(result);
//                                                term1.value = "undefined";
//                                                term1.type = "undefined";
//                                                term1.lexeme = "float";
//                                            }
//                                            else if (term2.type.compare("string") == 0)
//                                            {
//                                                // throw error
//                                            }
//                                        }
//                                        else
//                                        {
//                                            throw "ERROR. Variable " + term2.token + " undefined.";
//                                        }
//                                        
//                                    }
//                                    else
//                                    {
//                                        // throw unkown var error
//                                        throw "ERROR. Unkown variable: " + term2.token;
//                                        
//                                    }
//                                    
//                                    
//                                }
//                            }                        // else if type double
//                            else if (term1.type.compare("float") == 0)
//                            {
//                                double term1_double = stoi(term1.token);
//                                
//                                if (term2.lexeme.compare("integer") == 0)
//                                {
//                                    double result = term1_double + stoi(term2.token);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.lexeme.compare("float") == 0)
//                                {
//                                    double result = term1_double + stod(term2.token);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.lexeme.compare("var") == 0)
//                                {
//                                    // check if var is double or int
//                                    
//                                    //                                CHECK VAR in table
//                                    //                                check var defined
//                                    
//                                    // check in table
//                                    if ( var_table_global.has(term2.token)) {
//                                        
//                                        // check defined
//                                        if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                        {
//                                            if (term2.type.compare("integer") == 0)
//                                            {
//                                                double result = term1_double + stoi(term2.value);
//                                                term1.token = to_string(result);
//                                                term1.value = "undefined";
//                                                term1.type = "undefined";
//                                                term1.lexeme = "integer";
//                                            }
//                                            else if (term2.type.compare("float") == 0)
//                                            {
//                                                double result = term1_double + stod(term2.value);
//                                                term1.token = to_string(result);
//                                                term1.value = "undefined";
//                                                term1.type = "undefined";
//                                                term1.lexeme = "float";
//                                            }
//                                            else if (term2.type.compare("string") == 0)
//                                            {
//                                                // throw error
//                                            }
//                                        }
//                                        else
//                                        {
//                                            throw "ERROR. Variable " + term2.token + " undefined.";
//                                        }
//                                        
//                                    }
//                                    else
//                                    {
//                                        // throw unkown var error
//                                        throw "ERROR. Unkown variable: " + term2.token;
//                                        
//                                    }
//                                    
//                                    
//                                }
//                            }
//                            // else if type string
//                            else if (term1.type.compare("string") == 0)
//                            {
//                                // throw error
//                            }
//                        }
//                        else
//                        {
//                            throw "ERROR. Variable " + term2.token + " undefined.";
//                        }
//                        
//                    }
//                    else
//                    {
//                        // throw unkown var error
//                        throw "ERROR. Unkown variable: " + term1.token;
//                    }
//                }
//            }
//            else if (category.compare("minus_op") == 0)
//            {
//                if (term1.lexeme.compare("integer") == 0)
//                {
//                    int term1_int = stoi(term1.token);
//                    
//                    if (term2.lexeme.compare("integer") == 0)
//                    {
//                        int result = term1_int - stoi(term2.token);
//                        term1.token = to_string(result);
//                        term1.value = "undefined";
//                        term1.type = "undefined";
//                        term1.lexeme = "integer";
//                    }
//                    else if (term2.lexeme.compare("float") == 0)
//                    {
//                        double result = term1_int - stod(term2.token);
//                        term1.token = to_string(result);
//                        term1.value = "undefined";
//                        term1.type = "undefined";
//                        term1.lexeme = "float";
//                    }
//                    else if (term2.lexeme.compare("var") == 0)
//                    {
//                        // check if var is double or int
//                        
//                        //                        CHECK VAR in table
//                        //                        check var defined
//                        
//                        // check in table
//                        if ( var_table_global.has(term2.token)) {
//                            
//                            // check defined
//                            if (! (var_table_global.get(term2.token).value.compare("undefined") == 0)) {
//                                // check in table
//                                if (term2.type.compare("integer") == 0)
//                                {
//                                    int result = term1_int - stoi(term2.value);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "integer";
//                                }
//                                else if (term2.type.compare("float") == 0)
//                                {
//                                    double result = term1_int - stod(term2.value);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.type.compare("string") == 0)
//                                {
//                                    // throw error
//                                }
//                                
//                            }
//                            else
//                            {
//                                // throw undefined error
//                                throw "ERROR. Variable " + term2.token + " undefined.";
//                            }
//                            
//                        }
//                        else
//                        {
//                            // throw unkown var error
//                            throw "ERROR. Unkown variable: " + term2.token;
//                            
//                        }
//                        
//                    }
//                }
//                else if (term1.lexeme.compare("float") == 0)
//                {
//                    double term1_double = stoi(term1.token);
//                    
//                    if (term2.lexeme.compare("integer") == 0)
//                    {
//                        double result = term1_double - stoi(term2.token);
//                        term1.token = to_string(result);
//                        term1.value = "undefined";
//                        term1.type = "undefined";
//                        term1.lexeme = "float";
//                    }
//                    else if (term2.lexeme.compare("float") == 0)
//                    {
//                        double result = term1_double - stod(term2.token);
//                        term1.token = to_string(result);
//                        term1.value = "undefined";
//                        term1.type = "undefined";
//                        term1.lexeme = "float";
//                    }
//                    else if (term2.lexeme.compare("var") == 0)
//                    {
//                        // check if var is double or int
//                        
//                        //                        CHECK VAR in table
//                        //                        check var defined
//                        
//                        
//                        
//                        // check in table
//                        if ( var_table_global.has(term2.token)) {
//                            
//                            // check defined
//                            if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                            {
//                                if (term2.type.compare("integer") == 0)
//                                {
//                                    double result = term1_double - stoi(term2.value);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.type.compare("float") == 0)
//                                {
//                                    double result = term1_double - stod(term2.value);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.type.compare("string") == 0)
//                                {
//                                    // throw error
//                                }
//                            }
//                            else
//                            {
//                                throw "ERROR. Variable " + term2.token + " undefined.";
//                            }
//                            
//                        }
//                        else
//                        {
//                            // throw unkown var error
//                            throw "ERROR. Unkown variable: " + term2.token;
//                            
//                        }
//                        
//                        
//                    }
//                }
//                else if (term1.lexeme.compare("var") == 0)
//                {
//                    // check if symbol table (else throw undefined error)
//                    
//                    //                    unkown test
//                    
//                    if ( var_table_global.has(term1.token))
//                    {
//                        if ( !(var_table_global.get(term1.token).value.compare("undefined") == 0) )
//                        {
//                            // if type int
//                            if (term1.type.compare("integer") == 0)
//                            {
//                                int term1_int = stoi(term1.value);
//                                
//                                if (term2.lexeme.compare("integer") == 0)
//                                {
//                                    int result = term1_int - stoi(term2.token);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "integer";
//                                }
//                                else if (term2.lexeme.compare("float") == 0)
//                                {
//                                    double result = term1_int - stod(term2.token);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                    
//                                }
//                                else if (term2.lexeme.compare("var") == 0)
//                                {
//                                    // check if var is double or int
//                                    
//                                    //                                CHECK VAR in table
//                                    //                                check var defined
//                                    
//                                    // check in table
//                                    if ( var_table_global.has(term2.token)) {
//                                        
//                                        // check defined
//                                        if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                        {
//                                            if (term2.type.compare("integer") == 0)
//                                            {
//                                                int result = term1_int - stoi(term2.value);
//                                                term1.token = to_string(result);
//                                                
//                                                
//                                                
//                                            }
//                                            else if (term2.type.compare("float") == 0)
//                                            {
//                                                double result = term1_int - stod(term2.value);
//                                                term1.token = to_string(result);
//                                            }
//                                            else if (term2.type.compare("string") == 0)
//                                            {
//                                                // throw error
//                                            }
//                                        }
//                                        else
//                                        {
//                                            throw "ERROR. Variable " + term2.token + " undefined.";
//                                        }
//                                        
//                                    }
//                                    else
//                                    {
//                                        // throw unkown var error
//                                        throw "ERROR. Unkown variable: " + term2.token;
//                                        
//                                    }
//                                    
//                                    
//                                }
//                            }
//                            // else if type double
//                            else if (term1.type.compare("float") == 0)
//                            {
//                                double term1_double = stoi(term1.token);
//                                
//                                if (term2.lexeme.compare("integer") == 0)
//                                {
//                                    double result = term1_double - stoi(term2.token);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.lexeme.compare("float") == 0)
//                                {
//                                    double result = term1_double - stod(term2.token);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.lexeme.compare("var") == 0)
//                                {
//                                    // check if var is double or int
//                                    
//                                    //                                CHECK VAR in table
//                                    //                                check var defined
//                                    
//                                    // check in table
//                                    if ( var_table_global.has(term2.token)) {
//                                        
//                                        // check defined
//                                        if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                        {
//                                            if (term2.type.compare("integer") == 0)
//                                            {
//                                                double result = term1_double - stoi(term2.value);
//                                                term1.token = to_string(result);
//                                                term1.value = "undefined";
//                                                term1.type = "undefined";
//                                                term1.lexeme = "float";
//                                            }
//                                            else if (term2.type.compare("float") == 0)
//                                            {
//                                                double result = term1_double - stod(term2.value);
//                                                term1.token = to_string(result);
//                                                term1.value = "undefined";
//                                                term1.type = "undefined";
//                                                term1.lexeme = "float";
//                                            }
//                                            else if (term2.type.compare("string") == 0)
//                                            {
//                                                // throw error
//                                            }
//                                        }
//                                        else
//                                        {
//                                            throw "ERROR. Variable " + term2.token + " undefined.";
//                                        }
//                                        
//                                    }
//                                    else
//                                    {
//                                        // throw unkown var error
//                                        throw "ERROR. Unkown variable: " + term2.token;
//                                        
//                                    }
//                                    
//                                    
//                                }
//                            }
//                            // else if type string
//                            else if (term1.type.compare("string") == 0)
//                            {
//                                // throw error
//                            }
//                        }
//                        else
//                        {
//                            throw "ERROR. Variable " + term2.token + " undefined.";
//                        }
//                    }
//                    else
//                    {
//                        // throw unkown var error
//                        throw "ERROR. Unkown variable: " + term1.token;
//                    }
//                }
//            }
//            
//        }
//    }
//    else
//    {
//        append(toPrint, "Token ERROR. Expected math_pow (integer, string, float, minus_up, or var) and got a " + token + "\n");
//        //        return;
//    }
//    append(toPrint, "Exit Math\n");
//    
//    return term1;
//    
//}
//
//// math_value
//Val parseMath_value(string & toPrint)
//{
//    append(toPrint, "Enter Math_value\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    
//    Val term1;
//    Val term2;
//    
//    if ((category.compare("integer") == 0) ||
//        (category.compare("single_quote") == 0) ||
//        (category.compare("float") == 0) ||
//        (category.compare("minus_op") == 0) ||
//        (category.compare("var") == 0) ||
//        (category.compare("p_left") == 0) ) // math pow check
//    {
//        //append(toPrint, "Token Match. Expected math_pow (integer, string, float, minus_up, or var) and got a " + token + "\n");
//        term1 = parseMath_term(toPrint);
//        
//        // look ahead
//        //currToken_index++;
//        token = tokenList[currToken_index][0];
//        category = tokenList[currToken_index][1];
//        if ((category.compare("mult_op") == 0) || (category.compare("div_op") == 0))
//        {
//            append(toPrint, "Token Match. Expected mult_op or div_op and got a " + token + "\n");
//            currToken_index++;
//            
//            term2 = parseMath(toPrint);
//            
//            if (category.compare("mult_op") == 0)
//            {
//                if (term1.lexeme.compare("integer") == 0)
//                {
//                    
//                    int term1_int = stoi(term1.token);
//                    
//                    if (term2.lexeme.compare("integer") == 0)
//                    {
//                        int result = term1_int * stoi(term2.token);
//                        term1.token = to_string(result);
//                        term1.value = "undefined";
//                        term1.type = "undefined";
//                        term1.lexeme = "integer";
//                    }
//                    else if (term2.lexeme.compare("float") == 0)
//                    {
//                        double result = term1_int * stod(term2.token);
//                        term1.token = to_string(result);
//                        term1.value = "undefined";
//                        term1.type = "undefined";
//                        term1.lexeme = "float";
//                    }
//                    else if (term2.lexeme.compare("var") == 0)
//                    {
//                        // check if var is double or int
//                        
//                        //                        CHECK VAR in table
//                        //                        check var defined
//                        
//                        // check in table
//                        if ( var_table_global.has(term2.token)) {
//                            
//                            // check defined
//                            if (! (var_table_global.get(term2.token).value.compare("undefined") == 0)) {
//                                // check in table
//                                
//                                if (term2.type.compare("integer") == 0)
//                                {
//                                    int result = term1_int * stoi(term2.value);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "integer";
//                                }
//                                else if (term2.type.compare("float") == 0)
//                                {
//                                    double result = term1_int * stod(term2.value);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.type.compare("string") == 0)
//                                {
//                                    // throw error
//                                }
//                            }
//                            else
//                            {
//                                // throw undefined error
//                                throw "ERROR. Variable " + term2.token + " undefined.";
//                            }
//                            
//                        }
//                        else
//                        {
//                            // throw unkown var error
//                            throw "ERROR. Unkown variable: " + term2.token;
//                            
//                        }
//                        
//                        
//                    }
//                }
//                else if (term1.lexeme.compare("float") == 0)
//                {
//                    double term1_double = stoi(term1.token);
//                    
//                    if (term2.lexeme.compare("integer") == 0)
//                    {
//                        double result = term1_double * stoi(term2.token);
//                        term1.token = to_string(result);
//                        term1.value = "undefined";
//                        term1.type = "undefined";
//                        term1.lexeme = "integer";
//                    }
//                    else if (term2.lexeme.compare("float") == 0)
//                    {
//                        double result = term1_double * stod(term2.token);
//                        term1.token = to_string(result);
//                        term1.value = "undefined";
//                        term1.type = "undefined";
//                        term1.lexeme = "float";
//                    }
//                    else if (term2.lexeme.compare("var") == 0)
//                    {
//                        // check if var is double or int
//                        
//                        //                        CHECK VAR in table
//                        //                        check var defined
//                        
//                        // check in table
//                        if ( var_table_global.has(term2.token)) {
//                            
//                            // check defined
//                            if (! (var_table_global.get(term2.token).value.compare("undefined") == 0)) {
//                                // check in table
//                                if (term2.type.compare("integer") == 0)
//                                {
//                                    double result = term1_double * stoi(term2.value);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.type.compare("float") == 0)
//                                {
//                                    double result = term1_double * stod(term2.value);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.type.compare("string") == 0)
//                                {
//                                    // throw error
//                                }
//                                
//                            }
//                            else
//                            {
//                                // throw undefined error
//                                throw "ERROR. Variable " + term2.token + " undefined.";
//                            }
//                            
//                        }
//                        else
//                        {
//                            // throw unkown var error
//                            throw "ERROR. Unkown variable: " + term2.token;
//                            
//                        }
//                        
//                        
//                    }
//                }
//                else if (term1.lexeme.compare("var") == 0)
//                {
//                    // check if symbol table (else throw undefined error)
//                    
//                    //                    unkown test
//                    
//                    if ( var_table_global.has(term1.token))
//                    {
//                        if ( !(var_table_global.get(term1.token).value.compare("undefined") == 0) )
//                        {
//                            // if type int
//                            if (term1.type.compare("integer") == 0)
//                            {
//                                int term1_int = stoi(term1.value);
//                                
//                                if (term2.lexeme.compare("integer") == 0)
//                                {
//                                    int result = term1_int * stoi(term2.token);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "integer";
//                                }
//                                else if (term2.lexeme.compare("float") == 0)
//                                {
//                                    double result = term1_int * stod(term2.token);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.lexeme.compare("var") == 0)
//                                {
//                                    // check if var is in symbol table (else throw error
//                                    
//                                    // then check if var is double or int
//                                    //
//                                    //                                CHECK VAR in table
//                                    //                                check var defined
//                                    
//                                    // check in table
//                                    if ( var_table_global.has(term2.token)) {
//                                        
//                                        // check defined
//                                        if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                        {
//                                            if (term2.type.compare("integer") == 0)
//                                            {
//                                                int result = term1_int * stoi(term2.value);
//                                                term1.token = to_string(result);
//                                                term1.value = "undefined";
//                                                term1.type = "undefined";
//                                                term1.lexeme = "integer";
//                                            }
//                                            else if (term2.type.compare("float") == 0)
//                                            {
//                                                double result = term1_int * stod(term2.value);
//                                                term1.token = to_string(result);
//                                                term1.value = "undefined";
//                                                term1.type = "undefined";
//                                                term1.lexeme = "float";
//                                            }
//                                            else if (term2.type.compare("string") == 0)
//                                            {
//                                                // throw error
//                                            }
//                                        }
//                                        else
//                                        {
//                                            throw "ERROR. Variable " + term2.token + " undefined.";
//                                        }
//                                        
//                                    }
//                                    else
//                                    {
//                                        // throw unkown var error
//                                        throw "ERROR. Unkown variable: " + term2.token;
//                                        
//                                    }
//                                    
//                                    
//                                }
//                            }                        // else if type double
//                            else if (term1.type.compare("float") == 0)
//                            {
//                                double term1_double = stoi(term1.token);
//                                
//                                if (term2.lexeme.compare("integer") == 0)
//                                {
//                                    double result = term1_double * stoi(term2.token);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.lexeme.compare("float") == 0)
//                                {
//                                    double result = term1_double * stod(term2.token);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.lexeme.compare("var") == 0)
//                                {
//                                    // check if var is double or int
//                                    
//                                    //                                CHECK VAR in table
//                                    //                                check var defined
//                                    
//                                    // check in table
//                                    if ( var_table_global.has(term2.token)) {
//                                        
//                                        // check defined
//                                        if (! (var_table_global.get(term2.token).value.compare("undefined") == 0)) {
//                                            // check in table
//                                            if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                            {
//                                                if (term2.type.compare("integer") == 0)
//                                                {
//                                                    double result = term1_double * stoi(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.value = "undefined";
//                                                    term1.type = "undefined";
//                                                    term1.lexeme = "float";
//                                                }
//                                                else if (term2.type.compare("float") == 0)
//                                                {
//                                                    double result = term1_double * stod(term2.value);
//                                                    term1.token = to_string(result);
//                                                    term1.value = "undefined";
//                                                    term1.type = "undefined";
//                                                    term1.lexeme = "float";
//                                                }
//                                                else if (term2.type.compare("string") == 0)
//                                                {
//                                                    // throw error
//                                                }
//                                            }
//                                            else
//                                            {
//                                                throw "ERROR. Variable " + term2.token + " undefined.";
//                                            }
//                                            
//                                        }
//                                        else
//                                        {
//                                            // throw undefined error
//                                            throw "ERROR. Variable " + term2.token + " undefined.";
//                                        }
//                                        
//                                    }
//                                    else
//                                    {
//                                        // throw unkown var error
//                                        throw "ERROR. Unkown variable: " + term2.token;
//                                        
//                                    }
//                                    
//                                    
//                                }
//                            }
//                            // else if type string
//                            else if (term1.type.compare("string") == 0)
//                            {
//                                // throw error
//                            }
//                        }
//                        else
//                        {
//                            throw "Variable " + term1.token + " undefined.";
//                        }
//                    }
//                    else
//                    {
//                        // throw unkown var error
//                        throw "ERROR. Unkown variable: " + term1.token;
//                    }
//                }
//            }
//            else if (category.compare("div_op") == 0)
//            {
//                if (term1.lexeme.compare("integer") == 0)
//                {
//                    int term1_int = stoi(term1.token);
//                    
//                    if (term2.lexeme.compare("integer") == 0)
//                    {
//                        int result = term1_int / stoi(term2.token);
//                        term1.token = to_string(result);
//                        term1.value = "undefined";
//                        term1.type = "undefined";
//                        term1.lexeme = "integer";
//                    }
//                    else if (term2.lexeme.compare("float") == 0)
//                    {
//                        double result = term1_int / stod(term2.token);
//                        term1.token = to_string(result);
//                        term1.value = "undefined";
//                        term1.type = "undefined";
//                        term1.lexeme = "float";
//                    }
//                    else if (term2.lexeme.compare("var") == 0)
//                    {
//                        // check if var is double or int
//                        
//                        //                        CHECK VAR in table
//                        //                        check var defined
//                        
//                        // check in table
//                        if ( var_table_global.has(term2.token)) {
//                            
//                            // check defined
//                            if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                            {
//                                if (term2.type.compare("integer") == 0)
//                                {
//                                    int result = term1_int / stoi(term2.value);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "integer";
//                                }
//                                else if (term2.type.compare("float") == 0)
//                                {
//                                    double result = term1_int / stod(term2.value);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.type.compare("string") == 0)
//                                {
//                                    // throw error
//                                }
//                            }
//                            else
//                            {
//                                throw "ERROR. Variable " + term2.token + " undefined.";
//                            }
//                            
//                        }
//                        else
//                        {
//                            // throw unkown var error
//                            throw "ERROR. Unkown variable: " + term2.token;
//                        }
//                    }
//                }
//                else if (term1.lexeme.compare("float") == 0)
//                {
//                    double term1_double = stoi(term1.token);
//                    
//                    if (term2.lexeme.compare("integer") == 0)
//                    {
//                        double result = term1_double / stoi(term2.token);
//                        term1.token = to_string(result);
//                        term1.value = "undefined";
//                        term1.type = "undefined";
//                        term1.lexeme = "float";
//                    }
//                    else if (term2.lexeme.compare("float") == 0)
//                    {
//                        double result = term1_double / stod(term2.token);
//                        term1.token = to_string(result);
//                        term1.value = "undefined";
//                        term1.type = "undefined";
//                        term1.lexeme = "float";
//                    }
//                    else if (term2.lexeme.compare("var") == 0)
//                    {
//                        // check if var is double or int
//                        
//                        //                        CHECK VAR in table
//                        //                        check var defined
//                        
//                        // check in table
//                        if ( var_table_global.has(term2.token)) {
//                            
//                            // check defined
//                            if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                            {
//                                if (term2.type.compare("integer") == 0)
//                                {
//                                    double result = term1_double / stoi(term2.value);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.type.compare("float") == 0)
//                                {
//                                    double result = term1_double / stod(term2.value);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.type.compare("string") == 0)
//                                {
//                                    // throw error
//                                }
//                            }
//                            else
//                            {
//                                throw "ERROR. Variable " + term2.token + " undefined.";
//                            }
//                            
//                        }
//                        else
//                        {
//                            // throw unkown var error
//                            throw "ERROR. Unkown variable: " + term2.token;
//                            
//                        }
//                        
//                        
//                    }
//                }
//                else if (term1.lexeme.compare("var") == 0)
//                {
//                    // check if symbol table (else throw undefined error)
//                    
//                    //                    unkown test
//                    
//                    if ( var_table_global.has(term1.token))
//                    {
//                        if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                        {
//                            // if type int
//                            if (term1.type.compare("integer") == 0)
//                            {
//                                int term1_int = stoi(term1.value);
//                                
//                                if (term2.lexeme.compare("integer") == 0)
//                                {
//                                    int result = term1_int / stoi(term2.token);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "integer";
//                                }
//                                else if (term2.lexeme.compare("float") == 0)
//                                {
//                                    double result = term1_int / stod(term2.token);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.lexeme.compare("var") == 0)
//                                {
//                                    // check if var is double or int
//                                    
//                                    //                                CHECK VAR in table
//                                    //                                check var defined
//                                    
//                                    // check in table
//                                    if ( var_table_global.has(term2.token)) {
//                                        
//                                        // check defined
//                                        if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                        {
//                                            if (term2.type.compare("integer") == 0)
//                                            {
//                                                int result = term1_int / stoi(term2.value);
//                                                term1.token = to_string(result);
//                                                term1.value = "undefined";
//                                                term1.type = "undefined";
//                                                term1.lexeme = "integer";
//                                            }
//                                            else if (term2.type.compare("float") == 0)
//                                            {
//                                                double result = term1_int / stod(term2.value);
//                                                term1.token = to_string(result);
//                                                term1.value = "undefined";
//                                                term1.type = "undefined";
//                                                term1.lexeme = "float";
//                                            }
//                                            else if (term2.type.compare("string") == 0)
//                                            {
//                                                // throw error
//                                            }
//                                        }
//                                        else
//                                        {
//                                            throw "ERROR. Variable " + term2.token + " undefined.";
//                                        }
//                                        
//                                    }
//                                    else
//                                    {
//                                        // throw unkown var error
//                                        throw "ERROR. Unkown variable: " + term2.token;
//                                        
//                                    }
//                                }
//                            }
//                            // else if type double
//                            else if (term1.type.compare("float") == 0)
//                            {
//                                double term1_double = stoi(term1.token);
//                                
//                                if (term2.lexeme.compare("integer") == 0)
//                                {
//                                    double result = term1_double / stoi(term2.token);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.lexeme.compare("float") == 0)
//                                {
//                                    double result = term1_double / stod(term2.token);
//                                    term1.token = to_string(result);
//                                    term1.value = "undefined";
//                                    term1.type = "undefined";
//                                    term1.lexeme = "float";
//                                }
//                                else if (term2.lexeme.compare("var") == 0)
//                                {
//                                    // check if var is double or int
//                                    
//                                    
//                                    
//                                    //                                CHECK VAR in table
//                                    //                                check var defined
//                                    
//                                    if ( var_table_global.has(term2.token)) {
//                                        
//                                        
//                                        if ( !(var_table_global.get(term2.token).value.compare("undefined") == 0) )
//                                        {
//                                            if (term2.type.compare("integer") == 0)
//                                            {
//                                                double result = term1_double / stoi(term2.value);
//                                                term1.token = to_string(result);
//                                                term1.value = "undefined";
//                                                term1.type = "undefined";
//                                                term1.lexeme = "float";
//                                            }
//                                            else if (term2.type.compare("float") == 0)
//                                            {
//                                                double result = term1_double / stod(term2.value);
//                                                term1.token = to_string(result);
//                                                term1.value = "undefined";
//                                                term1.type = "undefined";
//                                                term1.lexeme = "float";
//                                            }
//                                            else if (term2.type.compare("string") == 0)
//                                            {
//                                                // throw error
//                                            }
//                                        }
//                                        else
//                                        {
//                                            throw "ERROR. Variable " + term2.token + " undefined.";
//                                        }
//                                        
//                                    }
//                                    else
//                                    {
//                                        // throw unkown var error
//                                        throw "ERROR. Unkown variable: " + term2.token;
//                                        
//                                    }
//                                    
//                                }
//                            }
//                            // else if type string
//                            else if (term1.type.compare("string") == 0)
//                            {
//                                // throw error
//                            }
//                        }
//                        else
//                        {
//                            throw "ERROR. Variable " + term2.token + " undefined.";
//                        }
//                    }
//                    else
//                    {
//                        // throw unkown var error
//                        throw "ERROR. Unkown variable: " + term1.token;
//                    }
//                    
//                }
//            }
//        }
//    }
//    else
//    {
//        append(toPrint, "Token ERROR. Expected math_pow (integer, string, float, minus_op, or var) and got a " + token + "\n");
//        //        return;
//    }
//    append(toPrint, "Exit Math_value\n");
//    
//    return term1;
//}
//
//// math_term
//Val parseMath_term(string & toPrint)
//{
//    
//    append(toPrint, "Enter math_term\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    
//    Val term1;
//    Val term2;
//    
//    if ((category.compare("integer") == 0) ||
//        (category.compare("single_quote") == 0) ||
//        (category.compare("float") == 0) ||
//        (category.compare("minus_op") == 0) ||
//        (category.compare("var") == 0) ||
//        (category.compare("p_left") == 0) ) // math_pow possiblities
//    {
//        append(toPrint, "Token Match. Expected math_pow (integer, string, float, minus_op, or var) and got a " + token + "\n");
//        term1 = parseMath_pow(toPrint);
//        
//        // look ahead
//        // currToken_index++;
//        token = tokenList[currToken_index][0];
//        category = tokenList[currToken_index][1];
//        
//        if (category.compare("pow_op") == 0)
//        {
//            currToken_index++;
//            append(toPrint, "Token Match. Expected possible pow_op and got a " + token + "\n");
//            term2 = parseMath(toPrint);
//            
//            // check if same type, if not same, throw error;
//            
//            // if string, throw error
//            
//            // if var, look in symbol table
//            
//            //
//            
//            if (term1.lexeme.compare("integer") == 0)
//            {
//                
//                int term1_int = stoi(term1.token);
//                
//                if (term2.lexeme.compare("integer") == 0)
//                {
//                    int result = pow( term1_int , stoi(term2.token) );
//                    term1.token = to_string(result);
//                    term1.value = "undefined";
//                    term1.type = "undefined";
//                    term1.lexeme = "integer";
//                }
//                else if (term2.lexeme.compare("float") == 0)
//                {
//                    double result = pow( term1_int , stod(term2.token) );
//                    term1.token = to_string(result);
//                    term1.value = "undefined";
//                    term1.type = "undefined";
//                    term1.lexeme = "float";
//                }
//                else if (term2.lexeme.compare("var") == 0)
//                {
//                    // check if var is double or int
//                    
//                    //                    CHECK VAR in table
//                    //                    check var defined
//                    
//                    if ( var_table_global.has(term2.token)) {
//                        
//                        
//                        if (! (var_table_global.get(term2.token).value.compare("undefined") == 0)) {
//                            // check in table
//                            if (term2.type.compare("integer") == 0)
//                            {
//                                int result = pow( term1_int , stoi(term2.value) );
//                                term1.token = to_string(result);
//                                term1.value = "undefined";
//                                term1.type = "undefined";
//                                term1.lexeme = "integer";
//                            }
//                            else if (term2.type.compare("float") == 0)
//                            {
//                                double result = pow( term1_int , stod(term2.value) );
//                                term1.token = to_string(result);
//                                term1.value = "undefined";
//                                term1.type = "undefined";
//                                term1.lexeme = "float";
//                            }
//                            else if (term2.type.compare("string") == 0)
//                            {
//                                // throw error
//                            }
//                            
//                        }
//                        else
//                        {
//                            // throw undefined error
//                            throw "ERROR. Variable " + term2.token + " undefined.";
//                        }
//                        
//                    }
//                    else
//                    {
//                        // throw unkown var error
//                        throw "ERROR. Unkown variable: " + term2.token;
//                        
//                    }
//                    
//                }
//            }
//            else if (term1.lexeme.compare("float") == 0)
//            {
//                double term1_double = stoi(term1.token);
//                
//                if (term2.lexeme.compare("integer") == 0)
//                {
//                    double result = pow( term1_double , stoi(term2.token) );
//                    term1.token = to_string(result);
//                    term1.value = "undefined";
//                    term1.type = "undefined";
//                    term1.lexeme = "float";
//                }
//                else if (term2.lexeme.compare("float") == 0)
//                {
//                    double result = pow( term1_double , stod(term2.token) );
//                    term1.token = to_string(result);
//                    term1.value = "undefined";
//                    term1.type = "undefined";
//                    term1.lexeme = "float";
//                }
//                else if (term2.lexeme.compare("var") == 0)
//                {
//                    // check if var is double or int
//                    
//                    
//                    if ( var_table_global.has(term2.token)) {
//                        
//                        
//                        if (! (var_table_global.get(term2.token).value.compare("undefined") == 0)) {
//                            // check in table
//                            
//                            if (term2.type.compare("integer") == 0)
//                            {
//                                double result = pow( term1_double , stoi(term2.value) );
//                                term1.token = to_string(result);
//                                term1.value = "undefined";
//                                term1.type = "undefined";
//                                term1.lexeme = "float";
//                            }
//                            else if (term2.type.compare("float") == 0)
//                            {
//                                double result = pow( term1_double , stod(term2.value) );
//                                term1.token = to_string(result);
//                                term1.value = "undefined";
//                                term1.type = "undefined";
//                                term1.lexeme = "float";
//                            }
//                            else if (term2.type.compare("string") == 0)
//                            {
//                                // throw error
//                            }
//                            
//                        }
//                        else
//                        {
//                            // throw undefined error
//                            throw "ERROR. Variable " + term2.token + " undefined.";
//                        }
//                        
//                    }
//                    else
//                    {
//                        // throw unkown var error
//                        throw "ERROR. Unkown variable: " + term2.token;
//                        
//                    }
//                }
//            }
//            else if (term1.lexeme.compare("var") == 0)
//            {
//                // check if symbol table (else throw undefined error)
//                
//                //                unkown test
//                
//                if ( var_table_global.has(term1.token))
//                {
//                    if ( !(var_table_global.get(term1.token).value.compare("undefined") == 0) )
//                    {
//                        // if type int
//                        if (term1.type.compare("integer") == 0)
//                        {
//                            int term1_int = stoi(term1.value);
//                            
//                            if (term2.lexeme.compare("integer") == 0)
//                            {
//                                int result = pow( term1_int , stoi(term2.token) );
//                                term1.token = to_string(result);
//                                term1.value = "undefined";
//                                term1.type = "undefined";
//                                term1.lexeme = "integer";
//                            }
//                            else if (term2.lexeme.compare("float") == 0)
//                            {
//                                double result = pow( term1_int , stod(term2.token) );
//                                term1.token = to_string(result);
//                                term1.value = "undefined";
//                                term1.type = "undefined";
//                                term1.lexeme = "float";
//                            }
//                            else if (term2.lexeme.compare("var") == 0)
//                            {
//                                // check if var is double or int
//                                
//                                // check in table
//                                if ( var_table_global.has(term2.token)) {
//                                    
//                                    // check defined
//                                    if (! (var_table_global.get(term2.token).value.compare("undefined") == 0)) {
//                                        // check in table
//                                        
//                                        if (term2.type.compare("integer") == 0)
//                                        {
//                                            int result = pow( term1_int , stoi(term2.value) );
//                                            term1.token = to_string(result);
//                                            term1.value = "undefined";
//                                            term1.type = "undefined";
//                                            term1.lexeme = "integer";
//                                        }
//                                        else if (term2.type.compare("float") == 0)
//                                        {
//                                            double result = pow( term1_int , stod(term2.value) );
//                                            term1.token = to_string(result);
//                                            term1.value = "undefined";
//                                            term1.type = "undefined";
//                                            term1.lexeme = "float";
//                                        }
//                                        else if (term2.type.compare("string") == 0)
//                                        {
//                                            // throw error
//                                        }
//                                        
//                                    }
//                                    else
//                                    {
//                                        // throw undefined error
//                                        throw "ERROR. Variable " + term2.token + " undefined.";
//                                    }
//                                    
//                                }
//                                else
//                                {
//                                    // throw unkown var error
//                                    throw "ERROR. Unkown variable: " + term2.token;
//                                    
//                                }
//                            }
//                        }
//                        // else if type double
//                        else if (term1.type.compare("float") == 0)
//                        {
//                            double term1_double = stoi(term1.token);
//                            
//                            if (term2.lexeme.compare("integer") == 0)
//                            {
//                                double result = pow( term1_double , stoi(term2.token) );
//                                term1.token = to_string(result);
//                                term1.value = "undefined";
//                                term1.type = "undefined";
//                                term1.lexeme = "float";
//                            }
//                            else if (term2.lexeme.compare("float") == 0)
//                            {
//                                double result = pow( term1_double , stod(term2.token) );
//                                term1.token = to_string(result);
//                                term1.value = "undefined";
//                                term1.type = "undefined";
//                                term1.lexeme = "float";
//                            }
//                            else if (term2.lexeme.compare("var") == 0)
//                            {
//                                // check if var is double or int
//                                
//                                
//                                // check in table
//                                if ( var_table_global.has(term2.token)) {
//                                    
//                                    // check defined
//                                    if (! (var_table_global.get(term2.token).value.compare("undefined") == 0)) {
//                                        // check in table
//                                        
//                                        if (term2.type.compare("integer") == 0)
//                                        {
//                                            double result = pow( term1_double , stoi(term2.value) );
//                                            term1.token = to_string(result);
//                                            term1.value = "undefined";
//                                            term1.type = "undefined";
//                                            term1.lexeme = "float";
//                                        }
//                                        else if (term2.type.compare("float") == 0)
//                                        {
//                                            double result = pow( term1_double , stod(term2.value) );
//                                            term1.token = to_string(result);
//                                        }
//                                        else if (term2.type.compare("string") == 0)
//                                        {
//                                            // throw error
//                                        }
//                                    }
//                                    else
//                                    {
//                                        // throw undefined error
//                                        throw "ERROR. Variable " + term2.token + " undefined.";
//                                    }
//                                    
//                                }
//                                else
//                                {
//                                    // throw unkown var error
//                                    throw "ERROR. Unkown variable: " + term2.token;
//                                    
//                                }
//                                
//                                
//                            }
//                        }
//                        // else if type string
//                        else if (term1.type.compare("string") == 0)
//                        {
//                            // throw error
//                        }
//                        
//                    }
//                    else
//                    {
//                        throw "Variable " + term1.token + " undefined.";
//                    }
//                }
//                else
//                {
//                    // throw unkown var error
//                    throw "ERROR. Unkown variable: " + term1.token;
//                }
//            }
//        }
//    }
//    else
//    {
//        append(toPrint, "Token Error. Expected math_pow (integer, string, float, minus_op, or var) and got a " + token + "\n");
//    }
//    append(toPrint, "Exit math_term\n");
//    
//    return term1;
//}
//
//// math_pow
//Val parseMath_pow(string & toPrint)
//{
//    
//    append(toPrint, "Enter math_pow\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    
//    Val math_pow;
//    
//    if (category.compare("minus_op") == 0) // CHANGE THIS
//    {
//        // ( (category.compare("minus_op") == 0) && (tokenList[currToken_index + 1][1].compare("integer") == 0) ) ||
//        // ( (category.compare("minus_op") == 0) && (tokenList[currToken_index + 1][1].compare("integer") == 0) ) )
//        
//        
//        // LOOK AHEAD TO decide on integer, float, or var
//        if (tokenList[currToken_index + 1][1].compare("var") == 0)
//        {
//            math_pow = parseVar(toPrint);
//        }
//        else if (tokenList[currToken_index + 1][1].compare("integer") == 0)
//        {
//            math_pow = parseInteger(toPrint);
//        }
//        else if (tokenList[currToken_index + 1][1].compare("float") == 0)
//        {
//            math_pow = parseFloat(toPrint);
//        }
//        else
//        {
//            append(toPrint, "Token Error. Expected  and got a " + token + "\n");  // START HERE
//            //            return;
//        }
//        
//        
//    }
//    else if (category.compare("p_left") == 0)
//    {
//        math_pow = parsePerens(toPrint);
//    }
//    else if (category.compare("var") == 0)
//    {
//        
//        currToken_index++;
//        token = tokenList[currToken_index][0]; // check ahead for either ( or .
//        category = tokenList[currToken_index][1];
//        currToken_index--; // put currToken back to var for both parseFunction_invoc or parseOb_manip
//        if ((category.compare("dot") == 0))
//        {
//            parseOb_manip(toPrint);
//        }
//        else if ((category.compare("p_left") == 0))
//        {
//            parseFunction_invoc(toPrint);
//        }
//        else
//        {
//            math_pow = parseVar(toPrint);
//        }
//    }
//    else if ((category.compare("integer") == 0) ||  // literal check
//             (category.compare("single_quote") == 0) ||
//             (category.compare("float") == 0) )
//    {
//        math_pow = parseLiteral(toPrint);
//    }
//    else
//    {
//        append(toPrint, "Token Error. Expected neg, literal, function_invoc, or ob_manip and got a " + token + "\n");
//        //        return;
//    }
//    append(toPrint, "Exit math_pow\n");
//    
//    return math_pow;
//    
//}
//
//// neg
///*
// void parseNeg(string & toPrint)
// {
// append(toPrint, "Enter neg\n");
// 
// cout << "inside parseNeg: " << toPrint << endl;
// 
// string token = tokenList[currToken_index][0];
// string category = tokenList[currToken_index][1];
// //currToken_index++;
// if (category.compare("minus_op") == 0)
// {
// append(toPrint, "Token Match. Expected var or minus_op and got a " + token + "\n");
// currToken_index++;
// 
// parseVar(toPrint);
// }
// else if (category.compare("var") == 0)
// {
// parseVar(toPrint);
// }
// else
// {
// append(toPrint, "Token Error. Expected var or minus_op and got a " + token + "\n");
// }
// append(toPrint, "Exit neg\n");
// }
// */
//
//// var
//
//// NEED TO: assign value and type from global symbol table
//// update var value for negative values.
//
//Val parseVar(string & toPrint)
//{
//    append(toPrint, "Enter var\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    currToken_index++;
//    
//    Val var = * new Val();
//    
//    // update var function; -- to be placed after all returns from functions
//    //  pass val by reference
//    
//    // if lexeme = "var", then
//    // assign value, type from symbol table
//    
//    
//    
//    if (category.compare("minus_op") == 0)
//    {
//        append(toPrint, "Token Match. Expected possible minus_op and got a " + token + "\n");
//        
//        token = tokenList[currToken_index][0];
//        category = tokenList[currToken_index][1];
//        
//        if (category.compare("var") == 0)
//        {
//            append(toPrint, "Token Match. Expected var and got a " + token + "\n");
//            currToken_index++;
//            
//            var.lexeme = "var";
//            var.token = token;
//            
//            if ( var_table_global.has(var.token)) {
//                
//                if (! (var_table_global.get(var.token).value.compare("undefined") == 0)) {
//                    // check in table
//                    
//                    var.type = var_table_global.get(var.token).type;
//                    var.value = "-" + var_table_global.get(var.token).value;
//                    
//                    
//                    
//                }
//            }
//            
//            //            if ( !var_table_global.has(token) )
//            //            {
//            //                var_table_global.add(var);
//            //            }
//        }
//        else
//        {
//            append(toPrint, "Token Error. Expected var and got a " + token + "\n");
//            //            return;
//        }
//    }
//    else if (category.compare("var") == 0)
//    {
//        append(toPrint, "Token Match. Expected var and got a " + token + "\n");
//        
//        //        currToken_index++;
//        
//        var.lexeme = "var";
//        var.token = token;
//        
//        if ( var_table_global.has(var.token)) {
//            
//            if (! (var_table_global.get(var.token).value.compare("undefined") == 0)) {
//                // check in table
//                
//                var.type = var_table_global.get(var.token).type;
//                var.value = var_table_global.get(var.token).value;
//            }
//        }
//        
//        //        if ( !var_table_global.has(token) )
//        //        {
//        //            var_table_global.add(var);
//        //        }
//        
//    }
//    else
//    {
//        append(toPrint, "Token Error. Expected var and got a " + token + "\n");
//        //        return;
//    }
//    append(toPrint, "Exit var\n");
//    
//    return var;
//}
//
//// literal
//Val parseLiteral(string & toPrint)
//{
//    append(toPrint, "Enter literal\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    //currToken_index++;
//    
//    Val literal; // = * new Val();
//    
//    if (category.compare("integer") == 0)
//    {
//        literal = parseInteger(toPrint);
//    }
//    else if (category.compare("single_quote") == 0)
//    {
//        literal = parseString_token(toPrint);
//    }
//    else if (category.compare("float") == 0)
//    {
//        literal = parseFloat(toPrint);
//    }
//    else
//    {
//        append(toPrint, "Token Error. Expected integer and got a " + token + "\n");
//        //        return;
//    }
//    append(toPrint, "Exit literal\n");
//    
//    return literal;
//    
//}
//
//// <integer_sign> → {-}<integer>
///*
// void parseInteger_sign(string & toPrint)
// {
// append(toPrint, "Enter integer_sign\n");
// string token = tokenList[currToken_index][0];
// string category = tokenList[currToken_index][1];
// 
// if (category.compare("minus_op") == 0)
// {
// currToken_index++;
// append(toPrint, "Token Match. Expected possible minus_op and got a " + token + "\n");
// }
// else
// {
// append(toPrint, "Token Error. Expected integer and got a " + token + "\n");
// }
// append(toPrint, "Exit int\n");
// }
// */
//
//Val parseInteger(string & toPrint)
//{
//    append(toPrint, "Enter int\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    currToken_index++;
//    
//    Val _integer = * new Val();
//    
//    if (category.compare("minus_op") == 0)
//    {
//        append(toPrint, "Token Match. Expected possible minus_op and got a " + token + "\n");
//        
//        token = tokenList[currToken_index][0];
//        category = tokenList[currToken_index][1];
//        if (category.compare("integer") == 0)
//        {
//            append(toPrint, "Token Match. Expected integer and got a " + token + "\n");
//            currToken_index++;
//            _integer.lexeme = "integer";
//            _integer.token = "-" + token;
//            
//        }
//        else
//        {
//            append(toPrint, "Token Error. Expected integer and got a " + token + "\n");
//            //            return;
//        }
//        
//    }
//    else if (category.compare("integer") == 0)
//    {
//        append(toPrint, "Token Match. Expected integer and got a " + token + "\n");
//        
//        _integer.lexeme = "integer";
//        _integer.token = token;
//    }
//    else
//    {
//        append(toPrint, "Token Error. Expected integer or minus_op and got a " + token + "\n");
//        //        return;
//    }
//    append(toPrint, "Exit int\n");
//    
//    return _integer;
//    
//}
//
///*
// void parseFloat_sign(string & toPrint)
// {
// append(toPrint, "Enter float\n");
// string token = tokenList[currToken_index][0];
// string category = tokenList[currToken_index][1];
// currToken_index++;
// if (category.compare("float") == 0)
// {
// append(toPrint, "Token Match. Expected float and got a " + token + "\n");
// }
// else
// {
// append(toPrint, "Token Error. Expected float and got a " + token + "\n");
// }
// append(toPrint, "Exit float\n");
// }
// */
//
//Val parseFloat(string & toPrint)
//{
//    append(toPrint, "Enter float\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    currToken_index++;
//    
//    Val _float = * new Val();
//    
//    if (category.compare("minus_op") == 0)
//    {
//        append(toPrint, "Token Match. Expected possible minus_op and got a " + token + "\n");
//        
//        token = tokenList[currToken_index][0];
//        category = tokenList[currToken_index][1];
//        if (category.compare("float") == 0)
//        {
//            append(toPrint, "Token Match. Expected float and got a " + token + "\n");
//            currToken_index++;
//            
//            _float.lexeme = "float";
//            _float.token = "-" + token;
//            
//        }
//        else
//        {
//            // error
//            append(toPrint, "Token Error. Expected float and got a " + token + "\n");
//            //            return;
//        }
//        
//    }
//    else if (category.compare("float") == 0)
//    {
//        append(toPrint, "Token Match. Expected float or minus_op and got a " + token + "\n");
//        _float.lexeme = "float";
//        _float.token = token;
//    }
//    else
//    {
//        append(toPrint, "Token Error. Expected float or minus_op and got a " + token + "\n");
//        //        return;
//    }
//    append(toPrint, "Exit float\n");
//    
//    return _float;
//    
//}
//
//Val parseString_token(string & toPrint)
//{
//    append(toPrint, "Enter string_token\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    
//    Val _string = * new Val();
//    
//    if ((category.compare("single_quote") == 0) && (currToken_index < tokenList.size()))
//    {
//        append(toPrint, "Token Match. Expected single_quote and got a " + token + "\n");
//        
//        currToken_index++; // go to next
//        token = tokenList[currToken_index][0];
//        category = tokenList[currToken_index][1];
//        
//        if ((category.compare("string") == 0) && (currToken_index < tokenList.size()))
//        {
//            append(toPrint, "Token Match. Expected string and got a " + token + "\n");
//            _string.lexeme = "string";
//            _string.token = token;
//            
//        }
//        else
//        {
//            append(toPrint, "Token Error. Expected string and got a " + token + "\n");
//            //            return;
//        }
//        
//        //        term(toPrint);
//        
//        // go to next
//        currToken_index++;
//        token = tokenList[currToken_index][0];
//        category = tokenList[currToken_index][1];
//        
//        if (category.compare("single_quote") == 0)
//        {
//            append(toPrint, "Token Match. Expected single_quote and got a " + token + "\n");
//            currToken_index++;
//        }
//        else
//        {
//            append(toPrint, "Token Error. Expected single_quote and got a " + token + "\n");
//            //            return;
//        }
//        
//        
//    }
//    else
//    {
//        //        Var(toPrint);
//        currToken_index++;
//        append(toPrint, "Token Error. Expected single_quote and got a " + token + "\n");
//    }
//    append(toPrint, "Exit string_token\n");
//    
//    return _string;
//    
//}
//
//Val parsePerens(string & toPrint)
//{
//    append(toPrint, "Enter perens\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    
//    Val term;
//    
//    if ( (category.compare("p_left") == 0) )
//    {
//        append(toPrint, "Token Match. Expected p_left and got a " + token + "\n");
//        
//        currToken_index++;
//        term = parseTerm(toPrint); // assuming that parseTerm() will increment currToken_index
//        
//        
//        
//        token = tokenList[currToken_index][0];
//        category = tokenList[currToken_index][1];
//        
//        if ( (category.compare("p_right") == 0) )
//        {
//            append(toPrint, "Token Match.xpected p_right and got a " + token + "\n");
//            currToken_index++;
//            
//        }
//        else
//        {
//            append(toPrint, "Token ERROR. Expected p_right and got a " + token + "\n");
//            //            return;
//        }
//    }
//    else if ( (category.compare("var") == 0) )
//    {
//        Val var = parseVar(toPrint);
//        return var;
//    }
//    else
//    {
//        append(toPrint, "Token ERROR. Expected p_left or var and got a " + token + "\n");
//        //        return;
//    }
//    append(toPrint, "Exit perens\n");
//    
//    
//    return term;
//    
//}
//
//Val parseRel_op(string & toPrint)
//{
//    append(toPrint, "Enter rel_op\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    currToken_index++;
//    
//    Val rel_op = * new Val();
//    
//    if ( (category.compare("less_than") == 0)     ||
//        (category.compare("greater_than") == 0)      ||
//        (category.compare("less_than_equal") == 0)   ||
//        (category.compare("greater_than_equal") == 0) )
//    {
//        append(toPrint, "Token Match. Expected rel_op and got a " + token + "\n");
//        rel_op.lexeme = "rel_op";
//        rel_op.token = token;
//    }
//    else
//    {
//        append(toPrint, "Token Error. Expected rel_op and got a " + token + "\n");
//        //        return;
//    }
//    append(toPrint, "Exit rel_op\n");
//    
//    return rel_op;
//}
//
//
//Val parseBool_op(string & toPrint)
//{
//    append(toPrint, "Enter bool_op\n");
//    string token = tokenList[currToken_index][0];
//    string category = tokenList[currToken_index][1];
//    currToken_index++;
//    
//    Val bool_op = * new Val();
//    
//    if ( (category.compare("and_op") == 0)     ||
//        (category.compare("or_op") == 0)      ||
//        (category.compare("equal_op") == 0)   ||
//        (category.compare("not_equal_op") == 0) )
//    {
//        append(toPrint, "Token Match. Expected bool_op and got a " + token + "\n");
//        
//        bool_op.lexeme = "bool_op";
//        bool_op.token = token;
//        
//    }
//    else
//    {
//        append(toPrint, "Token Error. Expected bool_op and got a " + token + "\n");
//        //        return;
//    }
//    append(toPrint, "Exit bool_op\n");
//    return bool_op;
//}
//
//
//
//
//int main(int argc, const char * argv[]) {
//    
//    int quittingtime = 0;
//    
//    if(argc > 1){
//        
//        string inputDataFileName( argv[1] );
//        ifstream inFile(inputDataFileName.c_str());
//        string line;
//        int position = 0;
//        string stringToTokenize;
//        
//        while(getline(inFile, line)){
//            
//            stringToTokenize += line;
//        }
//        
//        for(int i = 0; i < stringToTokenize.length(); i++){
//            
//            if(stringToTokenize[i] == '\n' || stringToTokenize[i] == '\t'){
//                
//                inputleng++;
//                
//            }
//            
//            if(stringToTokenize[i] == '{' || stringToTokenize[i] == '}'){
//                inputleng++;
//                
//            }
//            
//            else{
//                
//                userinput[i] = stringToTokenize[i];
//                inputleng++;
//            }
//            
//            
//        }
//        
//        inputleng++;
//        userinput[inputleng] = ' ';
//        inputleng++;
//        userinput[inputleng] = '$';
//        
//        // Do stuff
//        // tokenize input
//        // tokenList = tokenize();  // your implementation here will likely vary
//        makeMap();
//        tokenize();
//        
//        cout << "Number of Characters read:  " << inputleng << endl;
//        cout << "Input:  ";
//        for (int i = 0; i <= inputleng;  i++)// mimic input to confirm userinput array is populated.
//            cout << userinput[i];
//        cout << endl;
//                 
//        while (!quittingtime)
//        {
//            userinput[0] = ' ';
//            
//            reader(); // read input and store globally to userinput
//            
//            if (matches(pos, 4, "end"))    // quit, rather than use "matches" you can replace this with a condition for the "quit" token
//                quittingtime = 1;
//            else
//            {
//                if (userinput[inputleng] == ENDOFINPUT) {
//                    // Do stuff
//                    // tokenize input
//                    // tokenList = tokenize();  // your implementation here will likely vary
//                    
//                    makeMap();
//                    
//                    tokenize();
//                    
//                    string stringToPrint = "";
//                    currToken_index = 0;
//                    
//                    try {
//                        parseProgram(stringToPrint);
//                    }
//                    catch (string error)
//                    {
//                        cout << "ERRROR: missing end" << endl;
//                    }
//                    
//                    cout << endl;
//                    cout << stringToPrint << endl;
//                    
//                }
//                else
//                {
//                    cout << "Input Error: token found but end of input expected:  " << userinput[inputleng] << endl;
//                }
//            }
//            
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
//        }
//        
//    } // END read from file
//    
//    else{
//        while (!quittingtime)
//        {
//            reader(); // read input and store globally to userinput
//            Val output;
//            
//            if (matches(pos, 4, "quit"))    // quit, rather than use "matches" you can replace this with a condition for the "quit" token
//                quittingtime = 1;
//            else
//            {
//                if (userinput[inputleng] == ENDOFINPUT) {
//                    // Do stuff
//                    // tokenize input
//                    // tokenList = tokenize();  // your implementation here will likely vary
//                    
//                    makeMap();
//                    
//                    cout << endl;
//                    tokenize();
//                    
//                    string stringToPrint = "";
//                    currToken_index = 0;
//                    
//                    try {
//                        output = parseProgram(stringToPrint);
//                    }
//                    catch (string error)
//                    {
//                        cout << "ERRROR: " << error << endl;
//                    }
//                    
//                    cout << endl;
//                    cout << stringToPrint << endl;
//                    cout << "Output lexeme: " << output.lexeme << endl;
//                    cout << "Output token: " << output.token << endl;
//                    cout << "Output type: " << output.type << endl;
//                    cout << "Output value: " << output.value << endl;
//                    
//                    tokenList.clear();
//                    
//                    //                    Val test;
//                    //                    test.token = "x";
//                    //                    test.lexeme = "var";
//                    //                    test.value = "3";
//                    
//                    //                    var_table_global.add(test);
//                    //                    cout << "table has x: " << var_table_global.has("x") << endl;
//                    //                    cout << "x: " << var_table_global.get("x").value << endl;
//                }
//                else
//                {
//                    cout << "Input Error: token found but end of input expected:  " << userinput[inputleng] << endl;
//                }
//            }
//            
//            //            /************************
//            //             ******* Print Input *******
//            //             *************************/ // you will remove this and instead print the token list
//            //            //userinput[inputleng] = '\0'; // for printing
//            //            // tokenize();
//            //            cout << "Number of Characters read:  " << inputleng << endl;
//            //            cout << "Input:  ";
//            //            for (int i = 0; i <= inputleng;  i++)// mimic input to confirm userinput array is populated.
//            //                cout << userinput[i];
//            //            cout << endl;
//        }
//        
//        
//        
//        return 0;
//        
//    }
//    
//}
//
//
//
//
//
//
//
//#endif /* contingency_h */
