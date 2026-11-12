#include "theme.h"

#include <QColor>
#include <QDir>
#include <QFile>
#include <QRegularExpression>
#include <cmath>

Theme::Theme(QObject *parent) : QObject(parent) {
    const QString directory = QDir::homePath() + "/.local/state/omarchy/current/theme";
    m_watcher.addPath(QDir::homePath() + "/.local/state/omarchy/current");
    if (QFile::exists(directory + "/colors.toml")) m_watcher.addPath(directory + "/colors.toml");
    connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this, &Theme::reload);
    connect(&m_watcher, &QFileSystemWatcher::fileChanged, this, &Theme::reload);
    reload();
}
void Theme::reload() {
    QFile file(QDir::homePath() + "/.local/state/omarchy/current/theme/colors.toml");
    QString next = "#FFD60A";
    if (file.open(QIODevice::ReadOnly)) {
        const auto match = QRegularExpression(R"(^\s*accent\s*=\s*[\"'](#[0-9a-fA-F]{6})[\"'])", QRegularExpression::MultilineOption).match(QString::fromUtf8(file.readAll()));
        if (match.hasMatch()) next = match.captured(1);
    }
    if (next != m_accent) { m_accent = next; emit changed(); }
}
QString Theme::accentForeground() const { const QColor c(m_accent); const auto f=[](double v){return v<=.04045?v/12.92:std::pow((v+.055)/1.055,2.4);}; return .2126*f(c.redF())+.7152*f(c.greenF())+.0722*f(c.blueF())>.179 ? "black" : "white"; }
