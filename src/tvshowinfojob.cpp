/*
 * Copyright (C) 2014 Vishesh Handa <me@vhanda.in>
 * Copyright (C) 2014 David Faure <faure@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "tvshowinfojob.h"
#include "jobparams_p.h"
#include "configuration.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace TmdbQt;

class TmdbQt::TvShowInfoJobPrivate
{
public:
    TvShowInfoJobPrivate(int id, const JobParams &params)
        : m_tvShowId(id), m_result(params.configuration), m_params(params) {}
    int m_tvShowId;
    QNetworkReply *m_reply;
    QString m_errorMessage;
    TvShowDb m_result;
    const JobParams &m_params;
};

TvShowInfoJob::TvShowInfoJob(const JobParams &params, int tvid)
    : d(tvid, new TvShowInfoJobPrivate(params))
{
    QUrl url = params.baseUrl;
    url.setPath(url.path() + QStringLiteral("/tv/%1").arg(tvid));

    QNetworkRequest request(url);
    d->m_reply = params.qnam.get(request);
    connect(d->m_reply, SIGNAL(finished()), this, SLOT(requestFinished()));
}

TvShowInfoJob::~TvShowInfoJob()
{
    delete d->m_reply;
    delete d;
}

/**
 * @brief TvShowInfoJob::tvShowId
 * @return the ID of the TV show, as passed to TheMovieDbApi::getTvShowInfo
 */
int TvShowInfoJob::tvShowId() const
{
    return d->m_tvShowId;
}

/**
 * @brief TvShowInfoJob::hasError
 * @return true if the job had an error
 */
bool TvShowInfoJob::hasError() const
{
    return !errorMessage().isEmpty();
}

/**
 * @brief TvShowInfoJob::errorMessage
 * @return the error message for the job
 */
QString TvShowInfoJob::errorMessage() const
{
    return d->m_errorMessage;
}

/**
 * @brief TvShowInfoJob::result
 * @return the job result: information about a TV show, including a list of seasons, but
 * without details for the reasons.
 */
TvShowDb TvShowInfoJob::result() const
{
    return d->m_result;
}

void TvShowInfoJob::requestFinished()
{
    const QByteArray data = d->m_reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        d->m_errorMessage = QStringLiteral("Invalid json received");
    }
    QJsonObject root = doc.object();
    d->m_result.load(root);

    d->m_reply->deleteLater();
    d->m_reply = 0;

    emit result(this);
    deleteLater();
}
