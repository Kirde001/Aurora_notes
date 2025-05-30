#include "noteslistmodel.h"
#include <QDebug>

NotesListModel::NotesListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_db = new Database(this); // Database will be child of model
    loadNotes();
}

Database* NotesListModel::database() const { // Getter for Q_PROPERTY
    return m_db;
}

int NotesListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_notes.count();
}

QVariant NotesListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_notes.count())
        return QVariant();

    const Note &note = m_notes.at(index.row());
    switch (role) {
    case IdRole: return note.id();
    case TitleRole: return note.title();
    case ContentRole: return note.content();
    case ColorRole: return note.color();
    case IsChecklistRole: return note.isChecklist();
    case ChecklistItemsRole: return note.checklistItems();
    case CreatedRole: return note.created();
    case ModifiedRole: return note.modified();
    case PinnedRole: return note.pinned();
    default: return QVariant();
    }
}

QHash<int, QByteArray> NotesListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "noteId";
    roles[TitleRole] = "noteTitle";
    roles[ContentRole] = "noteContent";
    roles[ColorRole] = "noteColor";
    roles[IsChecklistRole] = "noteIsChecklist";
    roles[ChecklistItemsRole] = "noteChecklistItems";
    roles[CreatedRole] = "noteCreated";
    roles[ModifiedRole] = "noteModified";
    roles[PinnedRole] = "notePinned";
    return roles;
}

void NotesListModel::loadNotes()
{
    beginResetModel();
    m_notes = m_db->getAllNotes();
    endResetModel();
}

bool NotesListModel::addNote(const QString &title, const QString &content, const QString &color, bool isChecklist, const QVariantList &checklistItems)
{
    bool success = m_db->addNote(title, content, color, isChecklist, checklistItems);
    if (success) {
        loadNotes(); // Reload all notes
    }
    return success;
}

bool NotesListModel::updateNote(int id, const QString &title, const QString &content, const QString &color, bool isChecklist, const QVariantList &checklistItems, bool pinned)
{
    bool success = m_db->updateNote(id, title, content, color, isChecklist, checklistItems, pinned);
    if (success) {
        loadNotes(); // Reload for simplicity, could be optimized to update single item
    }
    return success;
}

bool NotesListModel::deleteNote(int id)
{
    bool success = m_db->deleteNote(id);
    if (success) {
        loadNotes(); // Reload
    }
    return success;
}

QVariant NotesListModel::getNote(int id) {
    Note note = m_db->getNoteById(id);
    if (note.id() != -1) { // Check if note is valid
        QVariantMap map;
        map.insert("noteId", note.id());
        map.insert("noteTitle", note.title());
        map.insert("noteContent", note.content());
        map.insert("noteColor", note.color());
        map.insert("noteIsChecklist", note.isChecklist());
        map.insert("noteChecklistItems", note.checklistItems());
        map.insert("noteCreated", note.created());
        map.insert("noteModified", note.modified());
        map.insert("notePinned", note.pinned());
        return map;
    }
    return QVariantMap(); // Return empty map if not found
}

void NotesListModel::togglePinNote(int id) {
    Note note = m_db->getNoteById(id);
    if (note.id() != -1) {
        bool success = m_db->setNotePinned(id, !note.pinned());
        if (success) {
            loadNotes();
        }
    }
}
