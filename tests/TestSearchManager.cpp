#include "SearchManager.h"
#include "TextEditor.h"

#include <gtest/gtest.h>

class SearchManagerTest : public ::testing::Test {
protected:
    SearchManager search;
    TextEditor editor;

    void SetUp() override {
        editor.setPlainText("Alpha beta\nalpha BETA\nGamma alpha\n");
        editor.moveCursor(QTextCursor::Start);
    }
};

TEST_F(SearchManagerTest, FindNextReturnsFalseForNullEditor) {
    EXPECT_FALSE(search.findNext(nullptr, "Alpha"));
}

TEST_F(SearchManagerTest, FindNextFindsFirstMatch) {
    EXPECT_TRUE(search.findNext(&editor, "Alpha"));
    EXPECT_EQ(editor.textCursor().selectedText(), "Alpha");
}

TEST_F(SearchManagerTest, FindNextIsCaseInsensitiveByDefault) {
    EXPECT_TRUE(search.findNext(&editor, "alpha"));
    EXPECT_EQ(editor.textCursor().selectedText(), "Alpha");
}

TEST_F(SearchManagerTest, FindNextCanBeCaseSensitive) {
    EXPECT_TRUE(search.findNext(&editor, "alpha", true));
    EXPECT_EQ(editor.textCursor().selectedText(), "alpha");
}

TEST_F(SearchManagerTest, FindNextReturnsFalseWhenNoMatchExists) {
    EXPECT_FALSE(search.findNext(&editor, "Delta"));
}


TEST_F(SearchManagerTest, FindPreviousReturnsFalseForEmptyQuery) {
    EXPECT_FALSE(search.findPrevious(&editor, ""));
}

TEST_F(SearchManagerTest, FindPreviousFindsPreviousMatch) {
    editor.moveCursor(QTextCursor::End);

    EXPECT_TRUE(search.findPrevious(&editor, "alpha"));
    EXPECT_EQ(editor.textCursor().selectedText(), "alpha");
}