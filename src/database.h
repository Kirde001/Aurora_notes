#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>
#include "note.h" // Include your Note class definition

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    bool openDb();
    void closeDb();

    Q_INVOKABLE bool addNote(const QString &title, const QString &content, const QString &color = "white", bool isChecklist = false, const QVariantList &checklistItems = QVariantList());
    Q_INVOKABLE bool updateNote(int id, const QString &title, const QString &content, const QString &color, bool isChecklist, const QVariantList &checklistItems, bool pinned);
    Q_INVOKABLE bool deleteNote(int id);
    Q_INVOKABLE QList<Note> getAllNotes();
    Q_INVOKABLE Note getNoteById(int id);
    Q_INVOKABLE bool setNotePinned(int id, bool pinned);


private:
    QSqlDatabase m_db;
    bool initDb();
};

#endif // DATABASE_H
