
add_library(Qt5::QWindowsVistaStylePlugin MODULE IMPORTED)

_populate_Widgets_plugin_properties(QWindowsVistaStylePlugin RELEASE "styles/qwindowsvistastyle.dll")
if(DEFINED MAYA_QT_NDEBUG)
	_populate_Widgets_plugin_properties(QWindowsVistaStylePlugin DEBUG "styles/qwindowsvistastyled.dll")
endif()

list(APPEND Qt5Widgets_PLUGINS Qt5::QWindowsVistaStylePlugin)
