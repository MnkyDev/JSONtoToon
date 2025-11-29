#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "jsonClasses.hpp"
#include "lexer.hpp"

using namespace std;

class parser{
    private:
    vector<Token> tokenCopies;
    int pointerIndex = 0;

    Token peek(){ // View the "current" token (No eating)
        if (pointerIndex >= tokenCopies.size()){
            return {UNKNOWN, ""}; // Simplest way to avoid errors
        }
        return tokenCopies[pointerIndex];
    }

    Token nextToken(){ // "eat" the current token, move onto the next one
        if (pointerIndex >= tokenCopies.size()){
            return {UNKNOWN, ""}; 
        }

        Token currentToken = tokenCopies[pointerIndex];
        pointerIndex++;
        return currentToken;
    }

    JSON::genericNode* parseObject(){
        nextToken();

        JSON::objectNode* newNode = new JSON::objectNode();

        while (peek().type != CLOSE_CURL && peek().type != UNKNOWN){
            if (peek().type != STRING){ // Can do this as key is always str in JSON
                cerr << "Parsing Error - Key must be of type (string)" << endl;
                return nullptr;
            }
            string key = nextToken().value;

            if (peek().type != COLON){
                cerr << "Parsing Error - Keys and Values must be seperated by ':'" << endl;
                return nullptr;
            }
            nextToken();

            JSON::genericNode* value = parseElement();

            if (value == nullptr){
                cerr << "Parsing Error - Could not find a value for the key " << key << endl;
                return nullptr;
            }

            newNode->addChild(key, value);

            if (peek().type == COMMA){
                nextToken();
            }
        }

        nextToken();

        return newNode;
    }

    JSON::genericNode* parseList(){
        nextToken(); // "Eat" the opening [

        JSON::listNode* newNode = new JSON::listNode();

        while(peek().type != CLOSE_SQUARE && peek().type != UNKNOWN){
            JSON::genericNode* value = parseElement();

            if (value == nullptr){
                cerr << "Parsing Error - Expected a value inside of the table";
                return nullptr;
            }
            newNode->add(value);

            if (peek().type == COMMA){
                nextToken();
            } else if(peek().type != CLOSE_SQUARE){
                cerr << "Parsing Error - Expected ] after table";
                return nullptr;
            }
        }

        nextToken(); // "Eat" the final ]
        return newNode;
    }

    public:
    parser(vector<Token> inputTokens){
        tokenCopies = inputTokens;
        pointerIndex = 0;
    }

    JSON::genericNode* parse() {
        return parseElement();
    }

    JSON::genericNode* parseElement(){
        Token currentNode = peek();

        if (currentNode.type == STRING){
            nextToken();
            return new JSON::stringNode(currentNode.value);
        }

        else if (currentNode.type == NUMBER){
            nextToken();
            return new JSON::numberNode(stod(currentNode.value));
        }

        else if (currentNode.type == BOOLEAN){
            nextToken();
            return new JSON::boolNode(currentNode.value);
        }

        else if (currentNode.type == OPEN_CURL){
            return parseObject();
        }

        else if (currentNode.type == OPEN_SQUARE){
            return parseList();
        }

        return nullptr;
    }
};