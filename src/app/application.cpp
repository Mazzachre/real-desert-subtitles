#include "application.h"

Rd::Application::Application::Application(QObject *parent)
: QObject(parent)
, m_engine{new QQmlApplicationEngine}
, m_window{nullptr}
, m_dropTarget{new DropTarget}
, m_fileSearch{new FileSearch} {

    qmlRegisterSingletonInstance("RealDesert", 1, 0, "DropTarget", m_dropTarget);
    qmlRegisterSingletonInstance("RealDesert", 1, 0, "FileSearch", m_fileSearch);

    connect(m_dropTarget, &Rd::Application::DropTarget::fileDropped, m_fileSearch, &Rd::Application::FileSearch::find);
}

Rd::Application::Application::~Application() {
    delete m_fileSearch;
    delete m_dropTarget;
    delete m_window;
    delete m_engine;
}

void Rd::Application::Application::start(const QRect& dimensions) {
    m_engine->load(QUrl("qrc:/res/qml/main.qml"));
    if (m_engine->rootObjects().isEmpty()) qFatal("Unable to parse QML file");
    m_window = qobject_cast<QQuickWindow*>(m_engine->rootObjects().first());
    if (!m_window) qFatal("No root window created");

    m_window->setIcon(QIcon(":/res/images/desert-logo.svg"));
    m_window->setHeight(dimensions.height()/2);
    m_window->setWidth(dimensions.width()/2);
    m_window->show();
}
