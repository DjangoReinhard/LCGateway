/* 
 * **************************************************************************
 * 
 *  file:       authenticator.h
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
#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H
#include <QObject>
#include <QMap>
#include <QRandomGenerator64>
#include <credentials.h>
class HttpRequest;


class Authenticator : public QObject
{
  Q_OBJECT
public:
  explicit Authenticator(QObject *parent = nullptr);

  QString genAuthToken(const QString& uri);
  QString calculateRealm(const QString& uri, const QString& hash) const;
  bool    contains(const QString& uri) const;
  bool    isConfigured() const;
  bool    isValid(const HttpRequest* request) const;
  void    setup(const QString& uri, const QString& user, const QString& password);

protected:
  QString authToken(qint64& v) const;
  QString getToken(const HttpRequest* requests) const;

private:
  QMap<QString, Credentials> config;
  QRandomGenerator64         random;
  QMap<QString, qint64>      authMap;
  };
#endif // AUTHENTICATOR_H
