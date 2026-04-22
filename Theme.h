#pragma once
#include <QString>
#include <QColor>

struct Theme {
    QString name;
    QString appStyleSheet;
    QString editorStyleSheet;
    QColor currentLineColor;
};