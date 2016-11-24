#ifndef COMPONENTDISPLAYERWIDGET_H
#define COMPONENTDISPLAYERWIDGET_H

#include <string>
#include <vector>

#include <QWidget>
#include <QHBoxLayout>

#include "component.h"

#include "qdisplayer.h"

namespace Ui {
class ComponentDisplayerWidget;
}

class ComponentDisplayerWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit ComponentDisplayerWidget(QWidget *parent = 0);
        ~ComponentDisplayerWidget();

        ///set the param list to show in the widget
        void setParametersList(vector <string> inParams);
        void setDesignatorList(vector <string> inList);

    public Q_SLOTS:
        void displayComponent(Component &inComponent);

        void on_previousCompoButton_clicked();
        void on_nextCompoButton_clicked();

    Q_SIGNALS:
        void componentUpdated(string);


    private:
        Ui::ComponentDisplayerWidget *ui;

        vector <string> mParametersList;

        vector <string> mDesList;
        vector <string>::iterator mDesIterator;

        vector <QDisplayer *> mDisplayerList;
};

#endif // COMPONENTDISPLAYERWIDGET_H
