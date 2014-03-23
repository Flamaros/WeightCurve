#include "qtquick2controlsapplicationviewer/qtquick2controlsapplicationviewer.h"

#include "WeightCurve.h"

#include <QQmlEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    int result;

    Application app(argc, argv);

    QGuiApplication::setApplicationName("Weight Curve");
    QGuiApplication::setApplicationVersion("1.0");
//    QGuiApplication::setOrganizationName("");
//    QGuiApplication::setOrganizationDomain("");

    QtQuick2ControlsApplicationViewer   viewer;

    viewer.qmlEngine().rootContext()->setContextProperty("application", WeightCurve::singleton());
    WeightCurve::singleton()->initialize();

    viewer.setMainQmlFile(QStringLiteral("qml/WeightCurve/main.qml"));

    viewer.show();

    result = app.exec();

    WeightCurve::destroy();
    return result;
}
