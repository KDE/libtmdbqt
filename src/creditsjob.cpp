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

#include "creditsjob.h"
#include "jobparams_p.h"
#include "configuration.h"
#include "personlist.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace TmdbQt;

class TmdbQt::CreditsJobPrivate
{
public:
    CreditsJobPrivate(const JobParams &params)
        : m_params(params) {}
    QNetworkReply *m_reply;
    QString m_errorMessage;
    PersonList m_cast;
    PersonList m_crew;
    const JobParams &m_params;
};

CreditsJob::CreditsJob(const JobParams &params, int movieId)
    : d(new CreditsJobPrivate(params))
{
    QUrl url = params.baseUrl;
    url.setPath(url.path() + QStringLiteral("/movie/%1/credits").arg(movieId));
    init(url);
}

CreditsJob::CreditsJob(const JobParams &params, int tvShowId, int seasonNum, int episodeNum)
    : d(new CreditsJobPrivate(params))
{
    QUrl url = params.baseUrl;
    url.setPath(url.path() + QStringLiteral("/tv/%1/season/%2/episode/%3/credits")
                .arg(tvShowId).arg(seasonNum).arg(episodeNum));
    init(url);
}

void CreditsJob::init(const QUrl &url)
{
    QNetworkRequest request(url);
    d->m_reply = d->m_params.qnam.get(request);
    connect(d->m_reply, &QNetworkReply::finished, this, &CreditsJob::requestFinished);
}

CreditsJob::~CreditsJob()
{
    delete d->m_reply;
    delete d;
}

/**
 * @brief CreditsJob::hasError
 * @return true if the job had an error
 */
bool CreditsJob::hasError() const
{
    return !errorMessage().isEmpty();
}

/**
 * @brief CreditsJob::errorMessage
 * @return the error message for the job
 */
QString CreditsJob::errorMessage() const
{
    return d->m_errorMessage;
}

/**
 * @brief CreditsJob::cast
 * @return first job result: the cast, i.e. the list of actors/actresses
 */
PersonList CreditsJob::cast() const
{
    return d->m_cast;
}

/**
 * @brief CreditsJob::crew
 * @return second job result: the crew, i.e. everyone else who worked on the movie/show
 */
PersonList CreditsJob::crew() const
{
    return d->m_crew;
}

void CreditsJob::requestFinished()
{
    const QByteArray data = d->m_reply->readAll();
    //qDebug() << data;
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        d->m_errorMessage = QStringLiteral("Invalid json received\n") + QString::fromUtf8(data);
    }
    QJsonObject root = doc.object();

    QJsonArray castArray = root.value(QStringLiteral("cast")).toArray();
    d->m_cast.load(castArray, d->m_params.configuration, Person::Cast);

    QJsonArray crewArray = root.value(QStringLiteral("crew")).toArray();
    d->m_crew.load(crewArray, d->m_params.configuration, Person::Crew);

    d->m_reply->deleteLater();
    d->m_reply = 0;

    emit result(this);
    deleteLater();
}
