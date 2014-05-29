#include "WeightCurveViewer.h"
#include "WeightCurve.h"

#include <QQmlEngine>
#include <QQmlContext>
#include <QSettings>

/// This show the window maximized or not as saved last time
void loadWindowSettings(WeightCurveViewer& viewer)
{
    QSettings   settings;

    viewer.setMaximized(settings.value("window/maximized", false).toBool());
    viewer.setPosition(settings.value("window/position", QPoint(-1, -1)).toPoint());
    viewer.setSize(settings.value("window/size", QSize(-1, -1)).toSize());
}

void saveWindowSettings(const WeightCurveViewer& viewer)
{
    QSettings   settings;

    settings.setValue("window/maximized", viewer.isMaximized());
    settings.setValue("window/position", viewer.position());
    settings.setValue("window/size", viewer.size());
}

int main(int argc, char *argv[])
{
    int result;

    Application app(argc, argv);

    QGuiApplication::setApplicationName("Weight Curve");
    QGuiApplication::setApplicationVersion("1.0");
    QGuiApplication::setOrganizationName("Flamaros");
    QGuiApplication::setOrganizationDomain("Flamaros.fr");

    QSettings::setDefaultFormat(QSettings::IniFormat);

    WeightCurveViewer   viewer;

    viewer.qmlEngine().rootContext()->setContextProperty("application", WeightCurve::singleton());
    WeightCurve::singleton()->initialize();

    viewer.setMainQmlFile(QStringLiteral("qml/WeightCurve/main.qml"));

    loadWindowSettings(viewer); // Will show the window in the right mode

    result = app.exec();

    saveWindowSettings(viewer);
    WeightCurve::destroy();
    return result;
}
