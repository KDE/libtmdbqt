/*
    Copyright (c) 2014 David Faure <faure@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <QTest>
#include <QSignalSpy>
#include <themoviedbapi.h>
#include <searchjob.h>
#include <creditsjob.h>
#include <QDebug>

using namespace TmdbQt;

class SearchTest : public QObject
{
    Q_OBJECT

public:
    SearchTest();

private slots:
    void testSearch();
    void testCredits();

private:
    TheMovieDbApi m_api;
};

static const char s_key[] = "6c125ca74f059b4c88bc49e1b09e241e"; // themoviedb.org api key given to David Faure for KVideoManager

SearchTest::SearchTest()
    : m_api(QString::fromLatin1(s_key))
{
    QSignalSpy initSpy(&m_api, SIGNAL(initialized()));
    QVERIFY(initSpy.wait());
}

void SearchTest::testSearch()
{
    const QString title = QString::fromUtf8("De l'autre côté du lit");
    SearchJob *job = m_api.searchMovie(title);
    QSignalSpy spy(job, SIGNAL(result(SearchJob*)));
    QVERIFY(spy.wait());
    QVERIFY2(!job->hasError(), qPrintable(job->errorMessage()));
    MovieDbList movies = job->result();
    QCOMPARE(movies.count(), 1);
    MovieDb movie = movies.first();
    QCOMPARE(movie.id(), 15709);
    QCOMPARE(movie.releaseDate(), QDate(2008, 12, 9));
    QCOMPARE(movie.title(), QString::fromLatin1("Changing Sides"));
    QCOMPARE(movie.originalTitle(), title);
    QVERIFY(movie.backdropPath().contains(QLatin1String(".jpg")));
    QVERIFY(movie.posterPath().contains(QLatin1String(".jpg")));

    const QString backdrop = movie.backdropUrl(QLatin1String("w92")).toString();
    QVERIFY2(backdrop.startsWith(QLatin1String("http://image.tmdb.org/t/p/w92/")), qPrintable(backdrop));
    const QString poster = movie.posterUrl(QLatin1String("w92")).toString();
    QVERIFY2(poster.startsWith(QLatin1String("http://image.tmdb.org/t/p/w92/")), qPrintable(poster));
}

void SearchTest::testCredits()
{
    CreditsJob *job = m_api.getCredits(15709);
    QSignalSpy spy(job, SIGNAL(result(CreditsJob*)));
    QVERIFY(spy.wait());
    QVERIFY2(!job->hasError(), qPrintable(job->errorMessage()));
    PersonList cast = job->cast();
    QCOMPARE(cast.count(), 14);
    const Person firstPerson = cast.at(0);
    QCOMPARE(firstPerson.name(), QStringLiteral("Dany Boon"));
    QCOMPARE(firstPerson.character(), QStringLiteral("Hugo Marciac"));
    QVERIFY2(firstPerson.profilePath().contains(QLatin1String(".jpg")), qPrintable(firstPerson.profilePath()));
    // TODO profileUrl

    PersonList crew = job->crew();
    QCOMPARE(crew.count(), 11);
    const Person firstCrew = crew.at(0);
    QCOMPARE(firstCrew.name(), QStringLiteral("Pascale Pouzadoux"));
    QCOMPARE(firstCrew.department(), QStringLiteral("Directing"));
    QCOMPARE(firstCrew.job(), QStringLiteral("Director"));
}

QTEST_MAIN(SearchTest)

#include "searchtest.moc"
