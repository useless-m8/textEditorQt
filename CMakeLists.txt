cmake_minimum_required(VERSION 3.21)
project(CodeEditorWidgets LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

qt_add_executable(CodeEditorWidgets
    main.cpp
    MainWindow.cpp
    TextEditor.cpp
    FileManager.cpp
    FileManager.h
    MainWindow.h
    TextEditor.h
    SyntaxHighlighter.cpp
    SyntaxHighlighter.h
    SearchManager.cpp
    SearchManager.h
    SettingsManager.cpp
    SettingsManager.h
    AutoSaveManager.cpp
    AutoSaveManager.h
    RecentFilesManager.cpp
    RecentFilesManager.h
    Theme.h
)

target_link_libraries(CodeEditorWidgets PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)