import QtQuick 2.14
import gameboard.data 1.0

GridView {
    id: view
    cellWidth: width / 5
    cellHeight: height / 8
    clip: true
    property var firstClickedElement: -1
    property var secondClickedElement: -1
    flow: GridView.FlowTopToBottom
    interactive: false
    model: GameBoardData { id: model }

    Connections {
            target: model
            onNoMatch: {
               Qt.quit();
            }
        }


    function check(index){
        if (firstClickedElement == -1) {
            firstClickedElement = index;
            return false;
        } else {
            secondClickedElement = index;
            return true;
        }
    }

    function step() {
        view.model.swapElements(firstClickedElement, secondClickedElement);
        firstClickedElement = -1;
        secondClickedElement = -1;
    }
    function forClick(index) {
        if (check(index))
        {
            step()
        }
    }

    delegate: Item {
        width: view.cellWidth
        height: view.cellHeight

        Tile {
            id: currTile
            //anchors.fill: parent
            color: model.display
            height: parent.height
            width: height
            radius: width / 2
            Text {
                text: index
            }

            MouseArea {
                anchors.fill: parent
                onClicked: forClick(index)
            }



        }



    }

    add: Transition {
        NumberAnimation { properties: "y"; from: -100; duration: 4000;
                          easing.type: Easing.InBack; alwaysRunToEnd: true}
        onRunningChanged: {
            if (!running) {
                console.error("^^^ finished");
                view.model.clearMatchAgain();
            }
        }
    }

    displaced: Transition {
        NumberAnimation { properties: "y"; duration: 4000; easing.type: Easing.InBack
        }
    }

    remove: Transition {
        NumberAnimation { properties: "width"; from: view.cellWidth; to: 0; duration: 2000; easing.type: Easing.InBack }
        NumberAnimation { properties: "height"; from: view.cellHeight; to: 0; duration: 2000; easing.type: Easing.InBack }
    }

//    removeDisplaced: Transition {
//        NumberAnimation { properties: "x,y"; duration: 2000; easing.type: Easing.InBack }
//    }

//    populate: Transition {
//        NumberAnimation { properties: "x,y"; duration: 2000; easing.type: Easing.InBack }
//    }
}
