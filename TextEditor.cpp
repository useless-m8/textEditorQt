#include "TextEditor.h"

#include <QFileInfo>
#include <QFrame>
#include <QPainter>
#include <QResizeEvent>
#include <QTextBlock>
#include <QTextEdit>

class TextEditor::LineNumberArea : public QWidget {
public:
    explicit LineNumberArea(TextEditor *editor)
        : QWidget(editor), m_editor(editor) {}

    QSize sizeHint() const override
    {
        return QSize(m_editor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        m_editor->lineNumberAreaPaintEvent(event);
    }

private:
    TextEditor *m_editor;
};

TextEditor::TextEditor(QWidget *parent)
    : QPlainTextEdit(parent),
    m_lineNumberArea(new LineNumberArea(this))
{
    setFrameShape(QFrame::NoFrame);
    setTabStopDistance(fontMetrics().horizontalAdvance(' ') * 4);

    connect(this, &QPlainTextEdit::blockCountChanged, this, &TextEditor::updateLineNumberAreaWidth);
    connect(this, &QPlainTextEdit::updateRequest, this, &TextEditor::updateLineNumberArea);
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &TextEditor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

QString TextEditor::filePath() const
{
    return m_filePath;
}

void TextEditor::setFilePath(const QString &path)
{
    m_filePath = path;
}

QString TextEditor::fileName() const
{
    if (m_filePath.isEmpty()) {
        return "untitled.txt";
    }
    return QFileInfo(m_filePath).fileName();
}

QString TextEditor::encoding() const
{
    return m_encoding;
}

void TextEditor::setEncoding(const QString &encoding)
{
    m_encoding = encoding;
}

bool TextEditor::modified() const
{
    return document()->isModified();
}

void TextEditor::setModified(bool modified)
{
    document()->setModified(modified);
}

int TextEditor::currentLine() const
{
    return textCursor().blockNumber() + 1;
}

int TextEditor::currentColumn() const
{
    return textCursor().positionInBlock() + 1;
}

void TextEditor::setEditorFontSize(int size)
{
    QFont f = font();
    f.setPointSize(size);
    setFont(f);
    updateLineNumberAreaWidth(0);
}

int TextEditor::editorFontSize() const
{
    return font().pointSize();
}

int TextEditor::lineNumberAreaWidth() const
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    return 12 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
}

void TextEditor::updateLineNumberAreaWidth(int)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void TextEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy) {
        m_lineNumberArea->scroll(0, dy);
    } else {
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect())) {
        updateLineNumberAreaWidth(0);
    }
}

void TextEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    const QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void TextEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(m_currentLineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void TextEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), QColor("#252526"));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            const QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor("#858585"));
            painter.drawText(0, top, m_lineNumberArea->width() - 6, fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void TextEditor::setCurrentLineColor(const QColor &color)
{
    m_currentLineColor = color;
    highlightCurrentLine();
}