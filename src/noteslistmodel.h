#ifndef NOTESLISTMODEL_H
#define NOTESLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include "note.h"
#include "database.h" // Include Database

class NotesListModel : public QAbstractListModel
{
    Q_OBJECT
    // Expose Database to QML if direct calls are needed (e.g. for simple add/delete from QML)
    // Or keep Database private and provide specific Q_INVOKABLE methods in NotesListModel
    Q_PROPERTY(Database* database READ database CONSTANT)

public:
    explicit NotesListModel(QObject *parent = nullptr);

    enum NoteRoles {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        ContentRole,
        ColorRole,
        IsChecklistRole,
        ChecklistItemsRole, // Will be a QVariantList
        CreatedRole,
        ModifiedRole,
        PinnedRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void loadNotes();
    Q_INVOKABLE bool addNote(const QString &title, const QString &content, const QString &color, bool isChecklist, const QVariantList &checklistItems);
    Q_INVOKABLE bool updateNote(int id, const QString &title, const QString &content, const QString &color, bool isChecklist, const QVariantList &checklistItems, bool pinned);
    Q_INVOKABLE bool deleteNote(int id);
    Q_INVOKABLE QVariant getNote(int id); // Returns a QVariantMap for easier QML use
    Q_INVOKABLE void togglePinNote(int id);


private:
    QList<Note> m_notes;
    Database* m_db; // Own the database object
};

#endif // NOTESLISTMODEL_H
