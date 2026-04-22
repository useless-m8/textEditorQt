#pragma once

#include <QMainWindow>

class TextEditor;
class FileManager;
class SearchManager;
class SettingsManager;
class AutoSaveManager;
class RecentFilesManager;
class SyntaxHighlighter;
class QListWidget;
class QLineEdit;
class QLabel;
class QAction;
class QCloseEvent;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newFile();
    void openFile();
    void openFileFromRecent();
    void saveFile();
    void saveFileAs();
    void findNext();
    void findPrevious();
    void showSettingsDialog();
    void updateStatusBar();
    void updateWindowTitle();

private:
    void createUi();
    void createMenus();
    void createToolBar();
    void createStatusBar();
    void createRecentFilesDock();
    void applySettings();
    void refreshRecentFilesUi();
    bool loadFile(const QString &path);
    bool saveToPath(const QString &path);
    bool maybeSave();

    TextEditor *m_editor;
    FileManager *m_fileManager;
    SearchManager *m_searchManager;
    SettingsManager *m_settingsManager;
    AutoSaveManager *m_autoSaveManager;
    RecentFilesManager *m_recentFilesManager;
    SyntaxHighlighter *m_syntaxHighlighter;

    QListWidget *m_recentFilesList;
    QLineEdit *m_searchEdit;
    QLabel *m_cursorLabel;
    QLabel *m_encodingLabel;
    QLabel *m_highlightingLabel;

    QAction *m_saveAction;
};