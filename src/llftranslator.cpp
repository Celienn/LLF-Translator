#include "llftranslator.h"

LLFTranslator::LLFTranslator()
{

}

LLFTranslator::~LLFTranslator()
{
    
}

void LLFTranslator::addVariable(const string &var )
{
    variables.push_back(var);
}

void LLFTranslator::addVariable( vector<string> var){
    for (int i = 0; i < var.size(); i++){
        variables.push_back(var[i]);
    }
};