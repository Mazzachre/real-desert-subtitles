#include "application.h"
#include <QQmlContext>

Rd::Application::Application::Application(QObject *parent)
: QObject(parent)
, m_engine{new QQmlApplicationEngine}
, m_window{nullptr}
, m_dropTarget{new DropTarget}
, m_fileSearch{new FileSearch}
, m_features{new FeatureModel}
, m_selected{new SelectedFeature}
, m_subtitles{new SubtitleModel} {

    QQmlContext* context = ((QQmlEngine *)m_engine)->rootContext();
    context->setContextProperty("DropTarget", m_dropTarget);
    context->setContextProperty("FileSearch", m_fileSearch);
    context->setContextProperty("FeatureModel", m_features);
    context->setContextProperty("SelectedFeature", m_selected);
    context->setContextProperty("SubtitleModel", m_subtitles);

    connect(m_dropTarget, &Rd::Application::DropTarget::fileDropped, m_fileSearch, &Rd::Application::FileSearch::find);
    connect(m_fileSearch, &Rd::Application::FileSearch::subtitlesFound, m_features, &Rd::Application::FeatureModel::setResults);
    connect(m_features, &Rd::Application::FeatureModel::featureSelected, m_selected, &Rd::Application::SelectedFeature::setSelected);
    connect(m_features, &Rd::Application::FeatureModel::featureSelected, m_subtitles, &Rd::Application::SubtitleModel::setSelected);
}

Rd::Application::Application::~Application() {
    ((QObject*)m_features)->deleteLater();
    ((QObject*)m_fileSearch)->deleteLater();
    ((QObject*)m_dropTarget)->deleteLater();
    ((QObject*)m_selected)->deleteLater();
    delete m_window;
    delete m_engine;
}

void Rd::Application::Application::start(const QRect& dimensions) {
    m_engine->load(QUrl("qrc:/res/qml/main.qml"));
    if (m_engine->rootObjects().isEmpty()) qFatal("Unable to parse QML file");
    m_window = qobject_cast<QQuickWindow*>(m_engine->rootObjects().first());
    if (!m_window) qFatal("No root window created");

    ((QWindow *)m_window)->setIcon(QIcon(":/res/images/desert-logo.svg"));
    ((QWindow *)m_window)->setHeight(dimensions.height()/2);
    ((QWindow *)m_window)->setWidth(dimensions.width()/2);
    ((QWindow *)m_window)->show();
}
