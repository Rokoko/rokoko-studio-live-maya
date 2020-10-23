
add_library(Qt5::QTgaPlugin MODULE IMPORTED)

_populate_Gui_plugin_properties(QTgaPlugin RELEASE "imageformats/qtga.dll")
if(DEFINED MAYA_QT_NDEBUG)
	_populate_Gui_plugin_properties(QTgaPlugin DEBUG "imageformats/qtgad.dll")
endif()

list(APPEND Qt5Gui_PLUGINS Qt5::QTgaPlugin)
