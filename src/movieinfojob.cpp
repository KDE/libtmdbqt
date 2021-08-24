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

#include "movieinfojob.h"
#include "jobparams_p.h"
#include "configuration.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace TmdbQt;

class TmdbQt::MovieInfoJobPrivate
{
public:
    MovieInfoJobPrivate(const JobParams &params)
        : m_result(params.configuration), m_params(params) {}
    QNetworkReply *m_reply;
    QString m_errorMessage;
    MovieDb m_result;
    const JobParams &m_params;
};

MovieInfoJob::MovieInfoJob(const JobParams &params, int movieId)
    : d(new MovieInfoJobPrivate(params))
{
    QUrl url = params.baseUrl;
    url.setPath(url.path() + QStringLiteral("/movie/%1").arg(movieId));

    QNetworkRequest request(url);
    d->m_reply = params.qnam.get(request);
    connect(d->m_reply, &QNetworkReply::finished, this, &MovieInfoJob::requestFinished);
}

MovieInfoJob::~MovieInfoJob()
{
    delete d->m_reply;
    delete d;
}

/**
 * @brief MovieInfoJob::hasError
 * @return true if the job had an error
 */
bool MovieInfoJob::hasError() const
{
    return !errorMessage().isEmpty();
}

/**
 * @brief MovieInfoJob::errorMessage
 * @return the error message for the job
 */
QString MovieInfoJob::errorMessage() const
{
    return d->m_errorMessage;
}

/**
 * @brief MovieInfoJob::searchResult
 * @return the job result: detailed information about a movie
 */
MovieDb MovieInfoJob::searchResult() const
{
    return d->m_result;
}

void MovieInfoJob::requestFinished()
{
    const QByteArray data = d->m_reply->readAll();
    //qDebug() << data;
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        d->m_errorMessage = QStringLiteral("Invalid json received");
    }
    QJsonObject root = doc.object();
    d->m_result.load(root);

    d->m_reply->deleteLater();
    d->m_reply = nullptr;

    emit result(this);
    deleteLater();
}
