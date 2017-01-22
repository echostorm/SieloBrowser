QT += widgets
QT += network
QT += webenginewidgets
QT += xml

macx:ICON = $${PWD}/icon.icns

SOURCES += \
    src/Main.cpp \
    src/SMainWindow.cpp \
    src/SWidgets/SMenu.cpp \
    src/SWidgets/SSearchArea.cpp \
    src/SWidgets/STabWidget.cpp \
    src/SWidgets/SToolBar.cpp \
    src/SWidgets/SUrlArea.cpp \
    src/SWindows/SHtmlSrcViewver.cpp \
    src/SWindows/SDownload.cpp \
    src/SWindows/SBookmarks.cpp \
    src/SWindows/SPreferences.cpp \
    src/SApplication.cpp \
    src/SPlugins/SPlugins.cpp \
    src/SPlugins/SPluginProxy.cpp \
    src/SWindows/History/SHistory.cpp \
    src/SWindows/History/SHistoryManager.cpp \
    src/SWindows/History/SHistoryView.cpp \
    src/SWindows/History/SHistoryModel.cpp \
    src/SWidgets/Web/SWebHitTestResult.cpp \
    src/SWidgets/Web/SWebPage.cpp \
    src/SDialogs/SCheckBoxDialog.cpp \
    src/SWidgets/Web/SWebView.cpp \
    src/SWidgets/OLDSWebView.cpp

HEADERS += \
    includes/SMainWindow.hpp \
    includes/SWidgets/SMenu.hpp \
    includes/SWidgets/SSearchArea.hpp \
    includes/SWidgets/STabWidget.hpp \
    includes/SWidgets/SToolBar.hpp \
    includes/SWidgets/SUrlArea.hpp \
    includes/SWindows/SHtmlSrcViewver.hpp \
    includes/SWindows/SDownload.hpp \
    includes/SActions.hpp \
    includes/SWindows/SBookmarks.hpp \
    includes/SWindows/SPreferences.hpp \
    includes/SApplication.hpp \
    includes/SPlugins/SPluginInterface.hpp \
    includes/SPlugins/SPlugins.hpp \
    includes/SPlugins/SPluginProxy.hpp \
    includes/SWindows/History/SHistory.hpp \
    includes/SWindows/History/SHistoryItem.hpp \
    includes/SWindows/History/SHistoryManager.hpp \
    includes/SWindows/History/SHistoryView.hpp \
    includes/SWindows/History/SHistoryModel.hpp \
    includes/SWidgets/Web/SWebView.hpp \
    includes/SWidgets/Web/SWebPage.hpp \
    includes/SWidgets/Web/SWebHitTestResult.hpp \
    includes/SDialogs/SCheckBoxDialog.hpp \
    includes/SWidgets/Web/SLoadRequest.hpp \
    includes/SWidgets/OLDSWebView.hpp
    includes/SApplication.hpp

RESOURCES += \
    ../Utils/ressources/data.qrc

