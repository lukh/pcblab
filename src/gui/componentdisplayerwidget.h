#ifndef COMPONENTDISPLAYERWIDGET_H
#define COMPONENTDISPLAYERWIDGET_H

#include <string>
#include <vector>

#include <QWidget>

#include "component.h"

namespace Ui {
class ComponentDisplayerWidget;
}

class ComponentDisplayerWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit ComponentDisplayerWidget(QWidget *parent = 0);
        ~ComponentDisplayerWidget();

        void setDesignatorList(vector <string> inList);

    public Q_SLOTS:
        void displayComponent(Component &inComponent);

        void on_previousCompoButton_clicked();
        void on_nextCompoButton_clicked();

    Q_SIGNALS:
        void componentUpdated(string);


    private:
        Ui::ComponentDisplayerWidget *ui;

        vector <string> mDesList;
        vector <string>::iterator mDesIterator;
};

#endif // COMPONENTDISPLAYERWIDGET_H
