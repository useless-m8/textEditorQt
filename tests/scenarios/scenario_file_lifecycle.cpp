#include "FileManager.h"
#include "RecentFilesManager.h"
#include "TextEditor.h"

#include <QApplication>
#include <QTemporaryDir>
#include <iostream>

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    QTemporaryDir dir;
    if (!dir.isValid()) {
        std::cerr << "Temporary directory creation failed\n";
        return 1;
    }

    FileManager fileManager;
    RecentFilesManager recentFiles;
    TextEditor editor;

    const QString path = dir.path() + "/scenario.cpp";

    editor.setPlainText("int main() { return 0; }\n");
    editor.setFilePath(path);
    editor.setEncoding("UTF-8");
    editor.setModified(true);

    if (!fileManager.saveFile(editor.filePath(), editor.toPlainText(), editor.encoding())) {
        std::cerr << "Save failed\n";
        return 2;
    }

    editor.setModified(false);
    recentFiles.addFile(path);

    const QString loaded = fileManager.openFile(path, "UTF-8");
    if (loaded != "int main() { return 0; }\n") {
        std::cerr << "Loaded content mismatch\n";
        return 3;
    }

    if (!recentFiles.files().contains(path)) {
        std::cerr << "Recent files were not updated\n";
        return 4;
    }

    std::cout << "Scenario passed: create -> edit -> save -> reopen -> recent files\n";
    return 0;
}
