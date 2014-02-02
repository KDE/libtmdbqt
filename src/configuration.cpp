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

#include "configuration_p.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

using namespace TmdbQt;

static QStringList readStringList(const QJsonArray &array)
{
    QStringList list;
    std::transform(array.begin(), array.end(), std::back_inserter(list), [](const QJsonValue &v) { return v.toString(); } );
    return list;
}

void Configuration::load(const QJsonObject &json)
{
    QJsonObject images = json.value(QStringLiteral("images")).toObject();
    m_imageBaseUrl = QUrl(images.value(QStringLiteral("base_url")).toString());
    m_posterSizes = readStringList(images.value(QStringLiteral("poster_sizes")).toArray());
    m_backdropSizes = readStringList(images.value(QStringLiteral("backdrop_sizes")).toArray());
    m_profileSizes = readStringList(images.value(QStringLiteral("profile_sizes")).toArray());
    m_logoSizes = readStringList(images.value(QStringLiteral("logo_sizes")).toArray());
}
