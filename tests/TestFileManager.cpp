#include "FileManager.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <gtest/gtest.h>

class FileManagerTest : public ::testing::Test {
protected:
    QTemporaryDir tempDir;
    FileManager manager;

    QString path(const QString &name) const {
        return tempDir.path() + "/" + name;
    }
};

TEST_F(FileManagerTest, SaveFileCreatesFileAndReturnsTrue) {
    const QString filePath = path("a.txt");

    EXPECT_TRUE(manager.saveFile(filePath, "hello", "UTF-8"));
    EXPECT_TRUE(QFile::exists(filePath));
}

TEST_F(FileManagerTest, OpenFileReturnsSavedUtf8Content) {
    const QString filePath = path("utf8.txt");

    ASSERT_TRUE(manager.saveFile(filePath, QString::fromUtf8("Привет\nмир"), "UTF-8"));

    EXPECT_EQ(manager.openFile(filePath, "UTF-8"), QString::fromUtf8("Привет\nмир"));
}

TEST_F(FileManagerTest, OpenMissingFileReturnsEmptyString) {
    EXPECT_TRUE(manager.openFile(path("missing.txt"), "UTF-8").isEmpty());
}

TEST_F(FileManagerTest, ExistsReturnsTrueForExistingFile) {
    const QString filePath = path("exists.txt");
    ASSERT_TRUE(manager.saveFile(filePath, "data", "UTF-8"));

    EXPECT_TRUE(manager.exists(filePath));
}


TEST_F(FileManagerTest, SaveFileOverwritesOldContentAtomically) {
    const QString filePath = path("overwrite.txt");

    ASSERT_TRUE(manager.saveFile(filePath, "old", "UTF-8"));
    ASSERT_TRUE(manager.saveFile(filePath, "new", "UTF-8"));

    EXPECT_EQ(manager.openFile(filePath, "UTF-8"), "new");
}


TEST_F(FileManagerTest, SaveBackupCreatesBackupFile) {
    const QString filePath = path("backup.txt");

    EXPECT_TRUE(manager.saveBackup(filePath, "backup-data"));
    EXPECT_TRUE(QFile::exists(manager.backupPathFor(filePath)));
}

TEST_F(FileManagerTest, LoadBackupReturnsStoredData) {
    const QString filePath = path("load_backup.txt");

    ASSERT_TRUE(manager.saveBackup(filePath, "backup-content"));

    EXPECT_EQ(manager.loadBackup(filePath), "backup-content");
}
