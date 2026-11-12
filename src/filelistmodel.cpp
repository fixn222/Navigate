#include "filelistmodel.h"

#include <QDir>
#include <QFileInfo>

FileListModel::FileListModel(QObject *parent) : QAbstractListModel(parent) {}

int FileListModel::rowCount(const QModelIndex &parent) const { return parent.isValid() ? 0 : m_entries.size(); }

QVariant FileListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_entries.size()) return {};
    const auto &item = m_entries.at(index.row());
    switch (role) {
    case NameRole: return item.name;
    case PathRole: return item.path;
    case DirectoryRole: return item.directory;
    case SizeRole: return item.size;
    case ModifiedRole: return item.modified;
    default: return {};
    }
}

QHash<int, QByteArray> FileListModel::roleNames() const {
    return {{NameRole, "name"}, {PathRole, "path"}, {DirectoryRole, "directory"},
            {SizeRole, "size"}, {ModifiedRole, "modified"}};
}

void FileListModel::setDirectory(const QString &path) {
    beginResetModel();
    m_entries.clear();
    QDir directory(path);
    const auto infos = directory.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden,
                                               QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);
    for (const QFileInfo &info : infos)
        m_entries.append({info.fileName(), info.absoluteFilePath(), info.isDir(), info.size(), info.lastModified()});
    endResetModel();
}

const FileEntry *FileListModel::entryAt(int row) const {
    return row >= 0 && row < m_entries.size() ? &m_entries.at(row) : nullptr;
}
