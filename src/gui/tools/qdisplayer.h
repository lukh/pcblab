#ifndef QDISPLAYER_H
#define QDISPLAYER_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QHBoxLayout>


#include "pcblab/common.h"

/// The QDisplayer allows to show a couple label/value
class QDisplayer : public QWidget
{
    Q_OBJECT
    public:
        QDisplayer(QWidget *parent = 0): QWidget(parent) {
            mName = new QLabel();
            mValue = new QLabel();

            QVBoxLayout *layout = new QVBoxLayout;
            layout->addWidget(mName);
            layout->addWidget(mValue);

            setLayout(layout);

            mName->setStyleSheet("font-weight: bold;");

            setMinimumHeight(80);
        }

        QDisplayer(QString inName, QWidget *parent = 0): QDisplayer(parent) {
            mName->setText(inName);
        }

        virtual ~QDisplayer(){
            delete mName;
            delete mValue;
        }



        void setName(QString inName) { mName->setText(inName); }

        void update(double inValue){
            mValue->setText(QString::number(inValue));
        }

        void update(int inValue){
            mValue->setText(QString::number(inValue));
        }

        void update(QString inStr){ mValue->setText(inStr); }

        void update(plPoint inValue){
            QString text;
            text += "(" + QString::number(inValue.mX) + ", " + QString::number(inValue.mY) + ")";
            mValue->setText(text);
        }

    private:
        QLabel *mName;
        QLabel *mValue;

};

#endif // QDISPLAYER_H
