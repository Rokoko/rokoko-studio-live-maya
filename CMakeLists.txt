cmake_minimum_required(VERSION 2.8)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(MAYA_QT_NDEBUG)

set(CMAKE_PREFIX_PATH $ENV{MAYA_LOCATION})
set(Qt5_DIR cmake/Qt5)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

# include the project setting file
# NOTE: intended that devkit root is 'PATH/TO/MAYA/devkit'
include($ENV{MAYA_LOCATION}/devkit/cmake/pluginEntry.cmake)

# specify project name
set(PROJECT_NAME rslm)

set(LIBRARY_DIRS $ENV{MAYA_LOCATION}/lib)

set(HEADERS
    src/singleton.h
    src/utils.h
    src/recorder.h
    src/receiverworker.h
    src/mapping.h
    src/animations.h
    src/constants.h
    src/ui/button.h
    src/ui/categoryheader.h
    src/ui/commandapicontent.h
    src/ui/infocontent.h
    src/ui/receivercontent.h
    src/ui/recordbutton.h
    src/ui/rootwidget.h
    src/ui/updatercontent.h
    thirdparty/lz4/lz4.h
    thirdparty/lz4/lz4hc.h
    thirdparty/lz4/lz4frame.h
    thirdparty/lz4/lz4frame_static.h
    thirdparty/lz4/xxhash.h
)

# set SOURCE_FILES
set(SOURCE_FILES ${HEADERS}
    src/animations.cpp
    src/main.cpp
    src/mapping.cpp
    src/receiverworker.cpp
    src/recorder.cpp
    src/utils.cpp
    src/ui/button.cpp
    src/ui/categoryheader.cpp
    src/ui/commandapicontent.cpp
    src/ui/infocontent.cpp
    src/ui/receivercontent.cpp
    src/ui/recordbutton.cpp
    src/ui/rootwidget.cpp
    src/ui/updatercontent.cpp
    thirdparty/lz4/lz4.c
    thirdparty/lz4/lz4hc.c
    thirdparty/lz4/xxhash.c
    thirdparty/lz4/lz4frame.c
    resources.qrc
)

set(INCLUDE_DIRS
    src
    src/ui
    thirdparty/lz4
    $ENV{MAYA_LOCATION}/include/QtCore
    $ENV{MAYA_LOCATION}/include/QtGui
    $ENV{MAYA_LOCATION}/include/QtWidgets
    $ENV{MAYA_LOCATION}/include/QtNetwork
)
# set linking libraries
set(LIBRARIES OpenMaya OpenMayaUI OpenMayaAnim Foundation)

build_plugin()



target_link_libraries(rslm Qt5::Widgets Qt5::Network)
