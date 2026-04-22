#pragma once

#include "Theme.h"

#include <QObject>
#include <QSettings>
#include <QVector>

class SettingsManager : public QObject {
    Q_OBJECT

public:
    explicit SettingsManager(QObject *parent = nullptr);

    int fontSize() const;
    void setFontSize(int size);

    QString themeName() const;
    void setThemeName(const QString &name);

    bool syntaxHighlightingEnabled() const;
    void setSyntaxHighlightingEnabled(bool enabled);

    QVector<Theme> availableThemes() const;
    Theme currentTheme() const;

    void load();
    void save();

private:
    QVector<Theme> buildThemes() const;

    QSettings m_settings;
    int m_fontSize = 14;
    QString m_themeName = "Dark";
    bool m_syntaxHighlightingEnabled = true;
};