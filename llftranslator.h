#ifndef LLFTRANSLATOR_H
#define LLFTRANSLATOR_H

#include <QWidget>
#include <string>
// #include <SimConnect.h>

using namespace std;

class LLFTranslator
{

    public:
        LLFTranslator();
        ~LLFTranslator();
        void addVariable(const string &var);
        void addVariable(const vector<string> var);

    private:
        vector<string> variables;
};
#endif // LLFTRANSLATOR_H