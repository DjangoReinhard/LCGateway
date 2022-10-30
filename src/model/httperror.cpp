/* 
 * **************************************************************************
 * 
 *  file:       httperror.cpp
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
#include <httperror.h>
#include <QDateTime>


HttpError::HttpError(ResponseCode error, const QString& serverID, QObject *parent)
 : HttpResponse(error, parent) {
  hdArgs.insert(QStringLiteral("Server"), serverID.toLatin1());
  hdArgs.insert(QStringLiteral("Connection"),   QStringLiteral("close"));
  hdArgs.insert(QStringLiteral("Content-Type"), QStringLiteral("text/html; charset=iso-8859-1"));
  body = QString(ErrTemplate).arg(error).arg(responseType(error)).arg(serverID).toLatin1();
  }


HttpError::HttpError(const HttpError& other)
 : HttpResponse(other) {
  if (&other != this)
     *this = other;
  }


HttpError::~HttpError() {
  }


HttpError& HttpError::operator=(const HttpError &other) {
  if (&other == this) return *this;
  HttpResponse::operator=(other);

  return *this;
  }


//"HTTP/1.1 %1 %2\r\n"
//"Date: %3\r\n"
//"Server: %4\r\n"
//"Content-Length: %5\r\n"

//"Connection: close\r\n"
//"Content-Type: text/html; charset=iso-8859-1\r\n"
const QString HttpError::ErrTemplate = QStringLiteral(
      "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n"
      "<html><head>\n<title>%1 %2</title>\n</head>"
      "<body>\n<h1>%2</h1>\n"
      "<p>%2<br/>\n</p>\n"
      "<hr>\n"
      "<address>%3</address>\n"
      "</body></html>\n"
      );
