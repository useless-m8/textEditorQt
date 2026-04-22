#include "SettingsManager.h"

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent),
    m_settings("TextEditor", "HighlighterSettings")
{
    load();
}

int SettingsManager::fontSize() const
{
    return m_fontSize;
}

void SettingsManager::setFontSize(int size)
{
    m_fontSize = size;
}

QString SettingsManager::themeName() const
{
    return m_themeName;
}

void SettingsManager::setThemeName(const QString &name)
{
    m_themeName = name;
}

bool SettingsManager::syntaxHighlightingEnabled() const
{
    return m_syntaxHighlightingEnabled;
}

void SettingsManager::setSyntaxHighlightingEnabled(bool enabled)
{
    m_syntaxHighlightingEnabled = enabled;
}

QVector<Theme> SettingsManager::availableThemes() const
{
    return buildThemes();
}

Theme SettingsManager::currentTheme() const
{
    const auto themes = buildThemes();
    for (const auto &theme : themes) {
        if (theme.name == m_themeName) {
            return theme;
        }
    }
    return themes.first();
}

void SettingsManager::load()
{
    m_fontSize = m_settings.value("fontSize", 14).toInt();
    m_themeName = m_settings.value("themeName", "Dark").toString();
    m_syntaxHighlightingEnabled = m_settings.value("syntaxHighlightingEnabled", true).toBool();
}

void SettingsManager::save()
{
    m_settings.setValue("fontSize", m_fontSize);
    m_settings.setValue("themeName", m_themeName);
    m_settings.setValue("syntaxHighlightingEnabled", m_syntaxHighlightingEnabled);
}

QVector<Theme> SettingsManager::buildThemes() const
{
    return {
        {
            "Dark",
            R"(
                QMainWindow { background: #1e1e1e; }
                QMenuBar { background: #2d2d30; color: #ffffff; }
                QMenuBar::item:selected { background: #3e3e42; }
                QMenu { background: #252526; color: white; border: 1px solid #3e3e42; }
                QMenu::item:selected { background: #094771; }
                QToolBar { background: #2d2d30; border: none; spacing: 6px; }
                QStatusBar { background: #007acc; color: white; }
                QLineEdit, QComboBox, QSpinBox { background: #2d2d30; color: white; border: 1px solid #555; padding: 4px; }
                QPushButton { background: #3e3e42; color: white; border: 1px solid #555; padding: 6px 10px; border-radius: 4px; }
                QPushButton:hover { background: #505357; }
                QListWidget { background: #252526; color: white; border: none; }
            )",
            R"(
                QPlainTextEdit {
                    background: #1e1e1e;
                    color: #d4d4d4;
                    selection-background-color: #264f78;
                    border: none;
                    font-family: Consolas;
                }
            )",
            QColor("#2a2d2e")
        },
        {
            "Light",
            R"(
                QMainWindow { background: #f3f3f3; }
                QMenuBar { background: #e9e9e9; color: #202020; }
                QMenuBar::item:selected { background: #d0d0d0; }
                QMenu { background: white; color: #202020; border: 1px solid #cfcfcf; }
                QMenu::item:selected { background: #cfe8ff; }
                QToolBar { background: #e9e9e9; border: none; spacing: 6px; }
                QStatusBar { background: #dbeeff; color: #202020; }
                QLineEdit, QComboBox, QSpinBox { background: white; color: black; border: 1px solid #aaa; padding: 4px; }
                QPushButton { background: #f8f8f8; color: black; border: 1px solid #aaa; padding: 6px 10px; border-radius: 4px; }
                QPushButton:hover { background: #eaeaea; }
                QListWidget { background: white; color: black; border: none; }
            )",
            R"(
                QPlainTextEdit {
                    background: white;
                    color: black;
                    selection-background-color: #cfe8ff;
                    border: none;
                    font-family: Consolas;
                }
            )",
            QColor("white")
        }
    };
}