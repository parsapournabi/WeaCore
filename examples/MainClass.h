#ifndef MAINCLASS_H
#define MAINCLASS_H

#include "WeaCore/utils.h"

class MainClass : public QObject
{

        Q_OBJECT
        W_PROP_HDEF(QString, prop1, Prop1, "Property1")
};


#endif // MAINCLASS_H
