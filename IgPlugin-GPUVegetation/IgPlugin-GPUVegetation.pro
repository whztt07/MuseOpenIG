#-------------------------------------------------
#
# Project created by QtCreator 2015-03-03T17:38:56
#
#-------------------------------------------------

QT       -= core gui

CONFIG += silent

TARGET = IgPlugin-GPUVegetation
TEMPLATE = lib

DEFINES += IGPLUGINGPUVEGETATION_LIBRARY

SOURCES += igplugingpuvegetation.cpp

HEADERS +=

LIBS += -losg -losgDB -losgViewer -lIgPluginCore -lIgCore -lOpenThreads

INCLUDEPATH += ../
DEPENDPATH += ../

unix {
    !mac:contains(QMAKE_HOST.arch, x86_64):{
        DESTDIR = /usr/local/lib64/igplugins
        target.path = /usr/local/lib64/igplugins
    } else {
        DESTDIR = /usr/local/lib/igplugins
        target.path = /usr/local/lib/igplugins
    }
    message($$TARGET Lib will be installed into $$DESTDIR)

    INSTALLS += target

    INCLUDEPATH += /usr/local/include
    DEPENDPATH += /usr/local/include
    INCLUDEPATH += /usr/local/lib64
    DEPENDPATH += /usr/local/lib64

    INCLUDEPATH += /usr/local/lib
    DEPENDPATH += /usr/local/lib

    # library version number files
    exists( "../openig_version.pri" ) {

	include( "../openig_version.pri" )
        isEmpty( VERSION ){ error( "$$TARGET -- bad or undefined VERSION variable inside file openig_version.pri" )
	} else {
        message( "$$TARGET -- Set version info to: $$VERSION" )
	}

    }
    else { error( "$$TARGET -- could not find pri library version file openig_version.pri" ) }

    # end of library version number files
}

OTHER_FILES += \
    Readme.txt

win32-g++:QMAKE_CXXFLAGS += -fpermissive -shared-libgcc -D_GLIBCXX_DLL

win32 {
    OSGROOT = $$(OSG_ROOT)
    isEmpty(OSGROOT) {
        message($$TARGET -- \"OpenSceneGraph\" not detected...)
    }
    else {
        message($$TARGET -- \"OpenSceneGraph\" detected in \"$$OSGROOT\")
        INCLUDEPATH += $$OSGROOT/include
        LIBS += -L$$OSGROOT/lib
    }
    OSGBUILD = $$(OSG_BUILD)
    isEmpty(OSGBUILD) {
        message($$TARGET -- \"OpenSceneGraph build\" not detected...)
    }
    else {
        message($$TARGET -- \"OpenSceneGraph build\" detected in \"$$OSGBUILD\")
        DEPENDPATH += $$OSGBUILD/lib
        INCLUDEPATH += $$OSGBUILD/include
        LIBS += -L$$OSGBUILD/lib
    }

    OPENIGBUILD = $$(OPENIG_BUILD)
    isEmpty (OPENIGBUILD) {
        OPENIGBUILD = $$IN_PWD/..
    }
    DESTDIR = $$OPENIGBUILD/lib
    DLLDESTDIR = $$OPENIGBUILD/bin/igplugins

    LIBS += -L$$OPENIGBUILD/lib
}
