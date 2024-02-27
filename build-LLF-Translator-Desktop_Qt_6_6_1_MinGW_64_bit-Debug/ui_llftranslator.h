/********************************************************************************
** Form generated from reading UI file 'llftranslator.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LLFTRANSLATOR_H
#define UI_LLFTRANSLATOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LLFTranslator
{
public:

    void setupUi(QWidget *LLFTranslator)
    {
        if (LLFTranslator->objectName().isEmpty())
            LLFTranslator->setObjectName("LLFTranslator");
        LLFTranslator->resize(800, 600);

        retranslateUi(LLFTranslator);

        QMetaObject::connectSlotsByName(LLFTranslator);
    } // setupUi

    void retranslateUi(QWidget *LLFTranslator)
    {
        LLFTranslator->setWindowTitle(QCoreApplication::translate("LLFTranslator", "LLFTranslator", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LLFTranslator: public Ui_LLFTranslator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LLFTRANSLATOR_H
