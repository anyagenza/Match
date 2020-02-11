import QtQuick 2.14
import QtQuick.Window 2.14
import gameboard.data 1.0

Window {
    id: windowMain
    visible: true
    width: 480
    height: 640

    property var windowWidth : width;
    property var windowHeight : height;
    property var rectHeigth: height

    Rectangle {
        id: labelRectangle
        color: "black"
        width: parent.width
        height: parent.height / 10

        Rectangle {
            id: textRectangle
            width: 100
            height: labelRectangle.height / 2
            color: "white"
            x: parent.width / 12
            anchors.verticalCenter: button.verticalCenter
            border.color: "indianred"
            border.width: 2
            Text {
                anchors.centerIn: textRectangle
                color: "indianred"
                text: game.model.score.toString()
            }
        }

        Rectangle {
            id: checkGameOver
            width: 100
            height: labelRectangle.height / 2
            color: "white"
            x: parent.width / 1.5
            anchors.verticalCenter: button.verticalCenter
            border.color: "indianred"
            border.width: 2
            MouseArea {
                anchors.fill: parent
                //onClicked: game.model.ifGameOver()
            }
        }


        Button {
            id: button
            width: 100
            height: labelRectangle.height / 2
            anchors.centerIn: parent

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onClicked: game.model.shuffle()
            }
            states: State {
                name: "pressed"; when: mouseArea.pressed
                PropertyChanges { target: button; scale: 1.2 }
            }
        }
    }

    Rectangle {
        id: rectBoard
        y: labelRectangle.height
        width: parent.width
        height: parent.height - labelRectangle.height

        GameBoard {
            id: game
            modelGame: GameBoardData {}
        }
    }
}
