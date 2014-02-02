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

#ifndef _TMDBQT_CONFIGURATION_P_H
#define _TMDBQT_CONFIGURATION_P_H

#include "tmdbqt_export.h"
#include <QStringList>
#include <QUrl>
class QJsonObject;

namespace TmdbQt
{
class Configuration
{
public:
    void load(const QJsonObject &json);

    QUrl imageBaseUrl() const { return m_imageBaseUrl; }
    QStringList posterSizes() const { return m_posterSizes; }
    QStringList backdropSizes() const { return m_backdropSizes; }
    QStringList profileSizes() const { return m_profileSizes; }
    QStringList logoSizes() const { return m_logoSizes; }

private:
    QUrl m_imageBaseUrl;
    QStringList m_posterSizes;
    QStringList m_backdropSizes;
    QStringList m_profileSizes;
    QStringList m_logoSizes;
};

} // namespace

#endif
