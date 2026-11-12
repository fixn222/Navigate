#pragma once

#include <QObject>

#include "filelistmodel.h"
#include "treemodel.h"

class Backend : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString currentPath READ currentPath NOTIFY currentPathChanged)
    Q_PROPERTY(QString homePath READ homePath CONSTANT)
    Q_PROPERTY(FileListModel *entries READ entries CONSTANT)
    Q_PROPERTY(TreeModel *tree READ tree CONSTANT)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
public:
    explicit Backend(QObject *parent = nullptr);
    QString currentPath() const { return m_currentPath; }
    QString homePath() const;
    FileListModel *entries() { return &m_entries; }
    TreeModel *tree() { return &m_tree; }
    QString status() const { return m_status; }
    Q_INVOKABLE void open(const QString &path);
    Q_INVOKABLE void up();
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void createFolder(const QString &name);
    Q_INVOKABLE void rename(const QString &path, const QString &name);
    Q_INVOKABLE void trash(const QString &path);
    Q_INVOKABLE void reveal(const QString &path);
    Q_INVOKABLE void copyPath(const QString &path);
    Q_INVOKABLE void toggleTree(const QString &path);
signals:
    void currentPathChanged();
    void statusChanged();
private:
    void setStatus(const QString &message);
    QString m_currentPath;
    QString m_status;
    FileListModel m_entries;
    TreeModel m_tree;
};
