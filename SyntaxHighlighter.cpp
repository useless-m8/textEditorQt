#include "SyntaxHighlighter.h"

#include <QFileInfo>

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {
    m_keywordFormat.setForeground(QColor("#569cd6"));
    m_keywordFormat.setFontWeight(QFont::Bold);

    m_stringFormat.setForeground(QColor("#ce9178"));
    m_commentFormat.setForeground(QColor("#6a9955"));
    m_numberFormat.setForeground(QColor("#b5cea8"));
}

void SyntaxHighlighter::setLanguageByFileName(const QString &fileName) {
    clearRules();
    const QString suffix = QFileInfo(fileName).suffix().toLower();

    if (suffix == "cpp" || suffix == "cc" || suffix == "cxx" || suffix == "h" || suffix == "hpp") {
        setupCppRules();
    } else if (suffix == "py") {
        setupPythonRules();
    }

    rehighlight();
}

void SyntaxHighlighter::setHighlightingEnabled(bool enabled) {
    m_enabled = enabled;
    rehighlight();
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    if (!m_enabled) {
        return;
    }

    for (const HighlightRule &rule : std::as_const(m_rules)) {
        auto it = rule.pattern.globalMatch(text);
        while (it.hasNext()) {
            const auto match = it.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}

void SyntaxHighlighter::setupCppRules() {
    const QStringList keywords = {
        "class", "public", "private", "protected", "virtual", "override", "const",
        "int", "double", "float", "bool", "void", "return", "if", "else", "for",
        "while", "switch", "case", "break", "continue", "include", "auto", "QString",
        "signals", "slots", "nullptr", "new", "delete", "this"
    };

    for (const QString &kw : keywords) {
        m_rules.push_back({QRegularExpression("\\b" + QRegularExpression::escape(kw) + "\\b"), m_keywordFormat});
    }

    m_rules.push_back({QRegularExpression(R"(".*?")"), m_stringFormat});
    m_rules.push_back({QRegularExpression(R"(//[^\n]*)"), m_commentFormat});
    m_rules.push_back({QRegularExpression(R"(\b\d+\b)"), m_numberFormat});
}

void SyntaxHighlighter::setupPythonRules() {
    const QStringList keywords = {
        "def", "class", "import", "from", "return", "if", "elif", "else", "for",
        "while", "True", "False", "None", "try", "except", "with", "as", "pass", "lambda"
    };

    for (const QString &kw : keywords) {
        m_rules.push_back({QRegularExpression("\\b" + QRegularExpression::escape(kw) + "\\b"), m_keywordFormat});
    }

    m_rules.push_back({QRegularExpression(R"(".*?"|'.*?')"), m_stringFormat});
    m_rules.push_back({QRegularExpression(R"(#[^\n]*)"), m_commentFormat});
    m_rules.push_back({QRegularExpression(R"(\b\d+\b)"), m_numberFormat});
}

void SyntaxHighlighter::clearRules() {
    m_rules.clear();
}