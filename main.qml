import QtQuick 2.14
import QtQuick.Window 2.14
import gameboard.data 1.0

Window {
    visible: true
    width: 480
    height: 640
    title: qsTr("Hello World")

    Rectangle {
        id: labelRectangle
        width: parent.width
        height: parent.height / 10

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
            modelGame: GameBoardData {}

        }

//        GameBoard {
//            anchors.fill: parent
//            id: game
//            model: GameBoardData {

//                onNoMatch: {
//                    var firstElement = game.itemAtIndex(first);
//                    var secondElement = game.itemAtIndex(second);
//                    firstElement.state = "blink";
//                    secondElement.state = "blink";
//                    //firstElement.scale = 1;
//                    //firstElement.height = 40
//                    //game.trembling(firstElement, secondElement);
//                }
//            }

//        }
    }
}
