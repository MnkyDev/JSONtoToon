#include <iostream>
#include <fstream>

#include "fileReader.hpp"
#include "jsonClasses.hpp"
#include "lexer.hpp"
#include "parser.hpp"

using namespace std;

int main(int argc, char* argv[]){

    programConfig config = readFile(argc, argv);   

    if (!config.error.empty()){
        cerr << "Error whilst creating Program Configuration {" << config.error << "} Please try again";
        return 0;
    } 

    cout << "Program Configuration Built Succesfully" << endl;

    lexer Lexer;
    vector<Token> tokens = Lexer.tokenise(config.initData);

    cout << "Lexical Anaysis Complete, Processed (" << tokens.size() << ") Tokens" << endl;
    
    parser Parser(tokens); 
    JSON::genericNode* root = Parser.parse(); // Start parsing from the root

    if (root){
        cerr << "Tree Build Sucesfully" << endl;
    } else {
        cerr << "An error occured whilst creating the tree";
    }
    ofstream output(config.outputFile);
    output << root->toToon(0) << endl;
    output.close();
    return 0;
}

