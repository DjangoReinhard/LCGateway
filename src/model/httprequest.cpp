/* 
 * **************************************************************************
 * 
 *  file:       httprequest.cpp
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
#include "httprequest.h"
#include <QDebug>


HttpRequest::HttpRequest(QObject* parent)
 : HttpMessage(parent)
 , reqType(QStringLiteral("POST")) {
  hdArgs[QStringLiteral("Accept")]       = QStringLiteral("*/*");
  hdArgs[QStringLiteral("Connection")]   = QStringLiteral("keep-alive");
  hdArgs[QStringLiteral("Content-Type")] = QStringLiteral("text/plain");
  }


HttpRequest::HttpRequest(const HttpRequest& other)
 : HttpMessage(other.parent()) {
  if (&other != this)
     *this = other;
  }


//Senders request
HttpRequest::HttpRequest(const QString& uri, const QByteArray& body, QObject* parent)
 : HttpMessage(body, parent)
 , reqType(QStringLiteral("POST"))
 , uri(uri) {
  hdArgs[QStringLiteral("Accept")]       = QStringLiteral("*/*");
  hdArgs[QStringLiteral("Connection")]   = QStringLiteral("keep-alive");
  hdArgs[QStringLiteral("Content-Type")] = QStringLiteral("text/plain");
  }


HttpRequest& HttpRequest::operator=(const HttpRequest &other) {
  if (this == &other) return *this;
  HttpMessage::operator=(other);

  reqType = other.reqType;
  uri     = other.uri;

  return *this;
  }


HttpRequest& HttpRequest::parse(const QString &rawMessage) {
  QStringList parts = rawMessage.split("\r\n");

  if (parts.size() < 3) return *this;
  QStringList sp = parts[0].split(" ");

  if (sp.size() > 2) {
     reqType  = sp.at(0);
     uri      = sp.at(1);
     proto    = sp.at(2);
     }
  parts.pop_front();
  HttpMessage::parse(parts);

  return *this;
  }


void HttpRequest::setRequestType(const QString &type) {
  reqType = type;
  }


QByteArray HttpRequest::toByteArray() const {
  HttpRequest* nct = const_cast<HttpRequest*>(this);
  QByteArray rv;

  nct->hdArgs["Content-Length"] = QString("%0").arg(body.size());
  rv.append(reqType.toLatin1());
  rv.append(' ');
  rv.append(uri.toLatin1());
  rv.append(' ');
  rv.append(proto.toLatin1());
  rv.append("\r\n");

  return HttpMessage::writeMessage(rv);
  }
