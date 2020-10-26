
add_library(Qt5::QICOPlugin MODULE IMPORTED)

_populate_Gui_plugin_properties(QICOPlugin RELEASE "imageformats/qico.dll")
if(DEFINED MAYA_QT_NDEBUG)
	_populate_Gui_plugin_properties(QICOPlugin DEBUG "imageformats/qicod.dll")
endif()

list(APPEND Qt5Gui_PLUGINS Qt5::QICOPlugin)
