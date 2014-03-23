# Add more folders to ship with the application, here
folder_01.source = qml/WeightCurve
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

QT += sql

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
	main.cpp \
	WeightCurve.cpp

HEADERS += \
	WeightCurve.h


# Installation path
# target.path =

!win32-msvc2010 {
QMAKE_CXXFLAGS += -std=gnu++0x
}

win32 {
RC_FILE = WeightCurve.rc
}

# Please do not modify the following two lines. Required for deployment.
include(qtquick2controlsapplicationviewer/qtquick2controlsapplicationviewer.pri)
qtcAddDeployment()