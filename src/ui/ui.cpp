#include "ui.h"
#include <QQmlContext>
#include <QFileInfo>

Rd::Ui::Ui::Ui(QObject* parent)
: QObject(parent)
, m_engine{new QQmlApplicationEngine}
, m_window{nullptr}
, m_features{new FeatureModel}
, m_subtitles{new SubtitleModel}
, m_target{new DropTarget}
, m_selected{new SelectedFeature}
, m_config{new Config}
, m_search{new Rd::Library::SubtitleFinder}
, m_download{new Rd::Library::SubtitleDownloader}
, m_saver{new Rd::Library::FileSaver} {

    QQmlContext* context = ((QQmlEngine *)m_engine)->rootContext();
    context->setContextProperty("DropTarget", m_target);
    context->setContextProperty("FeatureModel", m_features);
    context->setContextProperty("SubtitleModel", m_subtitles);
    context->setContextProperty("SelectedFeature", m_selected);
    context->setContextProperty("Config", m_config);
    context->setContextProperty("App", this);

    m_engine->load(QUrl("qrc:/res/qml/main.qml"));
    if (m_engine->rootObjects().isEmpty()) qFatal("Unable to parse QML file");
    m_window = qobject_cast<QQuickWindow*>(m_engine->rootObjects().first());
    if (!m_window) qFatal("No root window created");

    ((QWindow *)m_window)->setIcon(QIcon(":/res/images/desert-logo.svg"));

    connect(m_target, &DropTarget::fileDropped, this, &Ui::fileSelected);

    connect(m_search, &Rd::Library::SubtitleFinder::subtitlesFound, m_features, &FeatureModel::setFeatures);
    connect(m_search, &Rd::Library::SubtitleFinder::subtitlesFound, this, &Ui::subtitlesFound);
    connect(m_search, &Rd::Library::SubtitleFinder::noSubtitlesFound, this, &Ui::noSubtitlesFound);
    connect(m_search, &Rd::Library::SubtitleFinder::error, this, &Ui::handleError);

    connect(m_download, &Rd::Library::SubtitleDownloader::found, this, &Ui::subtitleFile);
    connect(m_download, &Rd::Library::SubtitleDownloader::usage, this, &Ui::handleUsageReport);
    connect(m_download, &Rd::Library::SubtitleDownloader::error, this, &Ui::handleError);

    connect(m_saver, &Rd::Library::FileSaver::done, this, &Ui::done);
}

Rd::Ui::Ui::~Ui() noexcept {
    ((QObject*)m_selected)->deleteLater();
    ((QObject*)m_config)->deleteLater();
    ((QObject*)m_features)->deleteLater();
    ((QObject*)m_subtitles)->deleteLater();
    ((QObject*)m_target)->deleteLater();
    ((QObject*)m_search)->deleteLater();
    ((QObject*)m_download)->deleteLater();
    ((QObject*)m_saver)->deleteLater();
    delete m_window;
    delete m_engine;
}

Rd::Ui::Ui::Mode Rd::Ui::Ui::mode() {
    return m_mode;
}

QString Rd::Ui::Ui::file() {
    return m_file.fileName();
}

quint32 Rd::Ui::Ui::remaining() {
    return m_remaining;
}

QString Rd::Ui::Ui::reset() {
    return m_reset;
}

QString Rd::Ui::Ui::target() {
    return m_targetFile;
}

void Rd::Ui::Ui::searchMovie(const QString& title, const QString& year) {
    m_search->findMovie(title, year);
}

void Rd::Ui::Ui::searchShow(const QString& title, const QString& season, const QString& episode) {
    m_search->findShow(title, season, episode);
}

void Rd::Ui::Ui::selectFeature(quint64 id) {
    const Feature& feature = m_features->getFeature(id);
    Q_EMIT fileIdentified(m_file, feature);

    m_subtitles->setSubtitles(feature.subtitles);
    m_selected->setSelected(feature);

    m_mode = Selected;
    Q_EMIT modeUpdated();
}

void Rd::Ui::Ui::selectSubtitle(quint64 id) {
    m_download->download(id);
    m_subtitles->markDownloaded(id);
}

void Rd::Ui::Ui::clear() {
    m_remaining = 0;
    m_reset.clear();
    Q_EMIT usageUpdated();

    m_targetFile.clear();
    Q_EMIT targetUpdated();

    m_file.clear();
    Q_EMIT fileUpdated();

    m_selected->clear();
    m_features->clear();
    m_subtitles->clear();

    m_mode = Search;
    Q_EMIT modeUpdated();
}

void Rd::Ui::Ui::stay() {
    m_remaining = 0;
    m_reset.clear();
    Q_EMIT usageUpdated();

    m_mode = Selected;
    Q_EMIT modeUpdated();
}

void Rd::Ui::Ui::finish() {
    Q_EMIT finished();
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

    m_search->findByFile(file);
}

void Rd::Ui::Ui::subtitlesFound(const QList<Feature>& features) {
    if (features.size() == 1) {
        const Feature& feature = features[0];
        Q_EMIT fileIdentified(m_file, feature);

        m_subtitles->setSubtitles(feature.subtitles);
        m_selected->setSelected(feature);

        m_mode = Selected;
    } else {
        m_mode = Selecting;
    }

    Q_EMIT modeUpdated();
}

void Rd::Ui::Ui::noSubtitlesFound() {
    m_mode = Retry;
    Q_EMIT modeUpdated();
}

void Rd::Ui::Ui::subtitleFile(const QUrl& url, const QString& filename) {
    QFileInfo fi = QFileInfo(m_file.toLocalFile());
    m_saver->download(url, fi.absolutePath(), filename);
    m_mode = Working;
    Q_EMIT modeUpdated();
    m_targetFile = filename;
    Q_EMIT targetUpdated();
}

void Rd::Ui::Ui::handleUsageReport(quint32 remaining, const QString& reset) {
    m_remaining = remaining;
    m_reset = reset;
    Q_EMIT usageUpdated();
}

void Rd::Ui::Ui::done() {
    m_mode = Done;
    Q_EMIT modeUpdated();
}

void Rd::Ui::Ui::handleError(const QString& head, const QString& body) {
    Q_EMIT error(head, body);
}
