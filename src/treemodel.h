#pragma once

#include <QAbstractListModel>
#include <QSet>

struct TreeEntry { QString name; QString path; int depth; bool expanded; bool hasChildren; };

class TreeModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { NameRole = Qt::UserRole + 1, PathRole, DepthRole, ExpandedRole, HasChildrenRole };
    explicit TreeModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    void toggle(const QString &path);
    void refresh();
private:
    void appendChildren(const QString &path, int depth);
    QList<TreeEntry> m_entries;
    QSet<QString> m_expanded;
};
