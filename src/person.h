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

#ifndef _TMDBQT_PERSON_H
#define _TMDBQT_PERSON_H

#include "tmdbqt_export.h"
#include <QString>
#include <QSharedDataPointer>
class QJsonObject;

namespace TmdbQt
{
class PersonList;
class Configuration;
class PersonPrivate;

class TMDBQT_EXPORT Person
{
public:
    Person(const Configuration &configuration);
    Person(const Person &other);
    ~Person();
    Person &operator=(const Person &other);

    enum PersonType { Cast, Crew };

    PersonType personType() const;

    QString name() const;
    QString character() const; // for Cast members only
    QString profilePath() const;

    QString department() const; // for Crew members only
    QString job() const; // for Crew members only


private:
    friend class PersonList;
    void load(const QJsonObject &json, PersonType personType);

    QSharedDataPointer<PersonPrivate> d;
};

} // namespace

#endif
