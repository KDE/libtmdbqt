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

#ifndef TMDBQT_TVDB_H
#define TMDBQT_TVDB_H

#include "tmdbqt_export.h"
#include <QString>
#include <QDate>
#include <QSharedDataPointer>

namespace TmdbQt
{
class TvShowDbList;
class TvSeasonDbList;
class TvShowInfoJob;
class TvDbPrivate;
class Configuration;

class TMDBQT_EXPORT TvShowDb
{
public:
    TvShowDb(const Configuration &config);
    TvShowDb(const TvShowDb &other);
    ~TvShowDb();
    TvShowDb &operator =(const TvShowDb &other);

    int id() const;
    QString name() const;
    QString originalName() const;

    QDate firstAiredDate() const;

    QString posterPath() const;
    QString backdropPath() const;

    QUrl posterUrl(const QString &size) const;
    QUrl backdropUrl(const QString &size) const;

    // More details (not available in TvSearchJob's result; requires TvShowInfoJob)
    QString overview() const;
    TvSeasonDbList seasons() const;

private:
    friend class TvShowDbList;
    friend class TvShowInfoJob;
    void load(const QJsonObject &json);

    QSharedDataPointer<TvDbPrivate> d;
};
}

#endif // TMDBQT_TVDB_H
