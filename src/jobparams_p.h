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

#ifndef _TMDBQT_JOBPARAMS_P_H_
#define _TMDBQT_JOBPARAMS_P_H_

#include "tmdbqt_export.h"
#include <QUrl>

class QNetworkAccessManager;
class QNetworkReply;

namespace TmdbQt
{
class Configuration;

/**
 * @brief The JobParams class puts together parameters passed to jobs.
 * @internal
 */
class JobParams
{
public:
    JobParams(QNetworkAccessManager &nam, Configuration &config)
        : qnam(nam), configuration(config) {}

    void setBaseUrl(const QUrl &url) { baseUrl = url; }

    /**
     * Issues a GET request for @p url through the shared QNetworkAccessManager.
     * Centralizes request setup so every job gets the same configuration
     * (notably HTTP/2 disabled, see jobparams.cpp).
     */
    QNetworkReply *get(const QUrl &url) const;

    QNetworkAccessManager &qnam;
    Configuration &configuration;
    QUrl baseUrl;
};

} // namespace

#endif
