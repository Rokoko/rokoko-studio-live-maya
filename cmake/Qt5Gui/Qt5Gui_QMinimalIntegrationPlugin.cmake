
add_library(Qt5::QMinimalIntegrationPlugin MODULE IMPORTED)

_populate_Gui_plugin_properties(QMinimalIntegrationPlugin RELEASE "platforms/qminimal.dll")
if(DEFINED MAYA_QT_NDEBUG)
	_populate_Gui_plugin_properties(QMinimalIntegrationPlugin DEBUG "platforms/qminimald.dll")
endif()

list(APPEND Qt5Gui_PLUGINS Qt5::QMinimalIntegrationPlugin)
