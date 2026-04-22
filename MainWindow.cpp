#include "MainWindow.h"

#include "AutoSaveManager.h"
#include "FileManager.h"
#include "RecentFilesManager.h"
#include "SearchManager.h"
#include "SettingsManager.h"
#include "SyntaxHighlighter.h"
#include "TextEditor.h"
#include "Theme.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDockWidget>
#include <QFileDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QStatusBar>
#include <QTextDocument>
#include <QToolBar>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_editor(new TextEditor(this)),
    m_fileManager(new FileManager(this)),
    m_searchManager(new SearchManager(this)),
    m_settingsManager(new SettingsManager(this)),
    m_autoSaveManager(new AutoSaveManager(this)),
    m_recentFilesManager(new RecentFilesManager(this)),
    m_syntaxHighlighter(new SyntaxHighlighter(m_editor->document())),
    m_recentFilesList(nullptr),
    m_searchEdit(nullptr),
    m_cursorLabel(nullptr),
    m_encodingLabel(nullptr),
    m_highlightingLabel(nullptr),
    m_saveAction(nullptr)
{
    createUi();
    createMenus();
    createToolBar();
    createStatusBar();
    createRecentFilesDock();

    setCentralWidget(m_editor);
    resize(1200, 800);

    m_autoSaveManager->setup(m_editor, m_fileManager, 30000);

    connect(m_editor, &QPlainTextEdit::cursorPositionChanged, this, &MainWindow::updateStatusBar);
    connect(m_editor->document(), &QTextDocument::modificationChanged, this, &MainWindow::updateWindowTitle);
    connect(m_editor->document(), &QTextDocument::modificationChanged, this, &MainWindow::updateStatusBar);
    connect(m_editor, &QPlainTextEdit::textChanged, this, &MainWindow::updateStatusBar);

    applySettings();
    refreshRecentFilesUi();
    updateStatusBar();
    updateWindowTitle();
}

MainWindow::~MainWindow() = default;

void MainWindow::createUi()
{
    setWindowTitle("Code Editor");
}

void MainWindow::createMenus()
{
    auto *fileMenu = menuBar()->addMenu(tr("&File"));

    auto *newAction = fileMenu->addAction(tr("New"));
    auto *openAction = fileMenu->addAction(tr("Open..."));
    m_saveAction = fileMenu->addAction(tr("Save"));
    auto *saveAsAction = fileMenu->addAction(tr("Save As..."));
    fileMenu->addSeparator();
    auto *exitAction = fileMenu->addAction(tr("Exit"));

    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveFileAs);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    auto *editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(tr("Undo"), m_editor, &QPlainTextEdit::undo, QKeySequence::Undo);
    editMenu->addAction(tr("Redo"), m_editor, &QPlainTextEdit::redo, QKeySequence::Redo);
    editMenu->addSeparator();
    editMenu->addAction(tr("Cut"), m_editor, &QPlainTextEdit::cut, QKeySequence::Cut);
    editMenu->addAction(tr("Copy"), m_editor, &QPlainTextEdit::copy, QKeySequence::Copy);
    editMenu->addAction(tr("Paste"), m_editor, &QPlainTextEdit::paste, QKeySequence::Paste);
    editMenu->addSeparator();
    editMenu->addAction(tr("Find Next"), this, &MainWindow::findNext);
    editMenu->addAction(tr("Find Previous"), this, &MainWindow::findPrevious);

    auto *settingsMenu = menuBar()->addMenu(tr("&Settings"));
    settingsMenu->addAction(tr("Preferences"), this, &MainWindow::showSettingsDialog);
}

void MainWindow::createToolBar()
{
    auto *toolBar = addToolBar(tr("Main Toolbar"));
    toolBar->setMovable(false);

    auto *newAction = toolBar->addAction(tr("New"));
    auto *openAction = toolBar->addAction(tr("Open"));
    auto *saveAction = toolBar->addAction(tr("Save"));

    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

    toolBar->addSeparator();

    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText(tr("Find..."));
    m_searchEdit->setMaximumWidth(260);
    toolBar->addWidget(m_searchEdit);

    auto *nextButton = new QPushButton(tr("Next"), this);
    auto *prevButton = new QPushButton(tr("Prev"), this);
    toolBar->addWidget(nextButton);
    toolBar->addWidget(prevButton);

    connect(nextButton, &QPushButton::clicked, this, &MainWindow::findNext);
    connect(prevButton, &QPushButton::clicked, this, &MainWindow::findPrevious);
}

void MainWindow::createStatusBar()
{
    m_cursorLabel = new QLabel(this);
    m_encodingLabel = new QLabel(this);
    m_highlightingLabel = new QLabel(this);

    statusBar()->addPermanentWidget(m_cursorLabel);
    statusBar()->addPermanentWidget(m_encodingLabel);
    statusBar()->addPermanentWidget(m_highlightingLabel);
}

void MainWindow::createRecentFilesDock()
{
    auto *dock = new QDockWidget(tr("Recent Files"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    m_recentFilesList = new QListWidget(dock);
    dock->setWidget(m_recentFilesList);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    connect(m_recentFilesList, &QListWidget::itemDoubleClicked, this, [this]() {
        openFileFromRecent();
    });
}
void MainWindow::applySettings()
{
    const Theme theme = m_settingsManager->currentTheme();
    qApp->setStyleSheet(theme.appStyleSheet);
    m_editor->setStyleSheet(theme.editorStyleSheet);
    m_editor->setEditorFontSize(m_settingsManager->fontSize());
    m_editor->setCurrentLineColor(theme.currentLineColor);
    m_syntaxHighlighter->setHighlightingEnabled(m_settingsManager->syntaxHighlightingEnabled());
    updateStatusBar();
}

void MainWindow::refreshRecentFilesUi()
{
    m_recentFilesList->clear();
    m_recentFilesList->addItems(m_recentFilesManager->files());
}

bool MainWindow::loadFile(const QString &path)
{
    const QString content = m_fileManager->openFile(path);
    m_editor->setPlainText(content);
    m_editor->setFilePath(path);
    m_editor->setEncoding("UTF-8");
    m_editor->setModified(false);
    m_recentFilesManager->addFile(path);
    m_syntaxHighlighter->setLanguageByFileName(path);

    refreshRecentFilesUi();
    updateWindowTitle();
    updateStatusBar();
    return true;
}

bool MainWindow::saveToPath(const QString &path)
{
    if (!m_fileManager->saveFile(path, m_editor->toPlainText(), m_editor->encoding())) {
        QMessageBox::warning(this, tr("Save file"), tr("Failed to save file."));
        return false;
    }

    m_editor->setFilePath(path);
    m_editor->setModified(false);
    m_recentFilesManager->addFile(path);
    m_syntaxHighlighter->setLanguageByFileName(path);

    refreshRecentFilesUi();
    updateWindowTitle();
    updateStatusBar();
    return true;
}

bool MainWindow::maybeSave()
{
    if (!m_editor->modified()) {
        return true;
    }

    const auto result = QMessageBox::warning(
        this,
        tr("Unsaved changes"),
        tr("The document has unsaved changes. Do you want to save them?"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
        QMessageBox::Save
        );

    if (result == QMessageBox::Save) {
        saveFile();
        return !m_editor->modified();
    }
    if (result == QMessageBox::Cancel) {
        return false;
    }
    return true;
}

void MainWindow::newFile()
{
    if (!maybeSave()) {
        return;
    }

    m_editor->clear();
    m_editor->setFilePath(QString());
    m_editor->setEncoding("UTF-8");
    m_editor->setModified(false);
    m_syntaxHighlighter->setLanguageByFileName(QString());
    updateWindowTitle();
    updateStatusBar();
}

void MainWindow::openFile()
{
    if (!maybeSave()) {
        return;
    }

    const QString path = QFileDialog::getOpenFileName(
        this,
        tr("Open file"),
        QString(),
        tr("Source files (*.cpp *.h *.hpp *.py *.txt);;All files (*.*)")
        );

    if (path.isEmpty()) {
        return;
    }

    loadFile(path);
}

void MainWindow::openFileFromRecent()
{
    if (!m_recentFilesList->currentItem()) {
        return;
    }

    if (!maybeSave()) {
        return;
    }

    loadFile(m_recentFilesList->currentItem()->text());
}

void MainWindow::saveFile()
{
    if (m_editor->filePath().isEmpty()) {
        saveFileAs();
        return;
    }

    saveToPath(m_editor->filePath());
}

void MainWindow::saveFileAs()
{
    const QString path = QFileDialog::getSaveFileName(
        this,
        tr("Save file as"),
        m_editor->fileName(),
        tr("Source files (*.cpp *.h *.hpp *.py *.txt);;All files (*.*)")
        );

    if (path.isEmpty()) {
        return;
    }

    saveToPath(path);
}

void MainWindow::findNext()
{
    if (!m_searchManager->findNext(m_editor, m_searchEdit->text())) {
        statusBar()->showMessage(tr("No more matches found"), 2000);
    }
}

void MainWindow::findPrevious()
{
    if (!m_searchManager->findPrevious(m_editor, m_searchEdit->text())) {
        statusBar()->showMessage(tr("No previous matches found"), 2000);
    }
}

void MainWindow::showSettingsDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Settings"));

    auto *layout = new QVBoxLayout(&dialog);
    auto *form = new QFormLayout();

    auto *fontSizeSpin = new QSpinBox(&dialog);
    fontSizeSpin->setRange(8, 40);
    fontSizeSpin->setValue(m_settingsManager->fontSize());

    auto *themeCombo = new QComboBox(&dialog);
    for (const Theme &theme : m_settingsManager->availableThemes()) {
        themeCombo->addItem(theme.name);
    }
    themeCombo->setCurrentText(m_settingsManager->themeName());

    auto *highlightingCombo = new QComboBox(&dialog);
    highlightingCombo->addItems({tr("Enabled"), tr("Disabled")});
    highlightingCombo->setCurrentIndex(m_settingsManager->syntaxHighlightingEnabled() ? 0 : 1);

    form->addRow(tr("Font size:"), fontSizeSpin);
    form->addRow(tr("Theme:"), themeCombo);
    form->addRow(tr("Syntax highlighting:"), highlightingCombo);

    layout->addLayout(form);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        m_settingsManager->setFontSize(fontSizeSpin->value());
        m_settingsManager->setThemeName(themeCombo->currentText());
        m_settingsManager->setSyntaxHighlightingEnabled(highlightingCombo->currentIndex() == 0);
        m_settingsManager->save();
        applySettings();
    }
}

void MainWindow::updateStatusBar()
{
    m_cursorLabel->setText(tr("Line: %1  Col: %2").arg(m_editor->currentLine()).arg(m_editor->currentColumn()));
    m_encodingLabel->setText(tr("Encoding: %1").arg(m_editor->encoding()));
    m_highlightingLabel->setText(
        m_settingsManager->syntaxHighlightingEnabled() ? tr("Highlighting: ON") : tr("Highlighting: OFF")
        );
}

void MainWindow::updateWindowTitle()
{
    setWindowTitle(QString("%1%2 - Code Editor")
                       .arg(m_editor->fileName())
                       .arg(m_editor->modified() ? " *" : ""));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_autoSaveManager->triggerBackupNow();
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}