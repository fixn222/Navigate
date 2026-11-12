#pragma once

#include <QAbstractListModel>
#include <QDateTime>

struct FileEntry {
    QString name;
    QString path;
    bool directory = false;
    qint64 size = 0;
    QDateTime modified;
};

class FileListModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { NameRole = Qt::UserRole + 1, PathRole, DirectoryRole, SizeRole, ModifiedRole };
    explicit FileListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    void setDirectory(const QString &path);
    const FileEntry *entryAt(int row) const;

private:
    QList<FileEntry> m_entries;
};
