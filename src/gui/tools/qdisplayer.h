#ifndef QDISPLAYER_H
#define QDISPLAYER_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpacerItem>

#include "pcblab/common.h"

/// The QDisplayer allows to show a couple label/value
class QDisplayer : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString value READ getValue WRITE update)

    public:
        QDisplayer(QWidget *parent = 0): QWidget(parent) {
            mName = new QLabel();
            mValue = new QLabel();

            QVBoxLayout *layout = new QVBoxLayout;
            layout->addWidget(mName);
            layout->addWidget(mValue);

            setLayout(layout);

            mName->setStyleSheet("font-weight: bold;");
        }

        QDisplayer(QString inName, QWidget *parent = 0): QDisplayer(parent) {
            mName->setText(inName);
        }

        virtual ~QDisplayer(){
            delete mName;
            delete mValue;
        }



        void setName(QString inName) { mName->setText(inName); }


        void update(QString inStr){ mValue->setText(inStr); }
        QString getValue() const { return mValue->text(); }


    private:
        QLabel *mName;
        QLabel *mValue;

};

#endif // QDISPLAYER_H
