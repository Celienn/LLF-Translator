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
