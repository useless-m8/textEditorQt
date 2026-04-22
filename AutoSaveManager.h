#pragma once

#include <QObject>
#include <QTimer>

class TextEditor;
class FileManager;

class AutoSaveManager : public QObject {
    Q_OBJECT
public:
    explicit AutoSaveManager(QObject *parent = nullptr);

    void setup(TextEditor *editor, FileManager *fileManager, int intervalMs = 30000);
    void triggerBackupNow();

private slots:
    void performAutoSave();

private:
    QTimer m_timer;
    TextEditor *m_editor = nullptr;
    FileManager *m_fileManager = nullptr;
};