#include "SettingsManager.h"

#include <QSettings>
#include <gtest/gtest.h>

class SettingsManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        QSettings settings("TextEditor", "HighlighterSettings");
        settings.clear();
    }

    void TearDown() override {
        QSettings settings("TextEditor", "HighlighterSettings");
        settings.clear();
    }
};

TEST_F(SettingsManagerTest, LoadsDefaultFontSize) {
    SettingsManager manager;
    EXPECT_EQ(manager.fontSize(), 14);
}

TEST_F(SettingsManagerTest, LoadsDefaultThemeName) {
    SettingsManager manager;
    EXPECT_EQ(manager.themeName(), "Dark");
}

TEST_F(SettingsManagerTest, LoadsDefaultSyntaxHighlightingEnabled) {
    SettingsManager manager;
    EXPECT_TRUE(manager.syntaxHighlightingEnabled());
}

TEST_F(SettingsManagerTest, SetFontSizeChangesValueInMemory) {
    SettingsManager manager;
    manager.setFontSize(20);

    EXPECT_EQ(manager.fontSize(), 20);
}

TEST_F(SettingsManagerTest, SetThemeNameChangesValueInMemory) {
    SettingsManager manager;
    manager.setThemeName("Light");

    EXPECT_EQ(manager.themeName(), "Light");
}

TEST_F(SettingsManagerTest, SetSyntaxHighlightingChangesValueInMemory) {
    SettingsManager manager;
    manager.setSyntaxHighlightingEnabled(false);

    EXPECT_FALSE(manager.syntaxHighlightingEnabled());
}

TEST_F(SettingsManagerTest, SavePersistsValues) {
    {
        SettingsManager manager;
        manager.setFontSize(22);
        manager.setThemeName("Light");
        manager.setSyntaxHighlightingEnabled(false);
        manager.save();
    }

    SettingsManager loaded;
    EXPECT_EQ(loaded.fontSize(), 22);
    EXPECT_EQ(loaded.themeName(), "Light");
    EXPECT_FALSE(loaded.syntaxHighlightingEnabled());
}
