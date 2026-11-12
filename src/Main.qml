import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

ApplicationWindow {
    id: win
    width: 1220; height: 760; minimumWidth: 820; minimumHeight: 540
    visible: true; title: backend.currentPath + " — Navigate"; color: "#101114"
    Material.theme: Material.Dark; Material.accent: theme.accent; Material.roundedScale: theme.radius
    property int selectedIndex: -1
    property bool helpVisible: false
    Shortcut { sequence: "?"; context: Qt.ApplicationShortcut; onActivated: helpVisible = !helpVisible }
    Shortcut { sequence: "Q"; context: Qt.ApplicationShortcut; onActivated: Qt.quit() }
    Shortcut { sequence: "Backspace"; context: Qt.ApplicationShortcut; onActivated: backend.up() }
    Shortcut { sequence: "Ctrl+R"; context: Qt.ApplicationShortcut; onActivated: backend.refresh() }
    Shortcut { sequence: "Ctrl+N"; context: Qt.ApplicationShortcut; onActivated: folderDialog.open() }
    Shortcut { sequence: "Delete"; context: Qt.ApplicationShortcut; onActivated: { if (selectedPath()) trashDialog.open() } }
    Shortcut { sequence: "F2"; context: Qt.ApplicationShortcut; onActivated: { if (selectedPath()) renameDialog.open() } }
    Shortcut { sequence: "Return"; context: Qt.ApplicationShortcut; onActivated: { if (selectedPath()) backend.open(selectedPath()) } }

    header: ToolBar {
        height: 62; Material.background: "#17181c"
        RowLayout { anchors.fill: parent; anchors.leftMargin: 18; anchors.rightMargin: 18; spacing: 10
            Label { text: "N"; font.bold: true; font.pixelSize: 20; color: theme.accent; Layout.rightMargin: 8 }
            ToolButton { text: "‹"; font.pixelSize: 30; enabled: backend.currentPath !== "/"; onClicked: backend.up() }
            ToolButton { text: "↻"; onClicked: backend.refresh() }
            Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 38; radius: Math.min(6, theme.radius); color: "#24262d"; border.color: "#31333a"
                Text { anchors.fill: parent; anchors.leftMargin: 14; anchors.rightMargin: 14; verticalAlignment: Text.AlignVCenter; text: backend.currentPath; color: "#f0f1f5"; elide: Text.ElideMiddle }
            }
            ToolButton { text: "+"; font.pixelSize: 22; onClicked: folderDialog.open() }
            ToolButton { text: "⋮"; font.pixelSize: 23; onClicked: appMenu.open() }
        }
    }

    SplitView {
        anchors.fill: parent; orientation: Qt.Horizontal
        Rectangle {
            SplitView.preferredWidth: 260; SplitView.minimumWidth: 185; color: "#17181c"
            ColumnLayout { anchors.fill: parent; anchors.margins: 12; spacing: 8
                Label { text: qsTr("LOCATIONS"); color: "#8f939f"; font.pixelSize: 11; font.bold: true; Layout.leftMargin: 8; Layout.topMargin: 4 }
                ItemDelegate { Layout.fillWidth: true; text: "⌂   " + qsTr("Home"); onClicked: backend.open(backend.homePath) }
                ItemDelegate { Layout.fillWidth: true; text: "▣   " + qsTr("Computer"); onClicked: backend.open("/") }
                Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: "#303139"; Layout.topMargin: 8; Layout.bottomMargin: 8 }
                Label { text: qsTr("FOLDERS"); color: "#8f939f"; font.pixelSize: 11; font.bold: true; Layout.leftMargin: 8 }
                ListView {
                    id: treeView; Layout.fillWidth: true; Layout.fillHeight: true; clip: true; model: backend.tree
                    ScrollBar.vertical: ScrollBar {}
                    delegate: ItemDelegate {
                        id: treeDelegate
                        required property string name; required property string path; required property int depth; required property bool expanded; required property bool hasChildren
                        width: treeView.width; height: 34; leftPadding: 8 + depth * 16; highlighted: backend.currentPath === path
                        contentItem: RowLayout { spacing: 5
                            Label { width: 15; text: treeDelegate.hasChildren ? (treeDelegate.expanded ? "⌄" : "›") : ""; color: "#aeb1bc" }
                            Label { text: "▸"; color: theme.accent; font.pixelSize: 15 }
                            Label { text: treeDelegate.name; Layout.fillWidth: true; elide: Text.ElideRight }
                        }
                        onClicked: backend.open(path)
                        onDoubleClicked: { if (hasChildren) backend.toggleTree(path) }
                    }
                }
            }
        }
        Rectangle {
            SplitView.fillWidth: true; color: "#101114"
            ColumnLayout { anchors.fill: parent; spacing: 0
                Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 40; color: "#15161a"
                    RowLayout { anchors.fill: parent; anchors.leftMargin: 20; anchors.rightMargin: 20
                        Label { text: qsTr("Name"); Layout.fillWidth: true; color: "#979ba8"; font.bold: true; font.pixelSize: 12 }
                        Label { text: qsTr("Modified"); Layout.preferredWidth: 170; color: "#979ba8"; font.bold: true; font.pixelSize: 12 }
                        Label { text: qsTr("Size"); Layout.preferredWidth: 90; horizontalAlignment: Text.AlignRight; color: "#979ba8"; font.bold: true; font.pixelSize: 12 }
                    }
                }
                ListView {
                    id: fileView; Layout.fillWidth: true; Layout.fillHeight: true; clip: true; model: backend.entries; currentIndex: win.selectedIndex
                    ScrollBar.vertical: ScrollBar {}
                    delegate: ItemDelegate {
                        id: row
                        required property string name; required property string path; required property bool directory; required property var size; required property var modified
                        property string itemPath: path
                        width: fileView.width; height: 58; highlighted: ListView.isCurrentItem
                        onClicked: { win.selectedIndex = index; fileView.currentIndex = index }
                        onDoubleClicked: backend.open(path)
                        onPressAndHold: { win.selectedIndex = index; fileView.currentIndex = index; contextMenu.popup() }
                        contentItem: RowLayout { spacing: 13
                            Rectangle { Layout.preferredWidth: 32; Layout.preferredHeight: 32; radius: 8; color: row.directory ? Qt.rgba(1, .84, .04, .14) : "#262831"
                                Label { anchors.centerIn: parent; text: row.directory ? "▰" : "◆"; color: row.directory ? theme.accent : "#b9bdc9"; font.pixelSize: 17 }
                            }
                            ColumnLayout { Layout.fillWidth: true; spacing: 2
                                Label { text: row.name; Layout.fillWidth: true; elide: Text.ElideRight; color: "#f3f4f7" }
                                Label { text: row.directory ? qsTr("Folder") : qsTr("File"); color: "#9296a3"; font.pixelSize: 11 }
                            }
                            Label { Layout.preferredWidth: 170; text: row.modified.toLocaleString(Qt.locale(), Locale.ShortFormat); color: "#afb2bc"; font.pixelSize: 12 }
                            Label { Layout.preferredWidth: 90; horizontalAlignment: Text.AlignRight; text: row.directory ? "—" : byteSize(row.size); color: "#afb2bc"; font.pixelSize: 12 }
                        }
                        MouseArea { anchors.fill: parent; acceptedButtons: Qt.RightButton; onClicked: mouse => { win.selectedIndex = index; fileView.currentIndex = index; contextMenu.popup() } }
                    }
                    Label { anchors.centerIn: parent; visible: fileView.count === 0; text: qsTr("This folder is empty"); color: "#9296a3" }
                }
                Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 34; color: "#17181c"
                    RowLayout { anchors.fill: parent; anchors.leftMargin: 18; anchors.rightMargin: 18
                        Label { text: backend.status.length ? backend.status : qsTr("%1 items").arg(fileView.count); color: backend.status.length ? "#ffb4ab" : "#9498a5"; font.pixelSize: 12 }
                        Item { Layout.fillWidth: true }
                        Label { text: qsTr("Right-click for actions"); color: "#777b87"; font.pixelSize: 12 }
                    }
                }
            }
        }
    }

    function byteSize(bytes) { if (bytes < 1024) return bytes + " B"; if (bytes < 1048576) return (bytes / 1024).toFixed(1) + " KB"; if (bytes < 1073741824) return (bytes / 1048576).toFixed(1) + " MB"; return (bytes / 1073741824).toFixed(1) + " GB" }
    function selectedName() { return fileView.currentItem ? fileView.currentItem.name : "" }
    function selectedPath() { return fileView.currentItem ? fileView.currentItem.itemPath : "" }

    Menu { id: contextMenu
        MenuItem { text: qsTr("Open"); enabled: selectedPath() !== ""; onTriggered: backend.open(selectedPath()) }
        MenuItem { text: qsTr("Open in default application"); enabled: selectedPath() !== ""; onTriggered: backend.reveal(selectedPath()) }
        MenuSeparator {}
        MenuItem { text: qsTr("Rename"); enabled: selectedPath() !== ""; onTriggered: renameDialog.open() }
        MenuItem { text: qsTr("Copy path"); enabled: selectedPath() !== ""; onTriggered: backend.copyPath(selectedPath()) }
        MenuSeparator {}
        MenuItem { text: qsTr("Move to trash"); enabled: selectedPath() !== ""; onTriggered: trashDialog.open() }
    }
    Menu { id: appMenu
        MenuItem { text: qsTr("New folder"); onTriggered: folderDialog.open() }
        MenuItem { text: qsTr("Refresh"); onTriggered: backend.refresh() }
        MenuSeparator {}
        MenuItem { text: qsTr("Keyboard shortcuts"); onTriggered: helpVisible = true }
    }
    Dialog { id: folderDialog; title: qsTr("New folder"); modal: true; anchors.centerIn: parent; standardButtons: Dialog.Ok | Dialog.Cancel
        onOpened: { folderName.text = ""; folderName.forceActiveFocus() }
        onAccepted: backend.createFolder(folderName.text)
        TextField { id: folderName; width: 320; placeholderText: qsTr("Folder name"); selectByMouse: true }
    }
    Dialog { id: renameDialog; title: qsTr("Rename"); modal: true; anchors.centerIn: parent; standardButtons: Dialog.Ok | Dialog.Cancel
        onOpened: { renameName.text = selectedName(); renameName.selectAll(); renameName.forceActiveFocus() }
        onAccepted: backend.rename(selectedPath(), renameName.text)
        TextField { id: renameName; width: 320; selectByMouse: true }
    }
    Dialog { id: trashDialog; title: qsTr("Move to trash?"); modal: true; anchors.centerIn: parent; standardButtons: Dialog.Yes | Dialog.Cancel
        Label { width: 340; wrapMode: Text.Wrap; text: qsTr("Move “%1” to the trash?").arg(selectedName()) }
        onAccepted: backend.trash(selectedPath())
    }
    Popup { visible: helpVisible; anchors.centerIn: parent; width: 400; height: 310; modal: true; padding: 24; closePolicy: Popup.NoAutoClose
        background: Rectangle { color: "#25262c"; radius: theme.radius }
        Column { spacing: 12
            Label { text: qsTr("Keyboard shortcuts"); font.bold: true; font.pixelSize: 20 }
            Label { text: "Enter    " + qsTr("Open") + "\nBackspace  " + qsTr("Parent folder") + "\nCtrl+N   " + qsTr("New folder") + "\nF2       " + qsTr("Rename") + "\nDelete   " + qsTr("Move to trash") + "\nCtrl+R   " + qsTr("Refresh") + "\nQ        " + qsTr("Quit") + "\n?        " + qsTr("Close this help") }
        }
    }
}
