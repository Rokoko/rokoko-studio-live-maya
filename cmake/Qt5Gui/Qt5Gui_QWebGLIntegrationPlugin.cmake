
add_library(Qt5::QWebGLIntegrationPlugin MODULE IMPORTED)

_populate_Gui_plugin_properties(QWebGLIntegrationPlugin RELEASE "platforms/qwebgl.dll")
if(DEFINED MAYA_QT_NDEBUG)
	_populate_Gui_plugin_properties(QWebGLIntegrationPlugin DEBUG "platforms/qwebgld.dll")
endif()

list(APPEND Qt5Gui_PLUGINS Qt5::QWebGLIntegrationPlugin)
