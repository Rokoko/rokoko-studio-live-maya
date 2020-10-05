
add_library(Qt5::QTuioTouchPlugin MODULE IMPORTED)

_populate_Gui_plugin_properties(QTuioTouchPlugin RELEASE "generic/qtuiotouchplugin.dll")
if(DEFINED MAYA_QT_NDEBUG)
	_populate_Gui_plugin_properties(QTuioTouchPlugin DEBUG "generic/qtuiotouchplugind.dll")
endif()

list(APPEND Qt5Gui_PLUGINS Qt5::QTuioTouchPlugin)
