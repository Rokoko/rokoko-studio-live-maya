
add_library(Qt5::QWbmpPlugin MODULE IMPORTED)

_populate_Gui_plugin_properties(QWbmpPlugin RELEASE "imageformats/qwbmp.dll")
if(DEFINED MAYA_QT_NDEBUG)
	_populate_Gui_plugin_properties(QWbmpPlugin DEBUG "imageformats/qwbmpd.dll")
endif()

list(APPEND Qt5Gui_PLUGINS Qt5::QWbmpPlugin)
