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

#include "jobparams_p.h"
#include <QDebug>
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

using namespace TmdbQt;

QNetworkReply *JobParams::get(const QUrl &url) const
{
    qDebug() << "GET" << url;

    // Resolve and prefer IPv4. Anycast IPv6 paths to api.themoviedb.org are flaky
    // on some networks (notably the user's Windows box): RST during headers or
    // ~300 B/s throughput. IPv4 paths are reliable for the same host, so we
    // rewrite to a literal IPv4 and keep the Host header / TLS SNI on the
    // original hostname.
    QUrl rewritten = url;
    const QString host = url.host();
    if (!host.isEmpty() && QHostAddress(host).isNull()) {
        // Not already a literal IP — look it up (cached after the first call).
        if (m_cachedHost != host) {
            const QHostInfo info = QHostInfo::fromName(host);
            QString ipv4;
            if (info.error() == QHostInfo::NoError) {
                for (const QHostAddress &a : info.addresses()) {
                    if (a.protocol() == QAbstractSocket::IPv4Protocol) {
                        ipv4 = a.toString();
                        break;
                    }
                }
            } else {
                qWarning() << "TmdbQt: DNS lookup failed for" << host << ":"
                           << info.errorString();
            }
            m_cachedHost = host;
            m_cachedIPv4 = ipv4;
        }
        if (!m_cachedIPv4.isEmpty()) {
            rewritten.setHost(m_cachedIPv4);
        }
    }

    QNetworkRequest request(rewritten);
    if (rewritten.host() != host) {
        // We connect to the IPv4 literal; tell the server (Host header) and the
        // TLS layer (SNI + cert verification) about the real hostname.
        request.setRawHeader("Host", host.toUtf8());
        request.setPeerVerifyName(host);
    }
    // Stash the original (hostname-bearing) URL so retryIfTransient can reissue
    // through this same path and re-resolve / re-rewrite if needed.
    request.setAttribute(QNetworkRequest::User, url);
    // Qt 6 enables HTTP/2 by default. Concurrent HTTP/2 streams to
    // api.themoviedb.org (e.g. one request per TV season, fired in parallel)
    // are intermittently dropped on Windows. Force HTTP/1.1.
    request.setAttribute(QNetworkRequest::Http2AllowedAttribute, false);
    return qnam.get(request);
}

QNetworkReply *JobParams::retryIfTransient(QNetworkReply *failedReply, int &retriesLeft) const
{
    const QNetworkReply::NetworkError err = failedReply->error();
    const bool transient = (err == QNetworkReply::RemoteHostClosedError
                            || err == QNetworkReply::TimeoutError
                            || err == QNetworkReply::TemporaryNetworkFailureError
                            || err == QNetworkReply::ConnectionRefusedError);
    if (!transient || retriesLeft <= 0) {
        return nullptr;
    }
    --retriesLeft;
    const QUrl originalUrl =
        failedReply->request().attribute(QNetworkRequest::User).toUrl();
    qWarning() << "TmdbQt: retrying" << originalUrl << "after transient error:"
               << failedReply->errorString();
    failedReply->deleteLater();
    return get(originalUrl);
}
