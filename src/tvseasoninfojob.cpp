/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2014  Vishesh Handa <me@vhanda.in>
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

#include "tvseasoninfojob.h"
#include "jobparams_p.h"
#include "configuration.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace TmdbQt;

class TmdbQt::TvSeasonInfoJobPrivate
{
public:
    TvSeasonInfoJobPrivate(const JobParams &params)
        : m_result(params.configuration), m_params(params) {}
    QNetworkReply *m_reply;
    QString m_errorMessage;
    TvSeasonDb m_result;
    const JobParams &m_params;
};

TvSeasonInfoJob::TvSeasonInfoJob(const JobParams &params, int tvid, int seasonNum)
    : d(new TvSeasonInfoJobPrivate(params))
{
    QUrl url = params.baseUrl;
    url.setPath(url.path() + QStringLiteral("/tv/%1/season/%2").arg(tvid).arg(seasonNum));

    QNetworkRequest request(url);
    d->m_reply = params.qnam.get(request);
    connect(d->m_reply, &QNetworkReply::finished, this, &TvSeasonInfoJob::requestFinished);
}

TvSeasonInfoJob::~TvSeasonInfoJob()
{
    delete d->m_reply;
    delete d;
}

/**
 * @brief TvSeasonInfoJob::hasError
 * @return true if the job had an error
 */
bool TvSeasonInfoJob::hasError() const
{
    return !errorMessage().isEmpty();
}

/**
 * @brief TvSeasonInfoJob::errorMessage
 * @return the error message for the job
 */
QString TvSeasonInfoJob::errorMessage() const
{
    return d->m_errorMessage;
}

/**
 * @brief TvSeasonInfoJob::searchResult
 * @return the job result: detailed information about a season
 */
TvSeasonDb TvSeasonInfoJob::searchResult() const
{
    return d->m_result;
}

void TvSeasonInfoJob::requestFinished()
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
