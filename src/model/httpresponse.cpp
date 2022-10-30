/* 
 * **************************************************************************
 * 
 *  file:       httpresponse.cpp
 *  project:    LCGateway
 *  subproject: main application
 *  purpose:    network gateway to linuxcnc nml interface         
 *  created:    22.10.2022 by Django Reinhard
 *  copyright:  (c) 2022 Django Reinhard -  all rights reserved
 * 
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 2 of the License, or 
 *  (at your option) any later version. 
 *   
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details. 
 *   
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * **************************************************************************
 */
#include <httpresponse.h>
#include <QDateTime>


HttpResponse::HttpResponse(ResponseCode rc, QObject *parent)
 : HttpMessage(parent)
 , rc(rc) {
  hdArgs.insert(QStringLiteral("Date"), usLocale.toString(QDateTime::currentDateTimeUtc()
                                                        , "ddd, dd MMM yyyy hh:mm:ss t"));
  }


HttpResponse::HttpResponse(const HttpResponse& other)
 : HttpMessage(other) {
  if (&other != this)
     *this = other;
  }


HttpResponse& HttpResponse::operator=(const HttpResponse &other) {
  if (&other == this) return *this;
  HttpMessage::operator=(other);

  rc = other.rc;

  return *this;
  }


HttpResponse& HttpResponse::parse(const QString &rawMessage) {
  QStringList parts = rawMessage.split("\r\n");

  if (parts.size() < 3) return *this;
  QStringList sp = parts[0].split(" ");

  if (sp.size() > 2) {
     bool ok = false;

     proto = sp.at(0);
     rc    = static_cast<ResponseCode>(sp.at(1).toInt(&ok));
     }
  parts.pop_front();
  HttpMessage::parse(parts);

  return *this;
  }


QString HttpResponse::responseType(int rc) const {
  switch (rc) {
    case 200: return QStringLiteral("OK");              // successfully processed
    case 202: return QStringLiteral("Accepted");        // accepted, will be processed
    case 400: return QStringLiteral("Bad Request");
    case 401: return QStringLiteral("Unauthorized");
    case 402: return QStringLiteral("Payment required");
    case 403: return QStringLiteral("Forbidden");
    case 404: return QStringLiteral("Not found");
    case 405: return QStringLiteral("Method Not allowed");
    case 406: return QStringLiteral("Not acceptable");
    case 408: return QStringLiteral("Request timeout");
    case 413: return QStringLiteral("Payload too large");
    case 500: return QStringLiteral("Internal Server error");
    case 501: return QStringLiteral("Not implemented");
    case 502: return QStringLiteral("Bad gateway");
    case 503: return QStringLiteral("Service unavailable");
    case 504: return QStringLiteral("Gateway timeout");
    default:  return QStringLiteral("Unknown error");
    }
  }


QByteArray HttpResponse::toByteArray() const {
  HttpResponse* nct = const_cast<HttpResponse*>(this);
  QByteArray rv;

  nct->hdArgs["Content-Length"] = QString("%0").arg(body.size());
  rv.append(proto.toLatin1());
  rv.append(' ');
  rv.append(QString("%1").arg(rc).toLatin1());
  rv.append(' ');
  rv.append(responseType(rc).toLatin1());
  rv.append("\r\n");

  return HttpMessage::writeMessage(rv);
  }


QLocale HttpResponse::usLocale = QLocale("EN_US");
