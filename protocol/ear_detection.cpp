#include <protocol/ear_detection.h>

#include <QtGlobal>

namespace Protocol {
    EarDetectionApi::EarDetectionApi(QObject *parent)
        : QObject(parent)
    {}

    EarDetectionApi *EarDetectionApi::instance()
    {
        static EarDetectionApi api;
        return &api;
    }

    void EarDetectionApi::updateEarDetection(EarComponentGeneric component, EarStatusGeneric status)
    {
        emit earDetectionUpdated(component, status);
    }
}