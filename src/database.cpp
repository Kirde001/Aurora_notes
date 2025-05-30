#include "database.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

Database::Database(QObject *parent) : QObject(parent)
{
    if (!openDb()) {
        qWarning() << "Failed to open database!";
    }
}

Database::~Database()
{
    closeDb();
}

bool Database::openDb()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", "notesConnection"); // Named connection
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir dir(path);
    if (!dir.exists())
        dir.mkpath(".");
    m_db.setDatabaseName(path + "/notes.sqlite");

    if (!m_db.open()) {
        qWarning() << "Error: connection with database failed:" << m_db.lastError().text();
        return false;
    }
    return initDb();
}

void Database::closeDb()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
    QSqlDatabase::removeDatabase("notesConnection"); // Remove named connection
}

bool Database::initDb()
{
    if (!m_db.isOpen()) {
        qWarning() << "Database is not open for initialization.";
        return false;
    }

    QSqlQuery query(m_db);
    bool success = query.exec("CREATE TABLE IF NOT EXISTS notes ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "title TEXT, "
                              "content TEXT, "
                              "color TEXT DEFAULT 'white', "
                              "is_checklist INTEGER DEFAULT 0, "
                              "checklist_items TEXT, " // Store as JSON string
                              "created_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
                              "modified_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
                              "pinned INTEGER DEFAULT 0"
                              ")");
    if (!success) {
        qWarning() << "Failed to create table:" << query.lastError().text();
        return false;
    }

    // Add trigger to update modified_at timestamp
    success = query.exec("CREATE TRIGGER IF NOT EXISTS update_modified_at "
                         "AFTER UPDATE ON notes "
                         "FOR EACH ROW "
                         "BEGIN "
                         "  UPDATE notes SET modified_at = CURRENT_TIMESTAMP WHERE id = OLD.id; "
                         "END;");
    if (!success) {
        qWarning() << "Failed to create trigger:" << query.lastError().text();
        // Not critical, can proceed
    }

    return true;
}

bool Database::addNote(const QString &title, const QString &content, const QString &color, bool isChecklist, const QVariantList &checklistItems)
{
    if (!m_db.isOpen()) return false;
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO notes (title, content, color, is_checklist, checklist_items, created_at, modified_at) "
                  "VALUES (:title, :content, :color, :is_checklist, :checklist_items, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)");
    query.bindValue(":title", title);
    query.bindValue(":content", content);
    query.bindValue(":color", color);
    query.bindValue(":is_checklist", isChecklist ? 1 : 0);
    query.bindValue(":checklist_items", QJsonDocument::fromVariant(checklistItems).toJson(QJsonDocument::Compact)); // Store QVariantList as JSON

    if (!query.exec()) {
        qWarning() << "addNote error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::updateNote(int id, const QString &title, const QString &content, const QString &color, bool isChecklist, const QVariantList &checklistItems, bool pinned)
{
    if (!m_db.isOpen()) return false;
    QSqlQuery query(m_db);
    query.prepare("UPDATE notes SET title = :title, content = :content, color = :color, "
                  "is_checklist = :is_checklist, checklist_items = :checklist_items, pinned = :pinned, modified_at = CURRENT_TIMESTAMP "
                  "WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":title", title);
    query.bindValue(":content", content);
    query.bindValue(":color", color);
    query.bindValue(":is_checklist", isChecklist ? 1 : 0);
    query.bindValue(":checklist_items", QJsonDocument::fromVariant(checklistItems).toJson(QJsonDocument::Compact));
    query.bindValue(":pinned", pinned ? 1 : 0);

    if (!query.exec()) {
        qWarning() << "updateNote error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::deleteNote(int id)
{
    if (!m_db.isOpen()) return false;
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM notes WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qWarning() << "deleteNote error:" << query.lastError().text();
        return false;
    }
    return query.numRowsAffected() > 0;
}

QList<Note> Database::getAllNotes()
{
    QList<Note> notes;
    if (!m_db.isOpen()) return notes;
    QSqlQuery query("SELECT id, title, content, color, is_checklist, checklist_items, created_at, modified_at, pinned FROM notes ORDER BY pinned DESC, modified_at DESC", m_db);

    while (query.next()) {
        QJsonDocument doc = QJsonDocument::fromJson(query.value("checklist_items").toByteArray());
        notes.append(Note(
            query.value("id").toInt(),
            query.value("title").toString(),
            query.value("content").toString(),
            query.value("color").toString(),
            query.value("is_checklist").toBool(),
            doc.toVariant().toList(),
            query.value("created_at").toDateTime(),
            query.value("modified_at").toDateTime(),
            query.value("pinned").toBool()
        ));
    }
    return notes;
}

Note Database::getNoteById(int id) {
    Note note;
    if (!m_db.isOpen()) return note; // Return invalid note
    QSqlQuery query(m_db);
    query.prepare("SELECT id, title, content, color, is_checklist, checklist_items, created_at, modified_at, pinned FROM notes WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        QJsonDocument doc = QJsonDocument::fromJson(query.value("checklist_items").toByteArray());
        note = Note(
            query.value("id").toInt(),
            query.value("title").toString(),
            query.value("content").toString(),
            query.value("color").toString(),
            query.value("is_checklist").toBool(),
            doc.toVariant().toList(),
            query.value("created_at").toDateTime(),
            query.value("modified_at").toDateTime(),
            query.value("pinned").toBool()
        );
    } else {
        qWarning() << "getNoteById error or note not found:" << query.lastError().text();
    }
    return note;
}


bool Database::setNotePinned(int id, bool pinned) {
    if (!m_db.isOpen()) return false;
    QSqlQuery query(m_db);
    query.prepare("UPDATE notes SET pinned = :pinned, modified_at = CURRENT_TIMESTAMP WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":pinned", pinned ? 1 : 0);

    if (!query.exec()) {
        qWarning() << "setNotePinned error:" << query.lastError().text();
        return false;
    }
    return true;
}
