#ifndef COMPONENTDISPLAYERWIDGET_H
#define COMPONENTDISPLAYERWIDGET_H

#include <string>
#include <vector>

#include <QWidget>
#include <QHBoxLayout>
#include <QDialog>
#include <QTableView>
#include <QDataWidgetMapper>

#include "component.h"

#include "componentsmodelwrapper.h"

#include "qdisplayer.h"



/// a widget dedicated to displaying the component list
class DesListDialog: public QDialog{
    Q_OBJECT

    public:
        DesListDialog(QWidget *parent = 0): QDialog(parent) {
            setWindowTitle("Designators List");
            setMinimumWidth(400);
            setMinimumHeight(600);

            mView = new QTableView();

            QHBoxLayout *layout = new QHBoxLayout(this);
            layout->addWidget(mView);


            connect(mView, SIGNAL(clicked(const QModelIndex &)), this, SIGNAL(viewClicked(const QModelIndex &)));
        }


        ~DesListDialog(){
            delete mView;
        }



        void setModel(QAbstractTableModel *inModel) {
            mView->setModel(inModel);
            mView->resizeRowsToContents();
            mView->resizeColumnsToContents();
        }


    public Q_SLOTS:
        void setCurrentIndex(const QModelIndex &inMI){
            mView->setCurrentIndex(inMI);
        }


    Q_SIGNALS:
        void viewClicked(const QModelIndex &inIndex);



    private:
        QTableView *mView;
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


        void setModel(ComponentModelWrapper *inModel);

        void setParameters(vector <string> inParameters);


    public Q_SLOTS:
        void on_previousCompoButton_clicked();
        void on_nextCompoButton_clicked();
        void on_showListButton_clicked();

        void on_itemSelectedInList(const QModelIndex &inIndex);

    Q_SIGNALS:
        void componentUpdated(string);


    private:
        void updateDisplayers();
        void notify();

    private:
        Ui::ComponentDisplayerWidget *ui;

        vector <QDisplayer *> mDisplayerList;

        DesListDialog *mDesListDialog;

        /// parameters used by the view
        vector <string> mParameters;

        QDataWidgetMapper mMapper;
};

#endif // COMPONENTDISPLAYERWIDGET_H
