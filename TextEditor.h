#pragma once

#include <QPlainTextEdit>

class TextEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit TextEditor(QWidget *parent = nullptr);

    QString filePath() const;
    void setFilePath(const QString &path);

    QString fileName() const;

    QString encoding() const;
    void setEncoding(const QString &encoding);

    bool modified() const;
    void setModified(bool modified);

    int currentLine() const;
    int currentColumn() const;

    void setEditorFontSize(int size);
    int editorFontSize() const;

    int lineNumberAreaWidth() const;
    void lineNumberAreaPaintEvent(QPaintEvent *event);

    void setCurrentLineColor(const QColor &color);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int);
    void updateLineNumberArea(const QRect &rect, int dy);
    void highlightCurrentLine();

private:
    class LineNumberArea;
    LineNumberArea *m_lineNumberArea;
    QString m_filePath;
    QString m_encoding = "UTF-8";
    QColor m_currentLineColor = QColor("#2a2d2e");
};