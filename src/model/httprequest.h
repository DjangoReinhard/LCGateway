/* 
 * **************************************************************************
 * 
 *  file:       httprequest.h
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
#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include <httpmessage.h>


class HttpRequest : public HttpMessage
{
  Q_OBJECT
public:
  explicit HttpRequest(QObject* parent = nullptr);
  explicit HttpRequest(const HttpRequest& other);
  explicit HttpRequest(const QString& uri, const QByteArray& body, QObject* parent = nullptr);
  virtual ~HttpRequest() = default;

  virtual QByteArray toByteArray() const override;

  HttpRequest& operator=(const HttpRequest& other);
  HttpRequest& parse(const QString& rawMessage);
  QString      path() const     { return uri; }
  void         setRequestType(const QString& type);
  QString      type() const     { return reqType; }

private:
  QString reqType;
  QString uri;
  };
#endif // HTTPREQUEST_H
