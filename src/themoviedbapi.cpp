/*
 * Copyright (C) 2014 David Faure <faure@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "themoviedbapi.h"
#include "searchjob.h"
#include "tvsearchjob.h"
#include "creditsjob.h"
#include "movieinfojob.h"
#include "tvshowinfojob.h"
#include "tvseasoninfojob.h"
#include "configuration.h"
#include "jobparams_p.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
using namespace TmdbQt;

class TmdbQt::TheMovieDbApiPrivate
{
public:
    TheMovieDbApiPrivate(const QString &apiKey)
        : m_apiKey(apiKey)
        , m_jobParams(m_qnam, m_configuration)
    {
    }

    QUrl baseUrl() const;
    void slotConfigurationReady(TheMovieDbApi *q);

    QString m_apiKey;
    QNetworkAccessManager m_qnam;
    QNetworkReply *m_configurationReply;
    Configuration m_configuration;
    JobParams m_jobParams;
};

TheMovieDbApi::TheMovieDbApi(const QString &apiKey)
    : d(new TheMovieDbApiPrivate(apiKey))
{
    QUrl url = d->baseUrl();
    d->m_jobParams.setBaseUrl(url);

    url.setPath(url.path() + QLatin1String("/configuration"));
    QNetworkRequest request(url);
    d->m_configurationReply = d->m_qnam.get(request);
    connect(d->m_configurationReply, &QNetworkReply::finished,
            this, [this]() { d->slotConfigurationReady(this); });
}

TheMovieDbApi::~TheMovieDbApi()
{
    delete d;
}

bool TheMovieDbApi::isInitialized() const
{
    return d->m_configurationReply == nullptr;
}

SearchJob *TheMovieDbApi::searchMovie(const QString &movieName, int searchYear, const QString &language)
{
    return new SearchJob(d->m_jobParams, movieName, searchYear, language);
}

TvSearchJob* TheMovieDbApi::searchTvShow(const QString& tvShowName, int firstAiredYear, const QString& language)
{
    return new TvSearchJob(d->m_jobParams, tvShowName, firstAiredYear, language);
}

MovieInfoJob *TheMovieDbApi::getMovieInfo(int movieId)
{
    return new MovieInfoJob(d->m_jobParams, movieId);
}

TvShowInfoJob* TheMovieDbApi::getTvShowInfo(int tvshowId)
{
    return new TvShowInfoJob(d->m_jobParams, tvshowId);
}

TvSeasonInfoJob* TheMovieDbApi::getTvSeasonInfo(int tvshowId, int seasonNum)
{
    return new TvSeasonInfoJob(d->m_jobParams, tvshowId, seasonNum);
}

CreditsJob *TheMovieDbApi::getCredits(int movieId)
{
    return new CreditsJob(d->m_jobParams, movieId);
}

CreditsJob *TheMovieDbApi::getEpisodeCredits(int tvShowId, int seasonNum, int episodeNum)
{
    return new CreditsJob(d->m_jobParams, tvShowId, seasonNum, episodeNum);
}

Configuration &TheMovieDbApi::configuration() const
{
    return d->m_configuration;
}

void TheMovieDbApiPrivate::slotConfigurationReady(TheMovieDbApi *q)
{
    if (m_configurationReply->error()) {
        qDebug() << "ERROR" << m_configurationReply->errorString();
    } else {
        const QByteArray data = m_configurationReply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject root = doc.object();
        m_configuration.load(root);
        Q_EMIT q->initialized();
    }
    m_configurationReply->deleteLater();
    m_configurationReply = nullptr;
}

QUrl TheMovieDbApiPrivate::baseUrl() const
{
    static const char s_tmdbApiUrl[] = "http://api.themoviedb.org/3";
    QUrl url(QString::fromLatin1(s_tmdbApiUrl));
    QUrlQuery query(url);
    query.addQueryItem(QLatin1String("api_key"), m_apiKey);
    url.setQuery(query);
    return url;
}
