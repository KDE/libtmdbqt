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

#undef QT_NO_CAST_FROM_ASCII
#include <QTest>
#include <QSignalSpy>
#include <themoviedbapi.h>
#include <searchjob.h>
#include <tvsearchjob.h>
#include <creditsjob.h>
#include <movieinfojob.h>
#include <QDebug>

using namespace TmdbQt;

class SearchTest : public QObject
{
    Q_OBJECT

public:
    SearchTest();

private slots:
    void testSearch();
    void testTvSearch();
    void testMovieInfo();
    void testMovieInfoFightClub();
    void testCredits();

private:
    TheMovieDbApi m_api;
};

static const char s_key[] = "6c125ca74f059b4c88bc49e1b09e241e"; // themoviedb.org api key given to David Faure for KVideoManager

SearchTest::SearchTest()
    : m_api(QString::fromLatin1(s_key))
{
    QVERIFY(!m_api.isInitialized());
    QSignalSpy initSpy(&m_api, SIGNAL(initialized()));
    QVERIFY(initSpy.wait());
    QVERIFY(m_api.isInitialized());
}

void SearchTest::testSearch()
{
    const QString title = QString::fromUtf8("De l'autre côté du lit");
    SearchJob *job = m_api.searchMovie(title);
    QSignalSpy spy(job, SIGNAL(result(TmdbQt::SearchJob*)));
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
    QCOMPARE(movie.overview(), QString()); // not included here, needs MovieInfoJob

    const QString backdrop = movie.backdropUrl(QLatin1String("w92")).toString();
    QVERIFY2(backdrop.startsWith(QLatin1String("http://image.tmdb.org/t/p/w92/")), qPrintable(backdrop));
    const QString poster = movie.posterUrl(QLatin1String("w92")).toString();
    QVERIFY2(poster.startsWith(QLatin1String("http://image.tmdb.org/t/p/w92/")), qPrintable(poster));
}

void SearchTest::testTvSearch()
{
    const QString name = QString::fromLatin1("Breaking Bad");
    TvSearchJob *job = m_api.searchTvShow(name);
    QSignalSpy spy(job, SIGNAL(result(TmdbQt::TvSearchJob*)));
    QVERIFY(spy.wait());
    QVERIFY2(!job->hasError(), qPrintable(job->errorMessage()));
    TvShowDbList tvshows = job->result();
    QCOMPARE(tvshows.count(), 1);
    TvShowDb tvshow = tvshows.first();
    QCOMPARE(tvshow.id(), 1396);
    QCOMPARE(tvshow.firstAiredDate(), QDate(2008, 1, 19));
    QCOMPARE(tvshow.name(), name);
    QCOMPARE(tvshow.originalName(), name);
    QVERIFY(tvshow.backdropPath().contains(QLatin1String(".jpg")));
    QVERIFY(tvshow.posterPath().contains(QLatin1String(".jpg")));

    const QString backdrop = tvshow.backdropUrl(QLatin1String("w92")).toString();
    QVERIFY2(backdrop.startsWith(QLatin1String("http://image.tmdb.org/t/p/w92/")), qPrintable(backdrop));
    const QString poster = tvshow.posterUrl(QLatin1String("w92")).toString();
    QVERIFY2(poster.startsWith(QLatin1String("http://image.tmdb.org/t/p/w92/")), qPrintable(poster));
}

void SearchTest::testMovieInfo()
{
    MovieInfoJob *job = m_api.getMovieInfo(15709);
    QSignalSpy spy(job, SIGNAL(result(TmdbQt::MovieInfoJob*)));
    QVERIFY(spy.wait());
    QVERIFY2(!job->hasError(), qPrintable(job->errorMessage()));

    MovieDb movie = job->result();
    QCOMPARE(movie.overview(), QStringLiteral("Ariane and Hugo decide to exchange the lives they lead to escape from their routine, which after ten years of marriage, gives them the feeling of being hamsters in a wheel. She suddenly finds herself at the head of a construction equipment rental company and he tries to take the role of a house calling jewelry salesman...But is life really better when you live it on the other side of the bed?"));
    QCOMPARE(movie.productionCompanyNames(), QStringList() << "Fidélité Films" << "TF1 Films Production" << "Orange Cinéma Séries" << "Wild Bunch" << "Mars Distribution" << "Procirep");
    QCOMPARE(movie.budget(), 0); // not filled in
    QCOMPARE(movie.revenue(), 0); // not filled in
    QCOMPARE(movie.runtime(), 93);
}

void SearchTest::testMovieInfoFightClub()
{
    MovieInfoJob *job = m_api.getMovieInfo(550);
    QSignalSpy spy(job, SIGNAL(result(TmdbQt::MovieInfoJob*)));
    QVERIFY(spy.wait());
    QVERIFY2(!job->hasError(), qPrintable(job->errorMessage()));

    MovieDb movie = job->result();
    QCOMPARE(movie.productionCompanyNames(), QStringList() << "20th Century Fox" << "Fox 2000 Pictures" << "Regency Enterprises");
    QCOMPARE(movie.budget(), 63000000);
    QCOMPARE(movie.revenue(), 100853753);
    QCOMPARE(movie.runtime(), 139);
}

void SearchTest::testCredits()
{
    CreditsJob *job = m_api.getCredits(15709);
    QSignalSpy spy(job, SIGNAL(result(TmdbQt::CreditsJob*)));
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
