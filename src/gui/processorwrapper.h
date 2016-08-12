#ifndef PROCESSORWRAPPER_H
#define PROCESSORWRAPPER_H

#include <QThread>
#include "pcblab/viewprocessor/viewprocessor.h"

class ProcessorWrapper: public QThread
{
    public:
        ProcessorWrapper(ViewProcessor &inProcessor): QThread(), mProcessor(inProcessor) {}

    protected:
        virtual void run(){
            mProcessor.update();
        }

    private:
        ViewProcessor &mProcessor;
};

#endif // PROCESSORWRAPPER_H
