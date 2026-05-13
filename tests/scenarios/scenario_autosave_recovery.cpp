#include "AutoSaveManager.h"
#include "FileManager.h"
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
    TextEditor editor;
    AutoSaveManager autoSave;

    const QString path = dir.path() + "/draft.txt";

    editor.setFilePath(path);
    editor.setPlainText("important unsaved text");
    editor.setEncoding("UTF-8");
    editor.setModified(true);

    autoSave.setup(&editor, &fileManager, 100000);
    autoSave.triggerBackupNow();

    const QString backup = fileManager.loadBackup(path);
    if (backup != "important unsaved text") {
        std::cerr << "Backup was not created correctly\n";
        return 2;
    }

    TextEditor recoveredEditor;
    recoveredEditor.setPlainText(backup);
    recoveredEditor.setFilePath(path);
    recoveredEditor.setModified(true);

    if (recoveredEditor.toPlainText() != editor.toPlainText()) {
        std::cerr << "Recovered content mismatch\n";
        return 3;
    }

    std::cout << "Scenario passed: edit -> autosave -> load backup -> recover\n";
    return 0;
}
