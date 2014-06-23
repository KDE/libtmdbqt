/*
 * Copyright (C) 2014 Vishesh Handa <me@vhanda.in>
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

#include "tvseasondb.h"
#include "tvepisodedblist.h"
#include "configuration.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

using namespace TmdbQt;

class TmdbQt::TvSeasonDbPrivate : public QSharedData
{
public:
    TvSeasonDbPrivate(const Configuration &config)
        : m_configuration(config)
        , m_id(-1)
        , m_seasonNumber(-1)
    {}

    Configuration m_configuration;

    int m_id;
    int m_seasonNumber;

    QString m_name;
    QString m_overview;

    QDate m_airDate;
    QString m_posterPath;

    TvEpisodeDbList m_episodes;
};

TvSeasonDb::TvSeasonDb(const Configuration& config)
    : d(new TvSeasonDbPrivate(config))
{
}

TvSeasonDb::TvSeasonDb(const TvSeasonDb& other)
    : d(other.d)
{
}

TvSeasonDb::~TvSeasonDb()
{
}

TvSeasonDb &TvSeasonDb::operator=(const TvSeasonDb& other)
{
    d = other.d;
    return *this;
}

QDate TvSeasonDb::airDate() const
{
    return d->m_airDate;
}

int TvSeasonDb::id() const
{
    return d->m_id;
}

QString TvSeasonDb::name() const
{
    return d->m_name;
}

QString TvSeasonDb::overview() const
{
    return d->m_overview;
}

QString TvSeasonDb::posterPath() const
{
    return d->m_posterPath;
}

int TvSeasonDb::seasonNumber() const
{
    return d->m_seasonNumber;
}

QUrl TvSeasonDb::posterUrl(const QString& size) const
{
    QUrl url = d->m_configuration.imageBaseUrl();
    if (url.isEmpty())
        qWarning() << "No image base URL, did you wait for the initialized() signal before starting jobs?";
    url.setPath(url.path() + size + d->m_posterPath);
    return url;
}

TvEpisodeDbList TvSeasonDb::episodes() const
{
    return d->m_episodes;
}

void TvSeasonDb::load(const QJsonObject& json)
{
    d->m_id = json.value(QStringLiteral("id")).toInt();
    d->m_seasonNumber = json.value(QStringLiteral("season_number")).toInt();
    d->m_name = json.value(QStringLiteral("name")).toString();
    d->m_overview = json.value(QStringLiteral("overview")).toString();

    const QString airDate = json.value(QStringLiteral("air_date")).toString();
    d->m_airDate = QDate::fromString(airDate, Qt::ISODate);
    d->m_posterPath = json.value(QStringLiteral("poster_path")).toString();

    QJsonArray arr = json.value(QStringLiteral("episodes")).toArray();
    d->m_episodes.load(arr, d->m_configuration);
}


