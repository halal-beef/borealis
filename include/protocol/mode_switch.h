#ifndef PROTOCOL_MODE_SWITCH_H
#define PROTOCOL_MODE_SWITCH_H

#include <QObject>
#include <QList>
#include <QPair>
#include <QString>

namespace Protocol {
    class ModeSwitchApi : public QObject
    {
        Q_OBJECT

    public:
        using ModeTuple = QPair<int, QString>;
        using ModeTupleList = QList<ModeTuple>;

        static ModeSwitchApi *instance();

        const ModeTupleList &modeSwitches() const;
        void advertiseModes(const ModeTupleList &modes);
        void updateModeSwitch(const int mode);

    signals:
        void modesAdvertised(const ModeTupleList &modes);
        void modeUpdated(const int mode);

    private:
        explicit ModeSwitchApi(QObject *parent = nullptr);

        ModeTupleList m_modes;
    };
}

#endif // PROTOCOL_MODE_SWITCH_H