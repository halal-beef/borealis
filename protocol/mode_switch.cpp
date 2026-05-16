#include <protocol/mode_switch.h>

namespace Protocol {
    ModeSwitchApi::ModeSwitchApi(QObject *parent)
        : QObject(parent)
    {}

    ModeSwitchApi *ModeSwitchApi::instance()
    {
        static ModeSwitchApi api;
        return &api;
    }

    const ModeSwitchApi::ModeTupleList &ModeSwitchApi::modeSwitches() const
    {
        return m_modes;
    }

    void ModeSwitchApi::advertiseModes(const ModeTupleList &modes)
    {
        if (m_modes == modes)
            return;

        m_modes = modes;
        emit modesAdvertised(m_modes);
    }

    void ModeSwitchApi::updateModeSwitch(const int mode)
    {
        emit modeUpdated(mode);
    }
}