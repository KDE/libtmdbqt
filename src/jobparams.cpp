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

#include "jobparams_p.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

using namespace TmdbQt;

QNetworkReply *JobParams::get(const QUrl &url) const
{
    qDebug() << "GET" << url;
    QNetworkRequest request(url);
    // Qt 6 enables HTTP/2 by default. Concurrent HTTP/2 streams to
    // api.themoviedb.org (e.g. one request per TV season, fired in parallel)
    // are intermittently dropped on Windows, which used to surface as silently
    // truncated results (empty seasons, missing init). Force HTTP/1.1.
    request.setAttribute(QNetworkRequest::Http2AllowedAttribute, false);
    return qnam.get(request);
}
