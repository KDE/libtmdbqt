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

#include "moviedblist.h"

#include <QDebug>
#include <QJsonObject>

using namespace TmdbQt;

void MovieDb::load(const QJsonObject &json)
{
    m_backdropPath = json.value(QStringLiteral("backdrop_path")).toString();
    m_id = json.value(QStringLiteral("id")).toInt();
    m_originalTitle = json.value(QStringLiteral("original_title")).toString();
    const QString releaseDate = json.value(QStringLiteral("release_date")).toString();
    m_releaseDate = QDate::fromString(releaseDate, Qt::ISODate);
    m_posterPath = json.value(QStringLiteral("poster_path")).toString();
    m_title = json.value(QStringLiteral("title")).toString();
}
