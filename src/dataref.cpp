#include "dataref.h"

Dataref::Dataref(QString name, int frequency, double value)
{
    this->name = name;
    this->frequency = frequency;
    this->value = value;
}

Dataref::~Dataref()
{
    
}