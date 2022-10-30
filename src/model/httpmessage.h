/* 
 * **************************************************************************
 * 
 *  file:       httpmessage.h
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
#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H
#include <QObject>
#include <QMap>


class HttpMessage : public QObject
{
  Q_OBJECT
public:
  explicit HttpMessage(QObject *parent = nullptr);
  explicit HttpMessage(const QByteArray& body, QObject* parent = nullptr);
  explicit HttpMessage(const HttpMessage& other);
  virtual ~HttpMessage() = default;

  QString      headerValue(const QString& key) const { return hdArgs[key]; }
  QString      protocol() const { return proto; }
  QByteArray   payload() const { return body; }
  void         setPayLoad(const QString& body);
  void         setHeaderValue(const QString& key, const QString& value);

protected:  
  virtual HttpMessage& operator=(const HttpMessage& other);
  void parse(QStringList& parts);
  virtual QByteArray toByteArray() const = 0;
  QByteArray& writeMessage(QByteArray& buf) const;

  QMap<QString, QString> hdArgs;
  QString proto;
  QByteArray body;
  };
#endif // HTTPMESSAGE_H
