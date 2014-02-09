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

#include "moviedblist.h"
#include "configuration.h"

#include <QDebug>
#include <QJsonObject>

using namespace TmdbQt;

class TmdbQt::MovieDbPrivate : public QSharedData
{
public:
    MovieDbPrivate(const Configuration &config)
        : m_configuration(config) {}

    const Configuration &m_configuration;
    QString m_backdropPath;
    int m_id;
    QString m_originalTitle;
    QDate m_releaseDate;
    QString m_posterPath;
    QString m_title;
};

MovieDb::MovieDb(const Configuration &configuration)
    : d(new MovieDbPrivate(configuration))
{
}

MovieDb::MovieDb(const MovieDb &other)
    : d(other.d)
{
}

MovieDb::~MovieDb()
{
}

MovieDb &MovieDb::operator=(const MovieDb &other)
{
    d = other.d;
    return *this;
}

QString MovieDb::backdropPath() const
{
    return d->m_backdropPath;
}

int MovieDb::id() const
{
    return d->m_id;
}

QString MovieDb::originalTitle() const
{
    return d->m_originalTitle;
}

QDate MovieDb::releaseDate() const
{
    return d->m_releaseDate;
}

QString MovieDb::posterPath() const
{
    return d->m_posterPath;
}

QString MovieDb::title() const
{
    return d->m_title;
}

QUrl MovieDb::backdropUrl(const QString &size) const
{
    QUrl url = d->m_configuration.imageBaseUrl();
    url.setPath(url.path() + size + d->m_backdropPath);
    return url;
}

QUrl MovieDb::posterUrl(const QString &size) const
{
    QUrl url = d->m_configuration.imageBaseUrl();
    url.setPath(url.path() + size + d->m_posterPath);
    return url;
}

void MovieDb::load(const QJsonObject &json)
{
    d->m_backdropPath = json.value(QStringLiteral("backdrop_path")).toString();
    d->m_id = json.value(QStringLiteral("id")).toInt();
    d->m_originalTitle = json.value(QStringLiteral("original_title")).toString();
    const QString releaseDate = json.value(QStringLiteral("release_date")).toString();
    d->m_releaseDate = QDate::fromString(releaseDate, Qt::ISODate);
    d->m_posterPath = json.value(QStringLiteral("poster_path")).toString();
    d->m_title = json.value(QStringLiteral("title")).toString();
}
