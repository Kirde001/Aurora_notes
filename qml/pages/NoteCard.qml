import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
    id: noteCardRoot
    width: ListView.view.width // Or GridView.cellWidth
    height: Theme.itemSizeMedium * 2

    property int noteId
    property string noteTitle
    property string noteContent
    property string noteColor: "white"
    property bool notePinned: false
    property bool isChecklist: false
    property var checklistItems: [] // Expects a list of JS objects: [{text: "...", checked: ...}]

    signal pinClicked()

    Rectangle {
        id: cardBackground
        anchors.fill: parent
        color: Theme.rgba(noteColor, notePinned ? 0.8 : 0.6) // Use noteColor, slightly different if pinned
        border.color: Theme.rgba(noteColor, 1.0)
        border.width: 1
        radius: Theme.paddingSmall
    }

    Column {
        anchors.fill: parent
        anchors.margins: Theme.paddingMedium
        spacing: Theme.paddingSmall

        Row {
            width: parent.width
            spacing: Theme.paddingSmall
            Label {
                id: titleLabel
                text: noteTitle
                font.pixelSize: Theme.fontSizeLarge
                font.bold: true
                color: Theme.highlightColor
                elide: Text.ElideRight
                width: parent.width - pinButton.width - Theme.paddingSmall
            }
            IconButton {
                id: pinButton
                icon.source: notePinned ? "image://theme/icon-s-pinned" : "image://theme/icon-s-unpinned"
                icon.height: Theme.iconSizeSmall
                icon.width: Theme.iconSizeSmall
                anchors.verticalCenter: titleLabel.verticalCenter
                onClicked: {
                    noteCardRoot.pinClicked()
                    // Prevent click from propagating to the card itself if needed
                    // mouse.accepted = true
                }
            }
        }


        if (isChecklist && checklistItems && checklistItems.length > 0) {
            ListView {
                id: checklistPreview
                height: Math.min(parent.height - titleLabel.height - Theme.paddingMedium * 3, Theme.itemSizeSmall * checklistItems.length)
                width: parent.width
                interactive: false // Not scrollable within the card
                model: checklistItems
                delegate: Row {
                    spacing: Theme.paddingSmall
                    width: parent.width
                    Image {
                        source: modelData.checked ? "image://theme/icon-s-checked" : "image://theme/icon-s-unchecked"
                        height: Theme.fontSizeSmall
                        width: Theme.fontSizeSmall
                        anchors.verticalCenter: itemText.verticalCenter
                    }
                    Label {
                        id: itemText
                        text: modelData.text
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                        elide: Text.ElideRight
                        width: parent.width - Theme.iconSizeSmall - Theme.paddingSmall
                    }
                }
            }
        } else {
            Label {
                text: noteContent
                wrapMode: Text.WordWrap
                elide: Text.ElideRight
                maximumLineCount: 3 // Preview
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                width: parent.width
            }
        }
    }
}
