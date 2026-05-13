#include "SyntaxHighlighter.h"

#include <QTextDocument>
#include <gtest/gtest.h>

class SyntaxHighlighterTest : public ::testing::Test {
protected:
    QTextDocument document;
    SyntaxHighlighter highlighter{&document};
};

TEST_F(SyntaxHighlighterTest, CanSelectCppLanguageByCppExtension) {
    document.setPlainText("int main() { return 0; }");
    highlighter.setLanguageByFileName("main.cpp");

    EXPECT_NO_THROW(highlighter.rehighlight());
}

TEST_F(SyntaxHighlighterTest, CanSelectCppLanguageByHeaderExtension) {
    document.setPlainText("class A {};");
    highlighter.setLanguageByFileName("a.hpp");

    EXPECT_NO_THROW(highlighter.rehighlight());
}

TEST_F(SyntaxHighlighterTest, CanSelectPythonLanguageByPyExtension) {
    document.setPlainText("def f():\n    return 1");
    highlighter.setLanguageByFileName("script.py");

    EXPECT_NO_THROW(highlighter.rehighlight());
}

TEST_F(SyntaxHighlighterTest, UnknownExtensionClearsRulesWithoutCrash) {
    document.setPlainText("plain text");
    highlighter.setLanguageByFileName("notes.md");

    EXPECT_NO_THROW(highlighter.rehighlight());
}

TEST_F(SyntaxHighlighterTest, CanDisableHighlighting) {
    document.setPlainText("int value = 10;");
    highlighter.setLanguageByFileName("main.cpp");
    highlighter.setHighlightingEnabled(false);

    EXPECT_NO_THROW(highlighter.rehighlight());
}
