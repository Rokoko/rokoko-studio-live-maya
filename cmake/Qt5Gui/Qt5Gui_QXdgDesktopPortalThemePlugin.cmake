
add_library(Qt5::QXdgDesktopPortalThemePlugin MODULE IMPORTED)

_populate_Gui_plugin_properties(QXdgDesktopPortalThemePlugin RELEASE "platformthemes/qxdgdesktopportal.dll")
if(DEFINED MAYA_QT_NDEBUG)
	_populate_Gui_plugin_properties(QXdgDesktopPortalThemePlugin DEBUG "platformthemes/qxdgdesktopportald.dll")
endif()

list(APPEND Qt5Gui_PLUGINS Qt5::QXdgDesktopPortalThemePlugin)
