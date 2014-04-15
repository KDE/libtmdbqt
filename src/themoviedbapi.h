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

#ifndef _TMDBQT_THEMOVIEDBAPI_H_
#define _TMDBQT_THEMOVIEDBAPI_H_

#include "tmdbqt_export.h"
#include <QObject>

namespace TmdbQt
{
class SearchJob;
class TvSearchJob;
class CreditsJob;
class MovieInfoJob;
class Configuration;
class TheMovieDbApiPrivate;

/**
 * @brief The TheMovieDbApi class provides the main API for accessing
 * TheMovieDatabase.org (also known as TMDB).
 */
class TMDBQT_EXPORT TheMovieDbApi : public QObject
{
    Q_OBJECT
public:
    TheMovieDbApi(const QString &apiKey);
    ~TheMovieDbApi();

    bool isInitialized() const;

    SearchJob *searchMovie(const QString &movieName,
                           int searchYear = 0,
                           const QString &language = QString());

    TvSearchJob *searchTvShow(const QString &tvShowName,
                              int firstAiredYear = 0,
                              const QString &language = QString());

    MovieInfoJob *getMovieInfo(int movieId);
    CreditsJob *getCredits(int movieId);

    Configuration &configuration() const;

Q_SIGNALS:
    /**
     * Emitted when the initialization is done. Wait for this signal before calling other methods!
     */
    void initialized();

private Q_SLOTS:
    void slotConfigurationReady();

private:
    TheMovieDbApiPrivate *d;
};

} // namespace

#endif
