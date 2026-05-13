cmake_minimum_required(VERSION 3.21)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(APP_SOURCES_FOR_TESTS
    ../TextEditor.cpp
    ../FileManager.cpp
    ../SyntaxHighlighter.cpp
    ../SearchManager.cpp
    ../SettingsManager.cpp
    ../AutoSaveManager.cpp
    ../RecentFilesManager.cpp
)

add_library(CodeEditorCoreForTests STATIC
    ${APP_SOURCES_FOR_TESTS}
)

target_include_directories(CodeEditorCoreForTests PUBLIC
    ..
)

target_link_libraries(CodeEditorCoreForTests PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

add_executable(CodeEditorUnitTests
    test_main.cpp
    TestFileManager.cpp
    TestRecentFilesManager.cpp
    TestSettingsManager.cpp
    TestSearchManager.cpp
    TestSyntaxHighlighter.cpp
    TestTextEditor.cpp
    TestAutoSaveManager.cpp
)

target_link_libraries(CodeEditorUnitTests PRIVATE
    CodeEditorCoreForTests
    GTest::gtest
    GTest::gtest_main
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

add_test(NAME CodeEditorUnitTests COMMAND CodeEditorUnitTests)

add_subdirectory(scenarios)