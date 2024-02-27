#include "llftranslator.h"
#include "ui_llftranslator.h"

LLFTranslator::LLFTranslator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LLFTranslator)
{
    ui->setupUi(this);
}

LLFTranslator::~LLFTranslator()
{
    delete ui;
}

void LLFTranslator::addVariable( string &var )
{
    variables.push_back(var);
}

void addVariable(const vector<string> var){
    for (int i = 0; i < var.size(); i++){
        variables.push_back(var[i]);
    }
};

//test