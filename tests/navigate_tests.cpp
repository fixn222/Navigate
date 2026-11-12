#include <QDir>
#include <QTemporaryDir>
#include <QtTest>
#include "filelistmodel.h"

class NavigateTests : public QObject {
    Q_OBJECT
private slots:
    void listsDirectoriesBeforeFiles() {
        QTemporaryDir temp; QVERIFY(QDir(temp.path()).mkdir("folder"));
        QFile file(temp.filePath("note.txt")); QVERIFY(file.open(QIODevice::WriteOnly));
        FileListModel model; model.setDirectory(temp.path());
        QCOMPARE(model.rowCount(), 2);
        QCOMPARE(model.data(model.index(0, 0), FileListModel::NameRole).toString(), QString("folder"));
        QVERIFY(model.data(model.index(0, 0), FileListModel::DirectoryRole).toBool());
    }
};
QTEST_MAIN(NavigateTests)
#include "navigate_tests.moc"
