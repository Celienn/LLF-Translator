#ifndef LLFTRANSLATOR_H
#define LLFTRANSLATOR_H

#include <QWidget>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui {
    class LLFTranslator;
}
QT_END_NAMESPACE

using namespace std;

class LLFTranslator : public QWidget
{
    Q_OBJECT

    public:
        LLFTranslator(QWidget *parent = nullptr);
        ~LLFTranslator();
        void addVariable(const string &var);
        void addVariable(const vector<string> var);

    private:
        Ui::LLFTranslator *ui;
        vector<string> variables;
};
#endif // LLFTRANSLATOR_H
