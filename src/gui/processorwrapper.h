#ifndef PROCESSORWRAPPER_H
#define PROCESSORWRAPPER_H

#include <QThread>
#include "pcblab/viewprocessor/viewprocessor.h"

class ProcessorWrapper
{
    public:
        ProcessorWrapper(ViewProcessor &inProcessor): mProcessor(inProcessor) {}

    protected:
        virtual void run(){
            mProcessor.update();
        }

    private:
        ViewProcessor &mProcessor;
};

#endif // PROCESSORWRAPPER_H
