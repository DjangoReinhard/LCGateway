/* 
 * **************************************************************************
 * 
 *  file:       httpmessage.cpp
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
#include "httpmessage.h"


HttpMessage::HttpMessage(QObject *parent)
 : QObject(parent)
 , proto(QStringLiteral("HTTP/1.1")) {
  }


HttpMessage::HttpMessage(const QByteArray& body, QObject* parent)
 : QObject(parent)
 , proto(QStringLiteral("HTTP/1.1"))
 , body(body) {
  }


HttpMessage::HttpMessage(const HttpMessage& other)
 : QObject(other.parent()) {
  if (&other != this)
     *this = other;
  }


HttpMessage& HttpMessage::operator=(const HttpMessage& other) {
  if (&other == this) return *this;
  hdArgs  = other.hdArgs;
  proto   = other.proto;
  body    = other.body;

  return *this;
  }


// POST /linuxcnc/command HTTP/1.1
// Realm: what-ever
// Content-Type: text/plain
// User-Agent: PostmanRuntime/7.29.2
// Accept: */*
// Postman-Token: f581c4d3-4608-4cda-b5c6-928b34fd8a47
// Host: 192.39.29.19:1234
// Accept-Encoding: gzip, deflate, br
// Connection: keep-alive
// Content-Length: 19
//
// [was geht ab mann?]
void HttpMessage::parse(QStringList& parts) {
  QRegExp reg(":\\s+");

  if (parts.size()) {
     while (parts.size()) {
           QStringList sub = parts[0].split(reg);

           parts.pop_front();
           if (sub.size() > 1) {
              hdArgs[sub[0]] = sub[1];
              }
           else break;
           }
     body = parts.join("\r\n").toLatin1();        // undo initial splitting
     }
  }


void HttpMessage::setHeaderValue(const QString &key, const QString &value) {
  hdArgs.insert(key, value);
  }


void HttpMessage::setPayLoad(const QString &payload) {
  body = payload.toLatin1();
  }


QByteArray& HttpMessage::writeMessage(QByteArray& buf) const {
  for (auto i = hdArgs.keyBegin(); i != hdArgs.keyEnd(); ++i) {
      QString k = *i;

      if (k == QStringLiteral("Content-Length")) continue;
      buf.append(k.toLatin1());
      buf.append(": ");
      buf.append(hdArgs.value(k).toLatin1());
      buf.append("\r\n");
      }
  QString k = QStringLiteral("Content-Length");

  buf.append(k.toLatin1());
  buf.append(": ");
  buf.append(hdArgs.value(k).toLatin1());
  buf.append("\r\n\r\n");
  buf.append(body);

  return buf;
  }
