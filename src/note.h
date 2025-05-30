#ifndef NOTE_H
#define NOTE_H

#include <QString>
#include <QDateTime>
#include <QVariantList> // For checklist items

class Note {
public:
    Note(int id = -1,
         const QString &title = "",
         const QString &content = "",
         const QString &color = "white", // Default color
         bool isChecklist = false,
         const QVariantList &checklistItems = QVariantList(), // List of maps: [{"text": "item1", "checked": false}, ...]
         const QDateTime &created = QDateTime::currentDateTime(),
         const QDateTime &modified = QDateTime::currentDateTime(),
         bool pinned = false)
        : m_id(id), m_title(title), m_content(content), m_color(color),
          m_isChecklist(isChecklist), m_checklistItems(checklistItems),
          m_created(created), m_modified(modified), m_pinned(pinned) {}

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    QString title() const { return m_title; }
    void setTitle(const QString &title) { m_title = title; }

    QString content() const { return m_content; }
    void setContent(const QString &content) { m_content = content; }

    QString color() const { return m_color; }
    void setColor(const QString &color) { m_color = color; }

    bool isChecklist() const { return m_isChecklist; }
    void setIsChecklist(bool isChecklist) { m_isChecklist = isChecklist; }

    QVariantList checklistItems() const { return m_checklistItems; }
    void setChecklistItems(const QVariantList &items) { m_checklistItems = items; }

    QDateTime created() const { return m_created; }
    void setCreated(const QDateTime &created) { m_created = created; }

    QDateTime modified() const { return m_modified; }
    void setModified(const QDateTime &modified) { m_modified = modified; }

    bool pinned() const { return m_pinned; }
    void setPinned(bool pinned) { m_pinned = pinned; }

private:
    int m_id;
    QString m_title;
    QString m_content;
    QString m_color;
    bool m_isChecklist;
    QVariantList m_checklistItems;
    QDateTime m_created;
    QDateTime m_modified;
    bool m_pinned;
};

#endif // NOTE_H
