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

    Rectangle {
        id: labelRectangle

        gradient: Gradient {
                GradientStop { position: 0.0; color: "red" }
                GradientStop { position: 0.33; color: "yellow" }
                GradientStop { position: 1.0; color: "green" }
            }
        width: parent.width
        height: parent.height / 10

        Rectangle {
            id: textRectangle
            width: 100
            height: labelRectangle.height / 2

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

        gradient: Gradient {
                     GradientStop {
                         position: 0.0
                         SequentialAnimation on color {
                             loops: Animation.Infinite
                             ColorAnimation { from: "DeepSkyBlue"; to: "#0E1533"; duration: 5000 }
                             ColorAnimation { from: "#0E1533"; to: "DeepSkyBlue"; duration: 5000 }
                         }
                     }
                     GradientStop {
                         position: 1.0
                         SequentialAnimation on color {
                             loops: Animation.Infinite
                             ColorAnimation { from: "SkyBlue"; to: "#437284"; duration: 5000 }
                             ColorAnimation { from: "#437284"; to: "SkyBlue"; duration: 5000 }
                         }
                     }
                 }


        GameBoard {
            id: game
            modelGame: GameBoardData {}
        }
    }
}
