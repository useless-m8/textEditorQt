#include "FileManager.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSaveFile>
#include <QStandardPaths>
#include <QTextStream>

FileManager::FileManager(QObject *parent) : QObject(parent) {}

QString FileManager::openFile(const QString &path, const QString &) const {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }

    QTextStream in(&file);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    in.setEncoding(QStringConverter::Utf8);
#endif
    return in.readAll();
}

bool FileManager::saveFile(const QString &path, const QString &content, const QString &) const {
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    out.setEncoding(QStringConverter::Utf8);
#endif
    out << content;
    return file.commit();
}

bool FileManager::exists(const QString &path) const {
    return QFileInfo::exists(path);
}

QString FileManager::backupPathFor(const QString &path) const {
    const QString baseDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/backup";
    QDir().mkpath(baseDir);

    QString fileKey = path;
    if (fileKey.isEmpty()) {
        fileKey = "unsaved_document";
    }

    fileKey.replace(":", "_").replace("/", "_").replace("\\", "_");
    return baseDir + "/" + fileKey + ".bak";
}

bool FileManager::saveBackup(const QString &path, const QString &content) const {
    QFile file(backupPathFor(path));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream out(&file);
    out << content;
    return true;
}

QString FileManager::loadBackup(const QString &path) const {
    QFile file(backupPathFor(path));
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }
    QTextStream in(&file);
    return in.readAll();
}

bool FileManager::removeBackup(const QString &path) const {
    return QFile::remove(backupPathFor(path));
}