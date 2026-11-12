#include "backend.h"

#include <QDesktopServices>
#include <QGuiApplication>
#include <QClipboard>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QUrl>

Backend::Backend(QObject *parent) : QObject(parent), m_currentPath(QDir::homePath()), m_entries(this), m_tree(this) { refresh(); }

QString Backend::homePath() const { return QDir::homePath(); }

void Backend::setStatus(const QString &message) { if (m_status != message) { m_status = message; emit statusChanged(); } }

void Backend::refresh() { m_entries.setDirectory(m_currentPath); setStatus({}); }

void Backend::open(const QString &path) {
    const QFileInfo info(path);
    if (!info.exists()) { setStatus(tr("This item no longer exists.")); return; }
    if (info.isDir()) {
        m_currentPath = info.canonicalFilePath();
        m_entries.setDirectory(m_currentPath);
        setStatus({});
        emit currentPathChanged();
    } else if (!QDesktopServices::openUrl(QUrl::fromLocalFile(info.absoluteFilePath()))) {
        setStatus(tr("No application can open this file."));
    }
}

void Backend::up() { QDir dir(m_currentPath); if (dir.cdUp()) open(dir.absolutePath()); }

void Backend::createFolder(const QString &name) {
    const QString clean = name.trimmed();
    if (clean.isEmpty() || clean == "." || clean == ".." || clean.contains('/')) { setStatus(tr("Enter a valid folder name.")); return; }
    QDir dir(m_currentPath);
    if (!dir.mkdir(clean)) { setStatus(tr("Could not create the folder. It may already exist.")); return; }
    refresh();
}

void Backend::rename(const QString &path, const QString &name) {
    const QString clean = name.trimmed();
    QFileInfo info(path);
    if (clean.isEmpty() || clean.contains('/') || !info.exists()) { setStatus(tr("Enter a valid name.")); return; }
    const QString destination = info.dir().filePath(clean);
    if (QFileInfo::exists(destination) || !QFile::rename(path, destination)) { setStatus(tr("Could not rename this item.")); return; }
    refresh();
}

void Backend::trash(const QString &path) {
    QFile file(path);
    if (!file.moveToTrash()) { setStatus(tr("Could not move this item to the trash.")); return; }
    refresh();
}

void Backend::reveal(const QString &path) { QDesktopServices::openUrl(QUrl::fromLocalFile(path)); }

void Backend::copyPath(const QString &path) { QGuiApplication::clipboard()->setText(path); setStatus(tr("Path copied.")); }

void Backend::toggleTree(const QString &path) { m_tree.toggle(path); }
