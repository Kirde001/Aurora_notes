#include <auroraapp.h>
#include <QtQuick>
#include "src/noteslistmodel.h" // Add this
#include "src/database.h"     // Add this for qmlRegisterType if needed, or for direct use

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> application(Aurora::Application::application(argc, argv));
    application->setOrganizationName(QStringLiteral("ru.template"));
    application->setApplicationName(QStringLiteral("qt_notes_app"));

    // Register C++ types for QML
    qmlRegisterType<NotesListModel>("ru.template.qt_notes_app.models", 1, 0, "NotesListModel");
    // qmlRegisterType<Database>("ru.template.qt_notes_app.db", 1, 0, "Database"); // If you want to instantiate Database directly in QML

    QScopedPointer<QQuickView> view(Aurora::Application::createView());

    // Expose an instance of NotesListModel to QML
    NotesListModel notesListModel;
    view->rootContext()->setContextProperty("notesListModel", &notesListModel);


    view->setSource(Aurora::Application::pathTo(QStringLiteral("qml/qt_notes_app.qml")));
    view->show();

    return application->exec();
}
