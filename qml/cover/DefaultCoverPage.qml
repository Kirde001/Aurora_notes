import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.template.qt_notes_app.models 1.0 // Import your model

CoverBackground {
    Label {
        id: label
        anchors.centerIn: parent
        text: notesListModel.rowCount() + " " + (notesListModel.rowCount() === 1 ? qsTr("Note") : qsTr("Notes"))
        color: Theme.secondaryHighlightColor // Or Theme.highlightColor depending on cover action
    }

    CoverActionList {
        id: coverActionList
        CoverAction {
            iconSource: "image://theme/icon-m-add"
            onTriggered: {
                // This is tricky from cover; ideally, the app opens to NoteEditorPage.
                // For simplicity, just launches the app. Advanced handling would involve
                // signals or specific launch arguments.
                Qt.application.activate()
            }
        }
    }
    // Ensure notesListModel is accessible here
    // If not, you might need to fetch count via a C++ signal/property available globally or re-fetch.
    // For this example, we assume notesListModel instance from main.cpp is available.
    // If it doesn't update dynamically, you'd need to connect to model's signals.
}
