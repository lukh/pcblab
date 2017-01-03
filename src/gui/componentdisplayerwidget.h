#ifndef COMPONENTDISPLAYERWIDGET_H
#define COMPONENTDISPLAYERWIDGET_H

#include <string>
#include <vector>

#include <QWidget>
#include <QHBoxLayout>
#include <QTreeView>
#include <QHeaderView>
#include <QDataWidgetMapper>

#include "component.h"

#include "componentsmodelwrapper.h"

#include "qdisplayer.h"



/// a widget dedicated to displaying the component list
class DesListWidget: public QWidget{
    Q_OBJECT

    public:
        DesListWidget(QWidget *parent = 0): QWidget(parent) {
            //setMinimumWidth(190);
            //setMinimumHeight(200);

            mView = new QTreeView();
            mView->setExpandsOnDoubleClick(false);

            QHBoxLayout *layout = new QHBoxLayout(this);
            layout->addWidget(mView);


            connect(mView, SIGNAL(clicked(const QModelIndex &)), this, SIGNAL(viewClicked(const QModelIndex &)));
        }


        ~DesListWidget(){
            delete mView;
        }



        void setModel(QAbstractItemModel *inModel) {
            mView->setModel(inModel);
        }


    public Q_SLOTS:
        void setCurrentIndex(const QModelIndex &inMI){
            mView->setCurrentIndex(inMI);
        }


    Q_SIGNALS:
        void viewClicked(const QModelIndex &inIndex);



    private:
        QTreeView *mView;
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

        void setComponentByDes(string inDes);



        DesListWidget *getDesignatorListWidget() { return mDesListWidget; }

    public Q_SLOTS:
        void on_previousCompoButton_clicked();
        void on_nextCompoButton_clicked();

        void on_itemSelectedInTree(const QModelIndex &inIndex);

    Q_SIGNALS:
        void componentUpdated(string);
        void netUpdated(string);


    private:
        void updateDisplayers();
        void notify();

    private:
        Ui::ComponentDisplayerWidget *ui;

        vector <QDisplayer *> mDisplayerList;

        DesListWidget *mDesListWidget;

        /// parameters used by the view
        vector <string> mParameters;

        QDataWidgetMapper mMapper;
};

#endif // COMPONENTDISPLAYERWIDGET_H
