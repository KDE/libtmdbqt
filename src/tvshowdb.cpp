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

#include "tvshowdb.h"
#include "configuration.h"
#include "tvseasondblist.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

using namespace TmdbQt;

class TmdbQt::TvDbPrivate : public QSharedData
{
public:
    TvDbPrivate(const Configuration &config)
        : m_configuration(config)
        , m_id(-1)
    {}

    Configuration m_configuration;

    int m_id;
    QString m_name;
    QString m_originalName;

    QDate m_firstAired;
    QString m_posterPath;
    QString m_backdropPath;

    QString m_overview;
    TvSeasonDbList m_seasons;
};


TvShowDb::TvShowDb(const Configuration& config)
    : d(new TvDbPrivate(config))
{
}

TvShowDb::TvShowDb(const TvShowDb &other)
    : d(other.d)
{
}

TvShowDb::~TvShowDb()
{
}

TvShowDb &TvShowDb::operator=(const TvShowDb &other)
{
    d = other.d;
    return *this;
}

int TvShowDb::id() const
{
    return d->m_id;
}

QString TvShowDb::name() const
{
    return d->m_name;
}

QString TvShowDb::originalName() const
{
    return d->m_originalName;
}

QDate TvShowDb::firstAiredDate() const
{
    return d->m_firstAired;
}

QString TvShowDb::backdropPath() const
{
    return d->m_backdropPath;
}

QString TvShowDb::posterPath() const
{
    return d->m_posterPath;
}

QUrl TvShowDb::backdropUrl(const QString &size) const
{
    QUrl url = d->m_configuration.imageBaseUrl();
    if (url.isEmpty())
        qWarning() << "No image base URL, did you wait for the initialized() signal before starting jobs?";
    url.setPath(url.path() + size + d->m_backdropPath);
    return url;
}

QUrl TvShowDb::posterUrl(const QString &size) const
{
    QUrl url = d->m_configuration.imageBaseUrl();
    if (url.isEmpty())
        qWarning() << "No image base URL, did you wait for the initialized() signal before starting jobs?";
    url.setPath(url.path() + size + d->m_posterPath);
    return url;

}

QString TvShowDb::overview() const
{
    return d->m_overview;
}

TvSeasonDbList TvShowDb::seasons() const
{
    return d->m_seasons;
}

void TvShowDb::load(const QJsonObject& json)
{
    d->m_backdropPath = json.value(QStringLiteral("backdrop_path")).toString();
    d->m_id = json.value(QStringLiteral("id")).toInt();
    d->m_originalName = json.value(QStringLiteral("original_name")).toString();
    d->m_name = json.value(QStringLiteral("name")).toString();
    d->m_overview = json.value(QStringLiteral("overview")).toString();

    const QString firstAiredDate = json.value(QStringLiteral("first_air_date")).toString();
    d->m_firstAired = QDate::fromString(firstAiredDate, Qt::ISODate);
    d->m_posterPath = json.value(QStringLiteral("poster_path")).toString();

    QJsonArray arr = json.value(QStringLiteral("seasons")).toArray();
    d->m_seasons.load(arr, d->m_configuration);
}

