#include "SearchManager.h"
#include "TextEditor.h"

#include <QTextDocument>

SearchManager::SearchManager(QObject *parent) : QObject(parent) {}

bool SearchManager::findNext(TextEditor *editor, const QString &query, bool caseSensitive) {
    if (!editor || query.isEmpty()) {
        return false;
    }

    QTextDocument::FindFlags flags;
    if (caseSensitive) {
        flags |= QTextDocument::FindCaseSensitively;
    }

    return editor->find(query, flags);
}

bool SearchManager::findPrevious(TextEditor *editor, const QString &query, bool caseSensitive) {
    if (!editor || query.isEmpty()) {
        return false;
    }

    QTextDocument::FindFlags flags = QTextDocument::FindBackward;
    if (caseSensitive) {
        flags |= QTextDocument::FindCaseSensitively;
    }

    return editor->find(query, flags);
}