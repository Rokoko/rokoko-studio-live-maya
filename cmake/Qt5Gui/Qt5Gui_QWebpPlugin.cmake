
add_library(Qt5::QWebpPlugin MODULE IMPORTED)

_populate_Gui_plugin_properties(QWebpPlugin RELEASE "imageformats/qwebp.dll")
if(DEFINED MAYA_QT_NDEBUG)
	_populate_Gui_plugin_properties(QWebpPlugin DEBUG "imageformats/qwebpd.dll")
endif()

list(APPEND Qt5Gui_PLUGINS Qt5::QWebpPlugin)
