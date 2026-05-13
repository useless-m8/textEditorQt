# CMake generated Testfile for 
# Source directory: C:/texteditor/texteditor/tests
# Build directory: C:/texteditor/texteditor/build/Desktop-Debug/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[CodeEditorUnitTests]=] "C:/texteditor/texteditor/build/Desktop-Debug/CodeEditorUnitTests.exe")
set_tests_properties([=[CodeEditorUnitTests]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/texteditor/texteditor/tests/CMakeLists.txt;59;add_test;C:/texteditor/texteditor/tests/CMakeLists.txt;0;")
subdirs("../_deps/googletest-build")
subdirs("scenarios")
