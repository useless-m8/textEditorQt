#include "TextEditor.h"

#include <gtest/gtest.h>

class TextEditorTest : public ::testing::Test {
protected:
    TextEditor editor;
};

TEST_F(TextEditorTest, EmptyFilePathByDefault) {
    EXPECT_TRUE(editor.filePath().isEmpty());
}

TEST_F(TextEditorTest, SetFilePathStoresPath) {
    editor.setFilePath("/tmp/main.cpp");

    EXPECT_EQ(editor.filePath(), "/tmp/main.cpp");
}

TEST_F(TextEditorTest, FileNameReturnsUntitledForEmptyPath) {
    EXPECT_EQ(editor.fileName(), "untitled.txt");
}

TEST_F(TextEditorTest, FileNameReturnsBaseNameForPath) {
    editor.setFilePath("/tmp/project/main.cpp");

    EXPECT_EQ(editor.fileName(), "main.cpp");
}

TEST_F(TextEditorTest, EncodingCanBeChanged) {
    editor.setEncoding("UTF-8");

    EXPECT_EQ(editor.encoding(), "UTF-8");
}


TEST_F(TextEditorTest, DocumentIsModifiedAfterTextEdit) {
    editor.setModified(false);
    editor.insertPlainText("abc");

    EXPECT_TRUE(editor.modified());
}

TEST_F(TextEditorTest, CurrentLineAndColumnUpdateWithCursor) {
    editor.setPlainText("abc\ndef");
    QTextCursor cursor = editor.textCursor();
    cursor.setPosition(5);
    editor.setTextCursor(cursor);

    EXPECT_EQ(editor.currentLine(), 2);
    EXPECT_EQ(editor.currentColumn(), 2);
}
