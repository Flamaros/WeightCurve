#pragma once

#include "qtquick2controlsapplicationviewer/qtquick2controlsapplicationviewer.h"

class WeightCurveViewer : public QtQuick2ControlsApplicationViewer
{
public:
    void    setMaximized(bool maximized);   /// Force the window to be show (maximized or not)
    bool    isMaximized() const;

    void    setPosition(const QPoint& position);    /// QPoint(-1, -1) Will do nothing
    QPoint  position() const;

    void    setSize(const QSize& size);             /// QSize(-1, -1) Will do nothing
    QSize   size() const;
};
