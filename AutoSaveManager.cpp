#include "AutoSaveManager.h"
#include "FileManager.h"
#include "TextEditor.h"

AutoSaveManager::AutoSaveManager(QObject *parent) : QObject(parent) {
    connect(&m_timer, &QTimer::timeout, this, &AutoSaveManager::performAutoSave);
}

void AutoSaveManager::setup(TextEditor *editor, FileManager *fileManager, int intervalMs) {
    m_editor = editor;
    m_fileManager = fileManager;
    m_timer.start(intervalMs);
}

void AutoSaveManager::triggerBackupNow() {
    performAutoSave();
}

void AutoSaveManager::performAutoSave() {
    if (!m_editor || !m_fileManager) {
        return;
    }

    if (!m_editor->modified()) {
        return;
    }

    if (!m_editor->filePath().isEmpty()) {
        m_fileManager->saveBackup(m_editor->filePath(), m_editor->toPlainText());
    } else {
        m_fileManager->saveBackup(QString(), m_editor->toPlainText());
    }
}