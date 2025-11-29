#pragma once 

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <algorithm> 

namespace JSON {
    struct genericNode; //Need to forward declare these
    struct objectNode;
    struct listNode;

    struct genericNode {
        virtual void print() = 0; 
        virtual std::string toToon(int indent = 0) = 0;
        virtual ~genericNode() = default;
    };

    struct stringNode : public genericNode {
        std::string value;
        stringNode(std::string val) : value(val) {} 

        void print() override { std::cout << "\"" << value  << "\""; }
        std::string toToon(int indent = 0) override { return value; }
    };

    struct numberNode : public genericNode {
        double value;
        numberNode(double val){
            value = val;
        }

        void print() override { std::cout << value; }

        std::string toToon(int indent = 0) override { // Crazy Ai code to remove 0's
            std::string s = std::to_string(value);
            s.erase(s.find_last_not_of('0') + 1, std::string::npos); 
            if (s.back() == '.') s.pop_back();
            return s;
        }
    };

    struct boolNode : public genericNode {
        std::string value;
        boolNode(std::string val){
            value = val;
        }

        void print() override { std::cout << value; }
        std::string toToon(int indent = 0) override { return value; }
    };

    struct objectNode : public genericNode {
        std::vector<std::pair<std::string, genericNode*>> children;

        void addChild(std::string key, genericNode* node) {
            children.push_back({key, node});
        }

        void print() override {
            std::cout << "{ \n";
            for(auto const& [key, node] : children){ 
                std::cout << " " << key << " ";
                node->print();
                std::cout << " \n";
            }
            std::cout << "}";
        }
        std::string toToon(int indent = 0) override; 
    };

    // List Logic
    struct listNode : public genericNode {
        std::vector<genericNode*> children;

        void add(genericNode* newChild) {
            children.push_back(newChild);
        }

        void print() override {
            std::cout << "[ \n";
            for (genericNode* node : children){
                node->print();
                std::cout << ", \n";
            }
            std::cout << "] \n";
        }
        std::string toToon(int indent = 0) override;
    };

    // Object Logic
    inline std::string objectNode::toToon(int indent) {
        std::string output = "";
        std::string spaces(indent, ' ');

        for (auto const& [key, node] : children){
            output += spaces + key;
            if (dynamic_cast<objectNode*>(node) != nullptr){ //no clue
                output += ":\n";
            } else{
                output += ": ";
            }

            output += node->toToon(indent + 2);
            if (output.empty() || output.back() != '\n'){
                output += "\n";
            }
        }
        return output;
    }

    // List Logic
    inline std::string listNode::toToon(int indent) {
        if (children.empty()) return "[]";

        std::string spaces(indent, ' ');
        std::string output = "";
        
        objectNode* firstObj = dynamic_cast<objectNode*>(children[0]);

        if (firstObj){
            output += "[" + std::to_string(children.size()) + "]{";

            std::vector<std::string> keys; 
            for(auto const& [key, val] : firstObj->children) {
                keys.push_back(key);
                output += key + ",";
            }
            if (!keys.empty()) output.pop_back(); 
            output += "}:\n";

            for (genericNode* child : children){
                output += spaces;
                objectNode* obj = static_cast<objectNode*>(child);
                
                for (const std::string& targetKey : keys) {
                    bool found = false;
                    for (auto const& [k, v] : obj->children) {
                        if (k == targetKey) {
                            output += v->toToon(0) + ",";
                            found = true;
                            break;
                        }
                    }
                    if (!found) output += "null,";
                }
                if (output.back() == ',') output.pop_back();
                output += "\n";
            }
        } else { // For simpler versions friends[3]: x, y, z
            output += "[" + std::to_string(children.size()) + "]:\n";
            output += spaces;
            
            for (genericNode* child : children) {
                output += child->toToon(0) + ",";
            }
            if (output.back() == ',') output.pop_back();
        }
        return output;
    }
}