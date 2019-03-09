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

#ifndef _TMDBQT_MOVIEDB_H
#define _TMDBQT_MOVIEDB_H

#include "tmdbqt_export.h"
#include <QString>
#include <QDate>
#include <QSharedDataPointer>
#include <QStringList>
#include <QUrl>
class QJsonObject;

namespace TmdbQt
{
class MovieDbList;
class Configuration;
class MovieDbPrivate;
class MovieInfoJob;

class TMDBQT_EXPORT MovieDb
{
public:
    MovieDb(const Configuration &configuration);
    MovieDb(const MovieDb &other);
    ~MovieDb();
    MovieDb &operator=(const MovieDb &other);

    QString backdropPath() const;
    int id() const;
    QString originalTitle() const;
    QDate releaseDate() const;
    QString posterPath() const;
    QString title() const;

    // More details (not available in SearchJob's result; requires MovieInfoJob)
    int budget() const;
    QString overview() const;
    QStringList productionCompanyNames() const;
    int revenue() const;
    int runtime() const; // in minutes

    QUrl backdropUrl(const QString &size) const;
    QUrl posterUrl(const QString &size) const;

private:
    friend class MovieDbList;
    friend class MovieInfoJob;
    void load(const QJsonObject &json);

    QSharedDataPointer<MovieDbPrivate> d;
};

} // namespace

QDebug operator<<(QDebug dbg, const TmdbQt::MovieDb &movie);

#endif
