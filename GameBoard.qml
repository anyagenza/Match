import QtQuick 2.14
import gameboard.data 1.0

GridView {
    id: view
    cellWidth: width / view.model.m_sizeX
    cellHeight: height / view.model.m_sizeY
    clip: true
    property var firstClickedElement: -1
    property var secondClickedElement: -1
    property var firstElement
    property var secondElement
    property var modelGame : ({})
    flow: GridView.FlowTopToBottom
    interactive: false
    model: modelGame
    anchors.fill: parent

    Connections {
        target:    modelGame
        onNoMatch: {
            firstElement = view.itemAtIndex(firstClickedElement);
            secondElement = view.itemAtIndex(secondClickedElement);
            animationBad.start();
        }
    }

    SequentialAnimation {
        id: animationBad
        NumberAnimation { targets: [firstElement ,secondElement]; property: "opacity"; to: 0.2; duration: 500}
        NumberAnimation { targets: [firstElement, secondElement]; property: "opacity"; to: 1; duration: 500 }
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
        id: currItem
        width: view.cellWidth
        height: view.cellHeight

        Tile {
            id: currTile
            color: model.display
            height: width
            width: currItem.height
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
        NumberAnimation { properties: "y"; from: -view.height/3 + y - 100 ; duration: 1000;
            easing.type: Easing.InBack; alwaysRunToEnd: true}
        onRunningChanged: {
            if (!running) {
                view.model.clearMatchAgain();
            }
        }
    }

    move: Transition {

        NumberAnimation { properties: "x,y"; duration: 1000;  }
        onRunningChanged: {
            if (!running) {
                view.model.clearMatchAgain();
            }
        }
    }

    displaced: Transition {
        NumberAnimation { properties: "y"; duration: 1000; easing.type: Easing.InBack }
    }

    remove: Transition {
        NumberAnimation { properties: "scale"; to: 0; duration: 500; easing.type: Easing.InBack }
    }
}
