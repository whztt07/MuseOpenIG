#-------------------------------------------------
#
# Project created by QtCreator 2015-03-05T11:20:12
#
#-------------------------------------------------

QT       -= core gui

CONFIG += silent

TARGET = IgPlugin-Lighting
TEMPLATE = lib

DEFINES += IGPLUGINLIGHTING_LIBRARY

SOURCES += igpluginlighting.cpp

HEADERS +=

INCLUDEPATH += ../
DEPENDPATH += ../

LIBS += -losg -losgDB -losgViewer -lOpenThreads -losgShadow -lIgCore -lIgPluginCore -losgUtil

OTHER_FILES += $${PWD}/ConfigFiles/libIgPlugin-Lighting.so.xml

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

    FILE = $${PWD}/ConfigFiles/libIgPlugin-Lighting.so.xml
    DDIR = $${DESTDIR}/libIgPlugin-Lighting.so.xml

    mac:DDIR = $${DESTDIR}/libIgPlugin-Lighting.dylib.xml

    QMAKE_POST_LINK =  test -d $$quote($$DESTDIR) || $$QMAKE_MKDIR $$quote($$DESTDIR) $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += test -e $$quote($$DDIR) || $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)

    #remove the files we manually installed above when we do a make distclean....
    QMAKE_DISTCLEAN += $${DESTDIR}/libIgPlugin-Lighting.*.xml

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

    FILE = $${PWD}/ConfigFiles/libIgPlugin-Lighting.so.xml
    DFILE = $${DLLDESTDIR}/IgPlugin-Lighting.dll.xml

    FILE ~= s,/,\\,g
    DFILE ~= s,/,\\,g

    QMAKE_POST_LINK =  if not exist $$quote($$DLLDESTDIR) $$QMAKE_MKDIR $$quote($$DLLDESTDIR) $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += if not exist $$quote($$DFILE) copy /y $$quote($$FILE) $$quote($$DFILE) $$escape_expand(\\n\\t)

    #remove the files we manually installed above when we do a make distclean....
    QMAKE_DISTCLEAN += $${DLLDESTDIR}/libIgPlugin-Lighting.*.xml

}
