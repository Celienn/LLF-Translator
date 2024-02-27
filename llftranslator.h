#ifndef LLFTRANSLATOR_H
#define LLFTRANSLATOR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class LLFTranslator;
}
QT_END_NAMESPACE

class LLFTranslator : public QWidget
{
    Q_OBJECT

public:
    LLFTranslator(QWidget *parent = nullptr);
    ~LLFTranslator();

private:
    Ui::LLFTranslator *ui;
};
#endif // LLFTRANSLATOR_H
