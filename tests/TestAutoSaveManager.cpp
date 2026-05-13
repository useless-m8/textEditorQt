#include "AutoSaveManager.h"
#include "FileManager.h"
#include "TextEditor.h"

#include <QFile>
#include <QTemporaryDir>
#include <gtest/gtest.h>

class AutoSaveManagerTest : public ::testing::Test {
protected:
    TextEditor editor;
    FileManager fileManager;
    AutoSaveManager autoSave;
    QTemporaryDir tempDir;

    QString path(const QString &name) const {
        return tempDir.path() + "/" + name;
    }
};

TEST_F(AutoSaveManagerTest, TriggerWithoutSetupDoesNotCrash) {
    EXPECT_NO_THROW(autoSave.triggerBackupNow());
}

TEST_F(AutoSaveManagerTest, DoesNotCreateBackupForUnmodifiedDocument) {
    const QString filePath = path("a.txt");
    editor.setFilePath(filePath);
    editor.setPlainText("content");
    editor.setModified(false);

    autoSave.setup(&editor, &fileManager, 100000);
    autoSave.triggerBackupNow();

    EXPECT_FALSE(QFile::exists(fileManager.backupPathFor(filePath)));
}

TEST_F(AutoSaveManagerTest, CreatesBackupForModifiedSavedDocument) {
    const QString filePath = path("saved.cpp");
    editor.setFilePath(filePath);
    editor.setPlainText("int main() {}");
    editor.setModified(true);

    autoSave.setup(&editor, &fileManager, 100000);
    autoSave.triggerBackupNow();

    EXPECT_EQ(fileManager.loadBackup(filePath), "int main() {}");
}

TEST_F(AutoSaveManagerTest, CreatesBackupForModifiedUnsavedDocument) {
    editor.setFilePath(QString());
    editor.setPlainText("unsaved");
    editor.setModified(true);

    autoSave.setup(&editor, &fileManager, 100000);
    autoSave.triggerBackupNow();

    EXPECT_EQ(fileManager.loadBackup(QString()), "unsaved");
}

TEST_F(AutoSaveManagerTest, BackupContentIsUpdatedOnSecondTrigger) {
    const QString filePath = path("updated.txt");
    editor.setFilePath(filePath);
    editor.setPlainText("old");
    editor.setModified(true);

    autoSave.setup(&editor, &fileManager, 100000);
    autoSave.triggerBackupNow();

    editor.setPlainText("new");
    editor.setModified(true);
    autoSave.triggerBackupNow();

    EXPECT_EQ(fileManager.loadBackup(filePath), "new");
}

TEST_F(AutoSaveManagerTest, SetupCanBeCalledMoreThanOnce) {
    const QString filePath = path("repeat.txt");
    editor.setFilePath(filePath);
    editor.setPlainText("data");
    editor.setModified(true);

    autoSave.setup(&editor, &fileManager, 100000);
    autoSave.setup(&editor, &fileManager, 100000);
    autoSave.triggerBackupNow();

    EXPECT_EQ(fileManager.loadBackup(filePath), "data");
}

TEST_F(AutoSaveManagerTest, TriggerDoesNotResetModifiedFlag) {
    const QString filePath = path("modified.txt");
    editor.setFilePath(filePath);
    editor.setPlainText("data");
    editor.setModified(true);

    autoSave.setup(&editor, &fileManager, 100000);
    autoSave.triggerBackupNow();

    EXPECT_TRUE(editor.modified());
}
