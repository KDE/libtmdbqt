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
#include <QJsonArray>
#include <QJsonObject>

using namespace TmdbQt;

class TmdbQt::MovieDbPrivate : public QSharedData
{
public:
    MovieDbPrivate(const Configuration &config)
        : m_configuration(config),
          m_id(-1),
          m_budget(0),
          m_revenue(0),
          m_runtime(0)
    {}

    const Configuration &m_configuration;
    QString m_backdropPath;
    int m_id;
    QString m_originalTitle;
    QDate m_releaseDate;
    QString m_posterPath;
    QString m_title;
    int m_budget;
    QString m_overview;
    QStringList m_productionCompanyNames;
    int m_revenue;
    int m_runtime;
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

int MovieDb::budget() const
{
    return d->m_budget;
}

QString MovieDb::overview() const
{
    return d->m_overview;
}

QStringList MovieDb::productionCompanyNames() const
{
    return d->m_productionCompanyNames;
}

int MovieDb::revenue() const
{
    return d->m_revenue;
}

int MovieDb::runtime() const
{
    return d->m_runtime;
}

QUrl MovieDb::backdropUrl(const QString &size) const
{
    QUrl url = d->m_configuration.imageBaseUrl();
    if (url.isEmpty())
        qWarning() << "No image base URL, did you wait for the initialized() signal before starting jobs?";
    url.setPath(url.path() + size + d->m_backdropPath);
    return url;
}

QUrl MovieDb::posterUrl(const QString &size) const
{
    QUrl url = d->m_configuration.imageBaseUrl();
    if (url.isEmpty())
        qWarning() << "No image base URL, did you wait for the initialized() signal before starting jobs?";
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

    d->m_budget = json.value(QStringLiteral("budget")).toInt();
    d->m_overview = json.value(QStringLiteral("overview")).toString();
    const QJsonArray productionCompanies = json.value(QStringLiteral("production_companies")).toArray();
    for (int i = 0; i < productionCompanies.count() ; ++i) {
        const QJsonObject obj = productionCompanies.at(i).toObject();
        d->m_productionCompanyNames << obj.value(QStringLiteral("name")).toString();
    }
    d->m_revenue = json.value(QStringLiteral("revenue")).toInt();
    d->m_runtime = json.value(QStringLiteral("runtime")).toInt();

}

QDebug operator<<(QDebug dbg, const MovieDb &movie)
{
    dbg << '[' << movie.id() << movie.title() << ']';
    return dbg;
}
