cmake_minimum_required(VERSION 3.21)
project(CodeEditorWidgets LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

set(APP_SOURCES
    MainWindow.cpp
    TextEditor.cpp
    FileManager.cpp
    SyntaxHighlighter.cpp
    SearchManager.cpp
    SettingsManager.cpp
    AutoSaveManager.cpp
    RecentFilesManager.cpp
)

set(APP_HEADERS
    MainWindow.h
    TextEditor.h
    FileManager.h
    SyntaxHighlighter.h
    SearchManager.h
    SettingsManager.h
    AutoSaveManager.h
    RecentFilesManager.h
    Theme.h
)

qt_add_executable(CodeEditorWidgets
    main.cpp
    ${APP_SOURCES}
    ${APP_HEADERS}
)

target_link_libraries(CodeEditorWidgets PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()