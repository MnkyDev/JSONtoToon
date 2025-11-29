#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

enum tokenTypes{ // Will get this working w/ american syntax one day    
    OPEN_CURL,
    CLOSE_CURL,
    OPEN_SQUARE,
    CLOSE_SQUARE,
    COLON,
    COMMA,
    STRING,
    NUMBER,
    BOOLEAN,
    UNKNOWN
};

struct Token{
    tokenTypes type;
    string value;

    void print(){ 
        switch (type){
            case OPEN_CURL: cout << "TOKEN: { " << endl; break;
            case CLOSE_CURL: cout << "TOKEN: } " << endl; break;
            case OPEN_SQUARE: cout << "TOKEN: [ " << endl; break;
            case CLOSE_SQUARE: cout << "TOKEN: ] " << endl; break;
            case COLON: cout << "TOKEN: : " << endl; break;
            case COMMA: cout << "TOKEN , " << endl; break;
            case STRING: cout << "TOKEN: STR - " << value << endl; break;
            case NUMBER: cout << "TOKEN: NUM - " << value << endl; break;
            case UNKNOWN: cout << "TOKEN: UNKNOWN - " << value << endl; break;
        }
    }
};

class lexer{
    public:
    vector<Token> tokenise(string textData){
        vector<Token> tokens;
        int index = 0;

        while (index < textData.length()){
            char currentCharacter = textData[index];

            if (isspace(currentCharacter)){ //Dont need whitespace in Lexical Analysis
                index++; 
                continue;
            }

            if (currentCharacter == '{'){
                tokens.push_back({OPEN_CURL, "{"});
                index++;
            } else if (currentCharacter == '}'){
                tokens.push_back({CLOSE_CURL, "}"});
                index++;
            } else if (currentCharacter == '['){
                tokens.push_back({OPEN_SQUARE, "["});
                index++;
            } else if (currentCharacter == ']'){
                tokens.push_back({CLOSE_SQUARE, "]"});
                index++;
            } else if (currentCharacter == ':'){
                tokens.push_back({COLON, ":"});
                index++;
            } else if (currentCharacter == ','){
                tokens.push_back({COMMA, ","});
                index++;
            }

            else if (currentCharacter == '"'){ //Bri'ish only 
                string currentVal = "";
                index++;

                while (index < textData.length() && textData[index] != '"'){
                    currentVal += textData[index];
                    index++;
                }

                index++;
                tokens.push_back({STRING, currentVal});
            }

            else if (isdigit(currentCharacter)){
                string currentVal = "";

                while (index < textData.length() && (isdigit(textData[index]) || textData[index] == '.')){
                    currentVal += textData[index];
                    index++;
                }

                tokens.push_back({NUMBER, currentVal});
            }

            else if (isalpha(currentCharacter)){
                string word = "";

                while (index < textData.length() && isalpha(textData[index])){
                    word += textData[index];
                    index++;
                }
                
                if (word == "true" || word == "false"){
                    tokens.push_back({BOOLEAN, word});
                } else{
                    cerr << "Unknown Token " << word;
                }
            }

            else{
                cerr << "Unknown Token " << currentCharacter;
                index++;
            }
        }
        return tokens;
    }
};