#ifndef COMPONENTPARAMETERSDISPLAYER_H
#define COMPONENTPARAMETERSDISPLAYER_H

#include <string>
#include <QWidget>
#include <QHBoxLayout>

#include "tools/qdisplayer.h"

#include "component.h"

class ComponentParametersDisplayer : public QWidget
{
    Q_OBJECT

    public:
        explicit ComponentParametersDisplayer(QWidget *parent = 0);
        ~ComponentParametersDisplayer();

        void clear();
        void add(const string &inName, const string &inValue);
        void add(const Component::Parameters &inParameters);

    private:
        QList<QDisplayer *> mParamsList;

};

#endif // COMPONENTPARAMETERSDISPLAYER_H
