#include "SearchManager.h"
#include "TextEditor.h"

#include <QApplication>
#include <iostream>

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    TextEditor editor;
    SearchManager search;

    editor.setPlainText("alpha\nbeta\nalpha\ngamma\n");
    editor.moveCursor(QTextCursor::Start);

    if (!search.findNext(&editor, "alpha")) {
        std::cerr << "First search failed\n";
        return 1;
    }

    if (editor.textCursor().selectedText() != "alpha") {
        std::cerr << "First match mismatch\n";
        return 2;
    }

    if (!search.findNext(&editor, "alpha")) {
        std::cerr << "Second search failed\n";
        return 3;
    }

    if (editor.textCursor().blockNumber() != 2) {
        std::cerr << "Second match should be on third line\n";
        return 4;
    }

    if (!search.findPrevious(&editor, "alpha")) {
        std::cerr << "Previous search failed\n";
        return 5;
    }

    std::cout << "Scenario passed: search next -> search next -> search previous\n";
    return 0;
}
