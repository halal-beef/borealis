#ifndef PROTOCOL_EAR_DETECTION_H
#define PROTOCOL_EAR_DETECTION_H

#include <QObject>

enum class EarComponentGeneric {
    Left = 0,
    Right = 1
};

enum class EarStatusGeneric {
    EarIn = 0,
    EarOut = 1,
    InCase = 2,
    Disconnected = 3,
};

namespace Protocol {
    class EarDetectionApi : public QObject
    {
        Q_OBJECT

    public:
        static EarDetectionApi *instance();

        void updateEarDetection(EarComponentGeneric component, EarStatusGeneric status);

    signals:
        void earDetectionUpdated(EarComponentGeneric component, EarStatusGeneric status);

    private:
        explicit EarDetectionApi(QObject *parent = nullptr);
    };
}

#endif // PROTOCOL_EAR_DETECTION_H
