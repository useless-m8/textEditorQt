#pragma once

#include <QObject>
#include <QString>

class FileManager : public QObject {
    Q_OBJECT
public:
    explicit FileManager(QObject *parent = nullptr);

    QString openFile(const QString &path, const QString &encoding = "UTF-8") const;
    bool saveFile(const QString &path, const QString &content, const QString &encoding = "UTF-8") const;
    bool exists(const QString &path) const;
    QString backupPathFor(const QString &path) const;
    bool saveBackup(const QString &path, const QString &content) const;
    QString loadBackup(const QString &path) const;
    bool removeBackup(const QString &path) const;
};