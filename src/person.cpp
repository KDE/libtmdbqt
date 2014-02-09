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

#include "personlist.h"
#include "configuration.h"

#include <QDebug>
#include <QJsonObject>

using namespace TmdbQt;

class TmdbQt::PersonPrivate : public QSharedData
{
public:
    PersonPrivate(const Configuration &config)
        : m_configuration(config) {}

    const Configuration &m_configuration;
    Person::PersonType m_personType;

    QString m_name;
    QString m_character;
    QString m_profilePath;
    QString m_department;
    QString m_job;
};

Person::Person(const Configuration &configuration)
    : d(new PersonPrivate(configuration))
{
}

Person::Person(const Person &other)
    : d(other.d)
{
}

Person::~Person()
{
}

Person &Person::operator=(const Person &other)
{
    d = other.d;
    return *this;
}

QString Person::name() const
{
    return d->m_name;
}

QString Person::character() const
{
    return d->m_character;
}

QString Person::profilePath() const
{
    return d->m_profilePath;
}

QString Person::department() const
{
    return d->m_department;
}

QString Person::job() const
{
    return d->m_job;
}

void Person::load(const QJsonObject &json, PersonType personType)
{
    d->m_personType = personType;
    d->m_name = json.value(QStringLiteral("name")).toString();
    d->m_character = json.value(QStringLiteral("character")).toString();
    d->m_department = json.value(QStringLiteral("department")).toString();
    d->m_job = json.value(QStringLiteral("job")).toString();
    d->m_profilePath = json.value(QStringLiteral("profile_path")).toString();
}
