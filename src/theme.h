#pragma once

#include <QFileSystemWatcher>
#include <QObject>

class Theme : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString accent READ accent NOTIFY changed)
    Q_PROPERTY(QString accentForeground READ accentForeground NOTIFY changed)
    Q_PROPERTY(int radius READ radius NOTIFY changed)
public:
    explicit Theme(QObject *parent = nullptr);
    QString accent() const { return m_accent; }
    QString accentForeground() const;
    int radius() const { return 0; }
signals: void changed();
private: void reload(); QString m_accent = "#FFD60A"; QFileSystemWatcher m_watcher;
};
