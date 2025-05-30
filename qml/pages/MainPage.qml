import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.template.qt_notes_app.models 1.0 // Import your model

Page {
    id: mainPage
    objectName: "mainPage"
    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: notesGrid.height + Theme.paddingLarge // Ensure grid scrolls

        PullDownMenu {
            MenuItem {
                text: qsTr("Add Note")
                onClicked: pageStack.push(Qt.resolvedUrl("NoteEditorPage.qml"))
            }
            MenuItem {
                text: qsTr("Refresh Notes") // For debugging
                onClicked: notesListModel.loadNotes()
            }
        }

        PageHeader {
            id: pageHeader
            title: qsTr("My Notes")
            // Removed the About button for now, can be added back to PullDownMenu or a dedicated settings page
        }

        SilicaGridView {
            id: notesGrid
            anchors.top: pageHeader.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: Theme.paddingMedium
            cellWidth: width / 2 - Theme.paddingSmall // Example: 2 columns
            cellHeight: Theme.itemSizeMedium * 2.5
            model: notesListModel // Use the C++ model instance

            delegate: NoteCard {
                width: notesGrid.cellWidth
                height: notesGrid.cellHeight - Theme.paddingSmall
                noteId: model.noteId
                noteTitle: model.noteTitle
                noteContent: model.noteContent
                noteColor: model.noteColor
                notePinned: model.notePinned
                isChecklist: model.noteIsChecklist // Pass isChecklist
                checklistItems: model.noteChecklistItems // Pass checklistItems

                onClicked: {
                    pageStack.push(Qt.resolvedUrl("NoteEditorPage.qml"), {
                                       "noteId": model.noteId,
                                       "isExistingNote": true
                                   })
                }
                onPinClicked: notesListModel.togglePinNote(model.noteId)
            }

            VerticalScrollDecorator {}
        }

        // Floating Action Button for adding notes
        IconButton {
            icon.source: "image://theme/icon-m-add"
            anchors {
                right: parent.right
                bottom: parent.bottom
                margins: Theme.paddingLarge
            }
            onClicked: pageStack.push(Qt.resolvedUrl("NoteEditorPage.qml"))
            z: 1 // Ensure it's above the grid
        }
    }

    Component.onCompleted: {
        notesListModel.loadNotes()
    }
}
