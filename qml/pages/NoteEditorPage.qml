import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.template.qt_notes_app.models 1.0 // For notesListModel

Page {
    id: noteEditorPage
    property bool isExistingNote: false
    property int currentNoteId: -1
    property string currentTitle: ""
    property string currentContent: ""
    property string currentColor: "white" // Default color from Theme or a list
    property bool currentIsChecklist: false
    property var currentChecklistItems: [] // Array of JS objects: { text: "...", checked: false }
    property bool currentPinned: false


    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height + Theme.paddingLarge

        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingMedium
            PageHeader {
                title: isExistingNote ? qsTr("Edit Note") : qsTr("New Note")
            }

            TextField {
                id: titleField
                width: parent.width
                placeholderText: qsTr("Title")
                text: currentTitle
                font.pixelSize: Theme.fontSizeLarge
                anchors.horizontalCenter: parent.horizontalCenter
            }

            TextArea {
                id: contentField
                width: parent.width
                height: Math.max(Theme.itemSizeLarge * 2, noteEditorPage.height * 0.3) // Adjust height
                placeholderText: qsTr("Content")
                text: currentContent
                font.pixelSize: Theme.fontSizeMedium
                anchors.horizontalCenter: parent.horizontalCenter
                visible: !currentIsChecklist // Show if not a checklist
            }

            // Checklist Editor
            Column {
                id: checklistEditor
                width: parent.width
                spacing: Theme.paddingSmall
                visible: currentIsChecklist

                Label {
                    text: qsTr("Checklist Items:")
                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.secondaryHighlightColor
                }

                Repeater {
                    model: currentChecklistItems
                    delegate: Row {
                        width: parent.width
                        spacing: Theme.paddingSmall

                        CheckBox {
                            id: itemCheckbox
                            checked: modelData.checked
                            onClicked: currentChecklistItems[index].checked = checked
                        }
                        TextField {
                            text: modelData.text
                            placeholderText: qsTr("List item")
                            width: parent.width - itemCheckbox.width - deleteItemButton.width - Theme.paddingMedium * 2
                            onTextChanged: currentChecklistItems[index].text = text
                        }
                        IconButton {
                            id: deleteItemButton
                            icon.source: "image://theme/icon-m-delete"
                            onClicked: currentChecklistItems.splice(index, 1) // Remove item
                        }
                    }
                }
                Button {
                    text: qsTr("Add Item")
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
                        currentChecklistItems.push({ text: "", checked: false })
                        // Force Repeater update if necessary by reassigning
                        currentChecklistItems = currentChecklistItems.slice()
                    }
                }
            }


            Row {
                width: parent.width
                spacing: Theme.paddingMedium
                anchors.horizontalCenter: parent.horizontalCenter
                CheckBox {
                    id: checklistToggle
                    text: qsTr("Is Checklist")
                    checked: currentIsChecklist
                    onCheckedChanged: currentIsChecklist = checked
                }
                // Simple Color Picker (can be improved with a dialog)
                ComboBox {
                    id: colorPicker
                    label: qsTr("Color")
                    width: parent.width / 2 - Theme.paddingSmall
                    currentIndex: availableColors.indexOf(currentColor) >=0 ? availableColors.indexOf(currentColor) : 0
                    menu: ContextMenu {
                        Repeater {
                            model: availableColors
                            MenuItem { text: modelData }
                        }
                    }
                    onCurrentIndexChanged: currentColor = availableColors[currentIndex]
                    ListModel { id: availableColorsModel } // Filled in Component.onCompleted
                }
            }


            Button {
                id: saveButton
                text: qsTr("Save")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    var finalChecklistItems = []
                    if (currentIsChecklist) {
                        for (var i = 0; i < currentChecklistItems.length; ++i) {
                            // Filter out empty items before saving
                            if (currentChecklistItems[i].text.trim() !== "") {
                                finalChecklistItems.push(currentChecklistItems[i])
                            }
                        }
                    }

                    if (isExistingNote) {
                        notesListModel.updateNote(currentNoteId, titleField.text, contentField.text, currentColor, currentIsChecklist, finalChecklistItems, currentPinned)
                    } else {
                        notesListModel.addNote(titleField.text, contentField.text, currentColor, currentIsChecklist, finalChecklistItems)
                    }
                    pageStack.pop()
                }
            }

            // Optional: Delete button if editing an existing note
            Button {
                text: qsTr("Delete")
                anchors.horizontalCenter: parent.horizontalCenter
                visible: isExistingNote
                backgroundColor: Theme.negativeColor
                onClicked: {
                    // Add a confirmation dialog here
                    var dialog = pageStack.push("Sailfish.Silica.ConfirmationDialog",
                                                {
                                                    title: qsTr("Delete Note"),
                                                    message: qsTr("Are you sure you want to delete this note?")
                                                });
                    dialog.accepted.connect(function() {
                        notesListModel.deleteNote(currentNoteId);
                        pageStack.pop(); // Pop the editor page
                    });
                }
            }
        }
    }

    // Available colors for the ComboBox
    property list<string> availableColors: [
        "white", "LightBlue", "LightGreen", "LightYellow", "LightPink", "LightGray",
        "SkyBlue", "PaleGreen", "Khaki", "Pink", "Silver"
    ]


    Component.onCompleted: {
        if (isExistingNote && currentNoteId !== -1) {
            var noteData = notesListModel.getNote(currentNoteId)
            if (noteData && noteData.noteId !== undefined) { // Check if noteData is valid
                currentTitle = noteData.noteTitle
                currentContent = noteData.noteContent
                currentColor = noteData.noteColor
                currentIsChecklist = noteData.noteIsChecklist
                currentChecklistItems = JSON.parse(JSON.stringify(noteData.noteChecklistItems)) // Deep copy for editing
                currentPinned = noteData.notePinned
            } else {
                console.log("Error: Could not load note with ID " + currentNoteId + " for editing.")
                // Optionally show an error to the user or pop the page
                pageStack.pop()
            }
        }
        // Fill the color picker model
        for (var i = 0; i < availableColors.length; ++i) {
            availableColorsModel.append({ text: availableColors[i] })
        }
        if (currentColor) { // Set initial selection if color exists
            var idx = availableColors.indexOf(currentColor)
            if (idx !== -1) colorPicker.currentIndex = idx
        }
    }
}
