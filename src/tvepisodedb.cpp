/*
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

#include "tvepisodedb.h"
#include "configuration.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

using namespace TmdbQt;

class TmdbQt::TvEpisodeDbPrivate : public QSharedData
{
public:
    TvEpisodeDbPrivate(const Configuration &config)
        : m_configuration(config)
        , m_episodeNumber(-1)
    {}

    const Configuration &m_configuration;

    int m_episodeNumber;
    QDate m_airDate;

    QString m_name;
    QString m_overview;
    QString m_stillPath;
};

TvEpisodeDb::TvEpisodeDb(const Configuration& config)
    : d(new TvEpisodeDbPrivate(config))
{
}

TvEpisodeDb::TvEpisodeDb(const TvEpisodeDb& other)
    : d(other.d)
{
}

TvEpisodeDb::~TvEpisodeDb()
{
}


TvEpisodeDb &TvEpisodeDb::operator=(const TvEpisodeDb& other)
{
    d = other.d;
    return *this;
}

QDate TvEpisodeDb::airDate() const
{
    return d->m_airDate;
}

QString TvEpisodeDb::name() const
{
    return d->m_name;
}

int TvEpisodeDb::episodeNumber() const
{
    return d->m_episodeNumber;
}

QString TvEpisodeDb::overview() const
{
    return d->m_overview;
}

QString TvEpisodeDb::stillPath() const
{
    return d->m_stillPath;
}

QUrl TvEpisodeDb::stillUrl(const QString& size) const
{
    QUrl url = d->m_configuration.imageBaseUrl();
    if (url.isEmpty())
        qWarning() << "No image base URL, did you wait for the initialized() signal before starting jobs?";
    url.setPath(url.path() + size + d->m_stillPath);
    return url;
}

void TvEpisodeDb::load(const QJsonObject& json)
{
    d->m_episodeNumber = json.value(QStringLiteral("episode_number")).toInt();
    const QString date = json.value(QStringLiteral("air_date")).toString();
    d->m_airDate = QDate::fromString(date, Qt::ISODate);

    d->m_stillPath = json.value(QStringLiteral("still_path")).toString();
    d->m_name = json.value(QStringLiteral("name")).toString();
    d->m_overview = json.value(QStringLiteral("overview")).toString();
}
