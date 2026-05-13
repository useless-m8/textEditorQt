#include "RecentFilesManager.h"

#include <QSettings>
#include <gtest/gtest.h>

class RecentFilesManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        QSettings settings("MyCompany", "CodeEditorWidgets");
        settings.clear();
    }

    void TearDown() override {
        QSettings settings("MyCompany", "CodeEditorWidgets");
        settings.clear();
    }
};

TEST_F(RecentFilesManagerTest, StartsEmptyWhenSettingsAreEmpty) {
    RecentFilesManager manager;
    EXPECT_TRUE(manager.files().isEmpty());
}

TEST_F(RecentFilesManagerTest, AddFileStoresPath) {
    RecentFilesManager manager;
    manager.addFile("/tmp/a.cpp");

    ASSERT_EQ(manager.files().size(), 1);
    EXPECT_EQ(manager.files().first(), "/tmp/a.cpp");
}

TEST_F(RecentFilesManagerTest, AddEmptyPathIsIgnored) {
    RecentFilesManager manager;
    manager.addFile("");

    EXPECT_TRUE(manager.files().isEmpty());
}

TEST_F(RecentFilesManagerTest, MostRecentFileIsPrepended) {
    RecentFilesManager manager;
    manager.addFile("/tmp/a.cpp");
    manager.addFile("/tmp/b.cpp");

    ASSERT_EQ(manager.files().size(), 2);
    EXPECT_EQ(manager.files().at(0), "/tmp/b.cpp");
    EXPECT_EQ(manager.files().at(1), "/tmp/a.cpp");
}

TEST_F(RecentFilesManagerTest, DuplicateFileIsMovedToFront) {
    RecentFilesManager manager;
    manager.addFile("/tmp/a.cpp");
    manager.addFile("/tmp/b.cpp");
    manager.addFile("/tmp/a.cpp");

    ASSERT_EQ(manager.files().size(), 2);
    EXPECT_EQ(manager.files().at(0), "/tmp/a.cpp");
    EXPECT_EQ(manager.files().at(1), "/tmp/b.cpp");
}

TEST_F(RecentFilesManagerTest, KeepsOnlyTenFiles) {
    RecentFilesManager manager;

    for (int i = 0; i < 12; ++i) {
        manager.addFile(QString("/tmp/%1.cpp").arg(i));
    }

    EXPECT_EQ(manager.files().size(), 10);
    EXPECT_EQ(manager.files().first(), "/tmp/11.cpp");
    EXPECT_FALSE(manager.files().contains("/tmp/0.cpp"));
    EXPECT_FALSE(manager.files().contains("/tmp/1.cpp"));
}

TEST_F(RecentFilesManagerTest, ClearRemovesAllFiles) {
    RecentFilesManager manager;
    manager.addFile("/tmp/a.cpp");
    manager.addFile("/tmp/b.cpp");

    manager.clear();

    EXPECT_TRUE(manager.files().isEmpty());
}

