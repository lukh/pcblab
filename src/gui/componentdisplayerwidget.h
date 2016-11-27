#ifndef COMPONENTDISPLAYERWIDGET_H
#define COMPONENTDISPLAYERWIDGET_H

#include <string>
#include <vector>

#include <QWidget>
#include <QHBoxLayout>
#include <QDialog>
#include <QListWidget>

#include "component.h"

#include "qdisplayer.h"



class DesListDialog: public QDialog{
    Q_OBJECT

    public:
        DesListDialog(QWidget *parent = 0): QDialog(parent) {
            setWindowTitle("Designators List");
            setMinimumWidth(200);
            setMinimumHeight(100);

            mList = new QListWidget();
            QObject::connect(mList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(selectItem(QListWidgetItem*)));


            QHBoxLayout *layout = new QHBoxLayout(this);
            layout->addWidget(mList);
        }


        ~DesListDialog(){
            delete mList;
        }

        void setCurrent(const string &inDes){
            QString des = QString::fromStdString(inDes);

            QList<QListWidgetItem *> res = mList->findItems(des, Qt::MatchExactly);
            if(res.size() == 1){
                mList->setCurrentItem(res[0]);
            }
        }

        void update(const vector<string> &inList){
            mList->clear();
            for(vector<string>::const_iterator it = inList.begin(); it != inList.end(); ++it){
                mList->addItem(QString::fromStdString(*it));
            }
        }

    public Q_SLOTS:
        void selectItem(QListWidgetItem *it){
            Q_EMIT(itemSelected(it->text().toStdString()));
        }

    Q_SIGNALS:
        void itemSelected(string);

    private:
        QListWidget *mList;
};


namespace Ui {
class ComponentDisplayerWidget;
}


/// Display component informations
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
        void on_showListButton_clicked();

        void on_itemSelectedInList(string inDes);

    Q_SIGNALS:
        void componentUpdated(string);


    private:
        Ui::ComponentDisplayerWidget *ui;

        vector <string> mParametersList;

        vector <string> mDesList;
        vector <string>::iterator mDesIterator;

        vector <QDisplayer *> mDisplayerList;

        DesListDialog *mDesListDialog;
};

#endif // COMPONENTDISPLAYERWIDGET_H
