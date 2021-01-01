/*
 * <one line to give the library's name and an idea of what it does.>
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

#ifndef TMDBQT_TVSEASONINFOJOB_H
#define TMDBQT_TVSEASONINFOJOB_H

#include <QObject>
#include "tvseasondb.h"

class QNetworkAccessManager;
class QUrl;

namespace TmdbQt
{

class TheMovieDbApi;
class TvSeasonInfoJobPrivate;
class JobParams;

class TvSeasonInfoJob : public QObject
{
    Q_OBJECT
public:
    bool hasError() const;
    QString errorMessage() const;
    TvSeasonDb searchResult() const;
    ~TvSeasonInfoJob();

Q_SIGNALS:
    void result(TmdbQt::TvSeasonInfoJob *job);

private Q_SLOTS:
    void requestFinished();

private:
    friend class TheMovieDbApi;
    TvSeasonInfoJob(const JobParams &params, int tvid, int seasonNum);

    TvSeasonInfoJobPrivate *d;
};

}

#endif // TMDBQT_TVSEASONINFOJOB_H
