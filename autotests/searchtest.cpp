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
#include <tvshowinfojob.h>
#include <creditsjob.h>
#include <movieinfojob.h>
#include <tvseasondblist.h>
#include <tvseasoninfojob.h>
#include <tvepisodedblist.h>
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
    void testTvShowInfo();
    void testTvSeasonInfo();
    void testCredits();
    void testEpisodeCredits();

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
    if (movies.count() > 1 && movies.at(1).id() == 15142) // "Dream a little dream" is "De l'autre côté du rêve", a fuzzy match...
        movies.removeAt(1);
    QCOMPARE(movies.count(), 1);
    MovieDb movie = movies.first();
    QCOMPARE(movie.id(), 15709);
    QCOMPARE(movie.releaseDate(), QDate(2008, 1, 7));
    QCOMPARE(movie.title(), QString::fromLatin1("Changing Sides"));
    QCOMPARE(movie.originalTitle(), title);
    QVERIFY(movie.backdropPath().contains(QLatin1String(".jpg")));
    QVERIFY(movie.posterPath().contains(QLatin1String(".jpg")));
    // new: the overview is actually available here
    QCOMPARE(movie.overview(), QStringLiteral("Ariane and Hugo decide to exchange the lives they lead to escape from their routine, which after ten years of marriage, gives them the feeling of being hamsters in a wheel. She suddenly finds herself at the head of a construction equipment rental company and he tries to take the role of a house calling jewelry salesman...But is life really better when you live it on the other side of the bed?"));

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
    QCOMPARE(tvshow.firstAiredDate(), QDate(2008, 1, 20));
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
    QCOMPARE(movie.productionCompanyNames(), QStringList() << "PROCIREP" << "Fidélité Films" << "OCS" << "Wild Bunch" << "Mars Distribution" << "TF1 Films Production");
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
    QVERIFY(movie.productionCompanyNames().contains("20th Century Fox"));
    QCOMPARE(movie.budget(), 63000000);
    QCOMPARE(movie.revenue(), 100853753);
    QCOMPARE(movie.runtime(), 139);
}

void SearchTest::testTvShowInfo()
{
    TvShowInfoJob *job = m_api.getTvShowInfo(1396);
    QSignalSpy spy(job, SIGNAL(result(TmdbQt::TvShowInfoJob*)));
    QVERIFY(spy.wait());
    QVERIFY2(!job->hasError(), qPrintable(job->errorMessage()));

    TvShowDb tvshow = job->result();
    QCOMPARE(tvshow.overview(), QStringLiteral("When Walter White, a New Mexico chemistry teacher, is diagnosed with Stage III cancer and given a prognosis of only two years left to live. He becomes filled with a sense of fearlessness and an unrelenting desire to secure his family's financial future at any cost as he enters the dangerous world of drugs and crime."));

    TvSeasonDbList seasons = tvshow.seasons();
    QCOMPARE(seasons.size(), 6);

    QCOMPARE(seasons[0].seasonNumber(), 0);
    QCOMPARE(seasons[1].seasonNumber(), 1);
    QCOMPARE(seasons[2].seasonNumber(), 2);
    QCOMPARE(seasons[3].seasonNumber(), 3);
    QCOMPARE(seasons[4].seasonNumber(), 4);
    QCOMPARE(seasons[5].seasonNumber(), 5);

    QCOMPARE(seasons[0].airDate(), QDate(2009, 2, 17));
    QCOMPARE(seasons[1].airDate(), QDate(2008, 1, 20));
    QCOMPARE(seasons[2].airDate(), QDate(2009, 3, 8));
    QCOMPARE(seasons[3].airDate(), QDate(2010, 3, 21));
    QCOMPARE(seasons[4].airDate(), QDate(2011, 7, 17));
    QCOMPARE(seasons[5].airDate(), QDate(2012, 7, 15));

    QVERIFY(!seasons[0].posterPath().isEmpty());
    QVERIFY(!seasons[1].posterPath().isEmpty());
    QVERIFY(!seasons[2].posterPath().isEmpty());
    QVERIFY(!seasons[3].posterPath().isEmpty());
    QVERIFY(!seasons[4].posterPath().isEmpty());
    QVERIFY(!seasons[5].posterPath().isEmpty());
}

void SearchTest::testTvSeasonInfo()
{
    TvSeasonInfoJob *job = m_api.getTvSeasonInfo(1396, 1);
    QSignalSpy spy(job, SIGNAL(result(TmdbQt::TvSeasonInfoJob*)));
    QVERIFY(spy.wait());
    QVERIFY2(!job->hasError(), qPrintable(job->errorMessage()));

    TvSeasonDb season = job->result();
    QCOMPARE(season.id(), 3572);
    QCOMPARE(season.overview(), QStringLiteral("High school chemistry teacher Walter White's life is suddenly transformed by a dire medical diagnosis. Street-savvy former student Jesse Pinkman \"teaches\" Walter a new trade."));
    QCOMPARE(season.name(), QStringLiteral("Season 1"));

    TvEpisodeDbList episodes = season.episodes();
    QCOMPARE(episodes.size(), 7);

    QCOMPARE(episodes[0].airDate(), QDate(2008, 1, 20));
    QCOMPARE(episodes[0].episodeNumber(), 1);
    QCOMPARE(episodes[0].name(), QStringLiteral("Pilot"));
    QCOMPARE(episodes[0].overview(), QStringLiteral("When an unassuming high school chemistry teacher discovers he has a rare form of lung cancer, he decides to team up with a former student and create a top of the line crystal meth in a used RV, to provide for his family once he is gone."));
    QVERIFY(!episodes[0].stillPath().isEmpty());
}

void SearchTest::testCredits()
{
    CreditsJob *job = m_api.getCredits(15709);
    QSignalSpy spy(job, SIGNAL(result(TmdbQt::CreditsJob*)));
    QVERIFY(spy.wait());
    QVERIFY2(!job->hasError(), qPrintable(job->errorMessage()));
    const PersonList cast = job->cast();
    QVERIFY(cast.count() >= 30);
    const Person firstPerson = cast.at(0);
    QCOMPARE(firstPerson.name(), QStringLiteral("Dany Boon"));
    QCOMPARE(firstPerson.character(), QStringLiteral("Hugo Marciac"));
    QVERIFY2(firstPerson.profilePath().contains(QLatin1String(".jpg")), qPrintable(firstPerson.profilePath()));
    // TODO profileUrl

    const PersonList crew = job->crew();
    QCOMPARE(crew.count(), 11);
    const Person firstCrew = crew.at(0);
    QCOMPARE(firstCrew.name(), QStringLiteral("Olivier Delbosc"));
    QCOMPARE(firstCrew.department(), QStringLiteral("Production"));
    QCOMPARE(firstCrew.job(), QStringLiteral("Producer"));
}

void SearchTest::testEpisodeCredits()
{
    CreditsJob *job = m_api.getEpisodeCredits(1396, 1, 2);
    QSignalSpy spy(job, SIGNAL(result(TmdbQt::CreditsJob*)));
    QVERIFY(spy.wait());
    QVERIFY2(!job->hasError(), qPrintable(job->errorMessage()));
    const PersonList cast = job->cast();
    QCOMPARE(cast.count(), 6);
    const PersonList crew = job->crew();
    QCOMPARE(crew.count(), 4);
}

QTEST_MAIN(SearchTest)

#include "searchtest.moc"
