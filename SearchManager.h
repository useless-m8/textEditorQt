#pragma once

#include <QObject>

class TextEditor;

class SearchManager : public QObject {
    Q_OBJECT
public:
    explicit SearchManager(QObject *parent = nullptr);

    bool findNext(TextEditor *editor, const QString &query, bool caseSensitive = false);
    bool findPrevious(TextEditor *editor, const QString &query, bool caseSensitive = false);
};