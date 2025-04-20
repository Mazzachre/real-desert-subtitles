#include "ui.h"
#include <QQmlContext>

Rd::Ui::Ui::Ui(QObject* parent)
: QObject(parent)
, m_engine{new QQmlApplicationEngine}
, m_window{nullptr}
, m_features{new FeatureModel}
, m_subtitles{new SubtitleModel}
, m_target{new DropTarget}
, m_selected{new SelectedFeature}
, m_search{new Rd::Library::SubtitleFinder}
, m_download{new Rd::Library::SubtitleDownloader} {

    QQmlContext* context = ((QQmlEngine *)m_engine)->rootContext();
    context->setContextProperty("DropTarget", m_target);
    context->setContextProperty("FeatureModel", m_features);
    context->setContextProperty("SubtitleModel", m_subtitles);
    context->setContextProperty("SelectedFeature", m_selected);
    context->setContextProperty("App", this);

    m_engine->load(QUrl("qrc:/res/qml/main.qml"));
    if (m_engine->rootObjects().isEmpty()) qFatal("Unable to parse QML file");
    m_window = qobject_cast<QQuickWindow*>(m_engine->rootObjects().first());
    if (!m_window) qFatal("No root window created");

    ((QWindow *)m_window)->setIcon(QIcon(":/res/images/desert-logo.svg"));

    connect(m_target, &DropTarget::fileDropped, m_search, &Rd::Library::SubtitleFinder::findByFile);
    connect(m_target, &DropTarget::fileDropped, this, &Ui::fileSelected);

    connect(m_search, &Rd::Library::SubtitleFinder::subtitlesFound, m_features, &FeatureModel::setFeatures);
    connect(m_search, &Rd::Library::SubtitleFinder::subtitlesFound, this, &Ui::subtitlesFound);
    connect(m_search, &Rd::Library::SubtitleFinder::noSubtitlesFound, this, &Ui::noSubtitlesFound);
    connect(m_search, &Rd::Library::SubtitleFinder::error, this, &Ui::handleError);

}

Rd::Ui::Ui::~Ui() noexcept {
    ((QObject*)m_selected)->deleteLater();
    ((QObject*)m_features)->deleteLater();
    ((QObject*)m_subtitles)->deleteLater();
    ((QObject*)m_target)->deleteLater();
    ((QObject*)m_search)->deleteLater();
    ((QObject*)m_download)->deleteLater();
    delete m_window;
    delete m_engine;
}

Rd::Ui::Ui::Mode Rd::Ui::Ui::mode() {
    return m_mode;
}

QString Rd::Ui::Ui::file() {
    return m_file.fileName();
}

void Rd::Ui::Ui::searchMovie(const QString& title, const QString& year) {
    //TODO validate?
    m_search->findMovie(title, year);
}

void Rd::Ui::Ui::searchShow(const QString& title, const QString& season, const QString& episode) {
    //TODO validate?
    m_search->findShow(title, season, episode);
}

void Rd::Ui::Ui::selectFeature(quint64 id) {
    const Feature& feature = m_features->getFeature(id);
    Q_EMIT fileIdentified(m_file, feature);

    m_subtitles->setSubtitles(feature.subtitles);
    m_selected->setSelected(feature);
    m_features->clear();

    m_mode = Selected;
    Q_EMIT modeUpdated();
}

void Rd::Ui::Ui::selectSubtitle(quint64 id) {
    m_download->download(id);
}

void Rd::Ui::Ui::clear() {
    m_file.clear();
    Q_EMIT fileUpdated();

    m_selected->clear();
    m_features->clear();
    m_subtitles->clear();

    m_mode = Search;
    Q_EMIT modeUpdated();
}

void Rd::Ui::Ui::show(const QRect& dimensions) {
    ((QWindow *)m_window)->setHeight(dimensions.height()/2);
    ((QWindow *)m_window)->setWidth(dimensions.width()/2);
    ((QWindow *)m_window)->setMinimumHeight(dimensions.height()/2);
    ((QWindow *)m_window)->setMinimumWidth(dimensions.width()/2);
    ((QWindow *)m_window)->show();
}

void Rd::Ui::Ui::hide() {
    ((QWindow *)m_window)->hide();
}

void Rd::Ui::Ui::fileSelected(const QUrl& file) {
    m_file = file;
    Q_EMIT fileUpdated();

    m_mode = Working;
    Q_EMIT modeUpdated();
}

void Rd::Ui::Ui::subtitlesFound(const QList<Feature>&) {
    m_mode = Selecting;
    Q_EMIT modeUpdated();
}

void Rd::Ui::Ui::noSubtitlesFound() {
    m_mode = Retry;
    Q_EMIT modeUpdated();
}

void Rd::Ui::Ui::handleError(const QString& head, const QString& body) {
    //TODO ERROR
    qDebug() << head << body << Qt::endl;
}
