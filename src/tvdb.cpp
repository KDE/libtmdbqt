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

#include "tvdb.h"
#include "configuration.h"

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
};


TvDb::TvDb(const Configuration& config)
    : d(new TvDbPrivate(config))
{
}

TvDb::TvDb(const TvDb &other)
    : d(other.d)
{
}

TvDb::~TvDb()
{
}

TvDb &TvDb::operator=(const TvDb &other)
{
    d = other.d;
    return *this;
}

int TvDb::id() const
{
    return d->m_id;
}

QString TvDb::name() const
{
    return d->m_name;
}

QString TvDb::originalName() const
{
    return d->m_originalName;
}

QDate TvDb::firstAiredDate() const
{
    return d->m_firstAired;
}

QString TvDb::backdropPath() const
{
    return d->m_backdropPath;
}

QString TvDb::posterPath() const
{
    return d->m_posterPath;
}

QUrl TvDb::backdropUrl(const QString &size) const
{
    QUrl url = d->m_configuration.imageBaseUrl();
    if (url.isEmpty())
        qWarning() << "No image base URL, did you wait for the initialized() signal before starting jobs?";
    url.setPath(url.path() + size + d->m_backdropPath);
    return url;
}

QUrl TvDb::posterUrl(const QString &size) const
{
    QUrl url = d->m_configuration.imageBaseUrl();
    if (url.isEmpty())
        qWarning() << "No image base URL, did you wait for the initialized() signal before starting jobs?";
    url.setPath(url.path() + size + d->m_posterPath);
    return url;

}

void TvDb::load(const QJsonObject& json)
{
    d->m_backdropPath = json.value(QStringLiteral("backdrop_path")).toString();
    d->m_id = json.value(QStringLiteral("id")).toInt();
    d->m_originalName = json.value(QStringLiteral("original_name")).toString();
    d->m_name = json.value(QStringLiteral("name")).toString();

    const QString firstAiredDate = json.value(QStringLiteral("first_air_date")).toString();
    d->m_firstAired = QDate::fromString(firstAiredDate, Qt::ISODate);
    d->m_posterPath = json.value(QStringLiteral("poster_path")).toString();
}

