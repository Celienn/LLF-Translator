#include "globaldata.h"
#include <qdebug.h>

GlobalData::GlobalData(){
    /*double var = 0.152;

    hash["oui"] = &var;
    qDebug() << *hash["oui"];*/
}


void GlobalData::trackVariable(QString key, double* value){

    hash[key] = value;

}


void GlobalData::unTrackVariable(QString key){

    if (hash.contains(key)) hash.remove(key);

}

double GlobalData::getValue(QString key){

    return (hash.contains(key) ? *(hash[key]) : 0);

}
