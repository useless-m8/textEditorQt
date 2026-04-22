#pragma once

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QVector>

struct HighlightRule {
    QRegularExpression pattern;
    QTextCharFormat format;
};

class SyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    explicit SyntaxHighlighter(QTextDocument *parent = nullptr);

    void setLanguageByFileName(const QString &fileName);
    void setHighlightingEnabled(bool enabled);

protected:
    void highlightBlock(const QString &text) override;

private:
    void setupCppRules();
    void setupPythonRules();
    void clearRules();

    QVector<HighlightRule> m_rules;
    bool m_enabled = true;

    QTextCharFormat m_keywordFormat;
    QTextCharFormat m_stringFormat;
    QTextCharFormat m_commentFormat;
    QTextCharFormat m_numberFormat;
};