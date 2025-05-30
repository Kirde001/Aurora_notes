TARGET = ru.template.qt_notes_app

 CONFIG += \
     auroraapp
+CONFIG += console
QT += sql

 PKGCONFIG += \

 SOURCES += \
     src/main.cpp \
+    src/database.cpp \
+    src/noteslistmodel.cpp

 HEADERS += \
+    src/note.h \
+    src/database.h \
+    src/noteslistmodel.h

 DISTFILES += \
    qml/pages/NoteCard.qml \
    qml/pages/NoteEditorPage.qml \
     rpm/ru.template.qt_notes_app.spec \

 AURORAAPP_ICONS = 86x86 108x108 128x128 172x172

 CONFIG += auroraapp_i18n

 TRANSLATIONS += \
     translations/ru.template.qt_notes_app.ts \
     translations/ru.template.qt_notes_app-ru.ts
