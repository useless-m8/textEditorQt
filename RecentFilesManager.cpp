#include "RecentFilesManager.h"

RecentFilesManager::RecentFilesManager(QObject *parent)
    : QObject(parent),
    m_settings("MyCompany", "CodeEditorWidgets") {
    load();
}

QStringList RecentFilesManager::files() const {
    return m_files;
}

void RecentFilesManager::addFile(const QString &path) {
    if (path.isEmpty()) {
        return;
    }

    m_files.removeAll(path);
    m_files.prepend(path);
    while (m_files.size() > 10) {
        m_files.removeLast();
    }
    save();
}

void RecentFilesManager::clear() {
    m_files.clear();
    save();
}

void RecentFilesManager::load() {
    m_files = m_settings.value("recentFiles").toStringList();
}

void RecentFilesManager::save() {
    m_settings.setValue("recentFiles", m_files);
}