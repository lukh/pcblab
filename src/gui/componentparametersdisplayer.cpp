#include "componentparametersdisplayer.h"

ComponentParametersDisplayer::ComponentParametersDisplayer(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    setMinimumHeight(80);
}

ComponentParametersDisplayer::~ComponentParametersDisplayer()
{
}

void ComponentParametersDisplayer::clear()
{
    for(int i = 0; i < mParamsList.size(); ++i){
        delete mParamsList.at(i);
    }

    delete layout();
    QHBoxLayout *layout = new QHBoxLayout(this);

    mParamsList.clear();
}

void ComponentParametersDisplayer::add(const string &inName, const string &inValue)
{
    QDisplayer *disp = new QDisplayer(QString::fromStdString(inName));
    disp->update(QString::fromStdString(inValue));
    mParamsList.append(disp);

    this->layout()->addWidget(disp);
}

void ComponentParametersDisplayer::add(const Component::Parameters &inParameters)
{
    for(Component::Parameters::const_iterator it = inParameters.begin(); it != inParameters.end(); ++it){
        add(it->first, it->second);
    }
}
