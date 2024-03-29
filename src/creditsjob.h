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

#ifndef _TMDBQT_CREDITSJOB_H_
#define _TMDBQT_CREDITSJOB_H_

#include "tmdbqt_export.h"
#include "personlist.h"
#include <QObject>

class QNetworkAccessManager;
class QUrl;

namespace TmdbQt
{
class TheMovieDbApi;
class CreditsJobPrivate;
class JobParams;
class PersonList;

class TMDBQT_EXPORT CreditsJob : public QObject
{
    Q_OBJECT
public:
    bool hasError() const;
    QString errorMessage() const;

    PersonList cast() const;
    PersonList crew() const;
    ~CreditsJob() override;

Q_SIGNALS:
    void result(TmdbQt::CreditsJob *job);

private Q_SLOTS:
    void requestFinished();

private:
    friend class TheMovieDbApi;
    CreditsJob(const JobParams &params, int movieId);
    CreditsJob(const JobParams &params, int tvShowId, int seasonNum, int episodeNum);
    void init(const QUrl &url);

    CreditsJobPrivate *d;
};

} // namespace

#endif
