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

void SearchTest::testTvShowInfo()
{
    TvShowInfoJob *job = m_api.getTvShowInfo(1396);
    QSignalSpy spy(job, SIGNAL(result(TmdbQt::TvShowInfoJob*)));
    QVERIFY(spy.wait());
    QVERIFY2(!job->hasError(), qPrintable(job->errorMessage()));

    TvShowDb tvshow = job->result();
    QCOMPARE(tvshow.overview(), QStringLiteral("Breaking Bad is an American crime drama television series created and produced by Vince Gilligan. Set and produced in Albuquerque, New Mexico, Breaking Bad is the story of Walter White, a struggling high school chemistry teacher who is diagnosed with inoperable lung cancer at the beginning of the series. He turns to a life of crime, producing and selling methamphetamine, in order to secure his family's financial future before he dies, teaming with his former student, Jesse Pinkman. Heavily serialized, the series is known for positioning its characters in seemingly inextricable corners and has been labeled a contemporary western by its creator."));

    TvSeasonDbList seasons = tvshow.seasons();
    QCOMPARE(seasons.size(), 6);

    QCOMPARE(seasons[0].seasonNumber(), 0);
    QCOMPARE(seasons[1].seasonNumber(), 1);
    QCOMPARE(seasons[2].seasonNumber(), 2);
    QCOMPARE(seasons[3].seasonNumber(), 3);
    QCOMPARE(seasons[4].seasonNumber(), 4);
    QCOMPARE(seasons[5].seasonNumber(), 5);

    QCOMPARE(seasons[0].airDate(), QDate(2009, 2, 17));
    QCOMPARE(seasons[1].airDate(), QDate(2008, 1, 19));
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
    QCOMPARE(season.overview(), QStringLiteral("The first season of the American television drama series Breaking Bad premiered on January 20, 2008 and concluded on March 9, 2008. It consisted of seven episodes, each running approximately 47 minutes in length, except the pilot episode which runs approximately 57 minutes. AMC broadcast the first season on Sundays at 10:00 pm in the United States. Season one was to consist of nine episodes, which was reduced to seven by the writer's strike. The complete first season was released on Region 1 DVD on February 24, 2009 and Region A Blu-ray on March 16, 2010."));
    QCOMPARE(season.name(), QStringLiteral("Season 1"));

    TvEpisodeDbList episodes = season.episodes();
    QCOMPARE(episodes.size(), 7);

    QCOMPARE(episodes[0].airDate(), QDate(2008, 1, 19));
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
    QCOMPARE(cast.count(), 14);
    const Person firstPerson = cast.at(0);
    QCOMPARE(firstPerson.name(), QStringLiteral("Dany Boon"));
    QCOMPARE(firstPerson.character(), QStringLiteral("Hugo Marciac"));
    QVERIFY2(firstPerson.profilePath().contains(QLatin1String(".jpg")), qPrintable(firstPerson.profilePath()));
    // TODO profileUrl

    const PersonList crew = job->crew();
    QCOMPARE(crew.count(), 11);
    const Person firstCrew = crew.at(0);
    QCOMPARE(firstCrew.name(), QStringLiteral("Pascale Pouzadoux"));
    QCOMPARE(firstCrew.department(), QStringLiteral("Directing"));
    QCOMPARE(firstCrew.job(), QStringLiteral("Director"));
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
