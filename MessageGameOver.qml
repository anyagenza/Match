import QtQuick 2.14
import QtQuick.Dialogs 1.1

MessageDialog {
    id: messageDialog
    title: "GameOver"
    text: "Try again?"
    onAccepted: {
        close();
    }
    Component.onCompleted: visible = true
    standardButtons: StandardButton.Yes | StandardButton.No
    onNo: Qt.quit();
}
