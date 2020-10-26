
add_library(Qt5::QTiffPlugin MODULE IMPORTED)

_populate_Gui_plugin_properties(QTiffPlugin RELEASE "imageformats/qtiff.dll")
if(DEFINED MAYA_QT_NDEBUG)
	_populate_Gui_plugin_properties(QTiffPlugin DEBUG "imageformats/qtiffd.dll")
endif()

list(APPEND Qt5Gui_PLUGINS Qt5::QTiffPlugin)
