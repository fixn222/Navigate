#include "treemodel.h"

#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

TreeModel::TreeModel(QObject *parent) : QAbstractListModel(parent) {
    m_expanded.insert(QDir::homePath());
    refresh();
}
int TreeModel::rowCount(const QModelIndex &parent) const { return parent.isValid() ? 0 : m_entries.size(); }
QVariant TreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_entries.size()) return {};
    const auto &entry = m_entries.at(index.row());
    switch (role) { case NameRole: return entry.name; case PathRole: return entry.path; case DepthRole: return entry.depth;
    case ExpandedRole: return entry.expanded; case HasChildrenRole: return entry.hasChildren; default: return {}; }
}
QHash<int, QByteArray> TreeModel::roleNames() const { return {{NameRole,"name"},{PathRole,"path"},{DepthRole,"depth"},{ExpandedRole,"expanded"},{HasChildrenRole,"hasChildren"}}; }
void TreeModel::appendChildren(const QString &path, int depth) {
    const auto directories = QDir(path).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden, QDir::IgnoreCase | QDir::Name);
    for (const QFileInfo &info : directories) {
        const QString child = info.canonicalFilePath();
        if (child.isEmpty()) continue;
        const bool expanded = m_expanded.contains(child);
        const bool hasChildren = !QDir(child).entryList(QDir::Dirs | QDir::NoDotAndDotDot).isEmpty();
        m_entries.append({info.fileName(), child, depth, expanded, hasChildren});
        if (expanded) appendChildren(child, depth + 1);
    }
}
void TreeModel::refresh() {
    beginResetModel(); m_entries.clear();
    const QString home = QDir::homePath();
    const bool homeExpanded = m_expanded.contains(home);
    m_entries.append({tr("Home"), home, 0, homeExpanded, true});
    if (homeExpanded) appendChildren(home, 1);
    const bool rootExpanded = m_expanded.contains("/");
    m_entries.append({tr("Computer"), "/", 0, rootExpanded, true});
    if (rootExpanded) appendChildren("/", 1);
    endResetModel();
}
void TreeModel::toggle(const QString &path) { if (m_expanded.contains(path)) m_expanded.remove(path); else m_expanded.insert(path); refresh(); }
