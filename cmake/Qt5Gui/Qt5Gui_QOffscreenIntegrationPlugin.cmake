
add_library(Qt5::QOffscreenIntegrationPlugin MODULE IMPORTED)

_populate_Gui_plugin_properties(QOffscreenIntegrationPlugin RELEASE "platforms/qoffscreen.dll")
if(DEFINED MAYA_QT_NDEBUG)
	_populate_Gui_plugin_properties(QOffscreenIntegrationPlugin DEBUG "platforms/qoffscreend.dll")
endif()

list(APPEND Qt5Gui_PLUGINS Qt5::QOffscreenIntegrationPlugin)
