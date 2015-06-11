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

#ifndef TMDBQT_TVEPISODE_H
#define TMDBQT_TVEPISODE_H

#include "tmdbqt_export.h"
#include <QString>
#include <QDate>
#include <QSharedDataPointer>
#include <QStringList>
#include <QUrl>

class QJsonObject;

namespace TmdbQt
{
class TvEpisodeDbList;
class Configuration;
class TvEpisodeDbPrivate;
class TvEpisodeInfoJob;

class TMDBQT_EXPORT TvEpisodeDb
{
public:
    TvEpisodeDb(const Configuration &config);
    TvEpisodeDb(const TvEpisodeDb &other);
    ~TvEpisodeDb();

    TvEpisodeDb &operator=(const TvEpisodeDb &other);

    int episodeNumber() const;
    QDate airDate() const;

    QString name() const;
    QString overview() const;
    QString stillPath() const;
    QUrl stillUrl(const QString &size) const;

private:
    friend class TvEpisodeDbList;
    void load(const QJsonObject &json);

    QSharedDataPointer<TvEpisodeDbPrivate> d;
};
}

#endif // TMDBQT_TVEPISODE_H
