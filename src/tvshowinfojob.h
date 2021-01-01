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

#ifndef TMDBQT_TVSHOWINFOJOB_H
#define TMDBQT_TVSHOWINFOJOB_H

#include "tmdbqt_export.h"
#include "tvshowdb.h"
#include <QObject>

class QNetworkAccessManager;
class QUrl;

namespace TmdbQt
{
class TheMovieDbApi;
class TvShowInfoJobPrivate;
class JobParams;

class TvShowInfoJob : public QObject
{
    Q_OBJECT
public:
    int tvShowId() const;

    bool hasError() const;
    QString errorMessage() const;
    TvShowDb searchResult() const;
    ~TvShowInfoJob();

Q_SIGNALS:
    void result(TmdbQt::TvShowInfoJob *job);

private Q_SLOTS:
    void requestFinished();

private:
    friend class TheMovieDbApi;
    TvShowInfoJob(const JobParams &params, int tvid);

    TvShowInfoJobPrivate *d;
};
}

#endif // TMDBQT_TVSHOWINFOJOB_H
