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

#ifndef _TMDBQT_MOVIEDB_H
#define _TMDBQT_MOVIEDB_H

#include "tmdbqt_export.h"
#include <QString>
#include <QDate>
class QJsonObject;

namespace TmdbQt
{
class MovieDbList;
class TMDBQT_EXPORT MovieDb
{
public:
    QString backdropPath() const { return m_backdropPath; }
    int id() const { return m_id; }
    QString originalTitle() const { return m_originalTitle; }
    QDate releaseDate() const { return m_releaseDate; }
    QString posterPath() const { return m_posterPath; }
    QString title() const { return m_title; }

private:
    friend class MovieDbList;
    void load(const QJsonObject &json);

    QString m_backdropPath;
    int m_id;
    QString m_originalTitle;
    QDate m_releaseDate;
    QString m_posterPath;
    QString m_title;
};

} // namespace

#endif
