#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include "jsonClasses.hpp"
#include "lexer.hpp"

using namespace std;

struct programConfig{
    string inputFile;
    string outputFile {"output.toon"}; 
    string initData;
    string error; // Allows us to display later
};

programConfig readFile(int argc, char* argv[]){
    programConfig config;

    string data;   
    for (int i = 1; i < argc; i++){
        string currentArg = argv[i];

        if ((currentArg == "-o" || currentArg == "-O")){
            if (i + 1 < argc){ // has to be -o fileName, cannot do -o for default
                config.outputFile = argv[i + 1];
                i++; 
            } else {
                config.error = "-o Requires exactly (1) argument";
                return config;
            }
        } else { // Assume input file
            config.inputFile = currentArg;
        }
    }   

    //Read data first 
    ifstream readFile(config.inputFile);
    stringstream buffer;

    if (!readFile.is_open()){
        config.error = "File cannot be opened";
        return config;
    }  

    buffer << readFile.rdbuf();
    config.initData = buffer.str();
    readFile.close();

    return config;
}