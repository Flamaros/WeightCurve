// checksum 0xad2d version 0x90001
/*
  This file was generated by the Qt Quick 2 Application wizard of Qt Creator.
  QtQuick2ControlsApplicationViewer is a convenience class.
  QML paths are handled here.
  It is recommended not to modify this file, since newer versions of Qt Creator
  may offer an updated version of it.
*/
#ifndef QTQUICK2APPLICATIONVIEWER_H
#define QTQUICK2APPLICATIONVIEWER_H

#ifndef QT_NO_WIDGETS
#include <QApplication>
#else
#include <QGuiApplication>
#endif

QT_BEGIN_NAMESPACE

#ifndef QT_NO_WIDGETS
#define Application QApplication
#else
#define Application QGuiApplication
#endif

QT_END_NAMESPACE

// Personnal adds
class QQmlEngine;

class QtQuick2ControlsApplicationViewer
{
public:
    explicit QtQuick2ControlsApplicationViewer();
    virtual ~QtQuick2ControlsApplicationViewer();

    void setMainQmlFile(const QString &file);
    void addImportPath(const QString &path);
    void show();

    // Personnal adds
    QQmlEngine& qmlEngine();

protected:
    class QtQuick2ApplicationViewerPrivate *d;
};

#endif // QTQUICK2APPLICATIONVIEWER_H
