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

#ifndef TMDBQT_TVSEASON_H
#define TMDBQT_TVSEASON_H

#include "tmdbqt_export.h"
#include <QString>
#include <QDate>
#include <QSharedDataPointer>
#include <QJsonObject>

namespace TmdbQt
{
class TvSeasonDbPrivate;
class TvSeasonDbList;
class Configuration;

class TMDBQT_EXPORT TvSeasonDb
{
public:
    TvSeasonDb(const Configuration &config);
    TvSeasonDb(const TvSeasonDb &other);
    ~TvSeasonDb();

    TvSeasonDb &operator=(const TvSeasonDb &other);

    int id() const;
    int seasonNumber() const;
    QDate airDate() const;

    QString posterPath() const;
    QUrl posterUrl(const QString &size) const;

    // More details required
    QString name() const;
    QString overview();
private:
    friend class TvSeasonDbList;
    void load(const QJsonObject &json);

    QSharedDataPointer<TvSeasonDbPrivate> d;
};

}

#endif // TMDBQT_TVSEASON_H
