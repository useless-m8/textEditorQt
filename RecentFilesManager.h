#pragma once

#include <QObject>
#include <QSettings>
#include <QStringList>

class RecentFilesManager : public QObject {
    Q_OBJECT
public:
    explicit RecentFilesManager(QObject *parent = nullptr);

    QStringList files() const;
    void addFile(const QString &path);
    void clear();

private:
    void load();
    void save();

    QStringList m_files;
    QSettings m_settings;
};