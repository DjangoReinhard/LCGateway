/* 
 * **************************************************************************
 * 
 *  file:       restclient.h
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
#ifndef RESTCLIENT_H
#define RESTCLIENT_H
#include <QRunnable>
#include <QEventLoop>
#include <QDataStream>
#include <QTcpSocket>
class HttpRequest;
class Authenticator;
class AbstractService;


class RESTClient : public QObject, public QRunnable
{
  Q_OBJECT
public:
  explicit RESTClient(qintptr socketDescriptor, Authenticator& auth, const QMap<QString, AbstractService*>& services, QObject* parent = nullptr);

  void onDisconnect();
  void run() override;

  void read();
  void error();

protected:
  void runService(AbstractService* service, const HttpRequest& request, QTcpSocket& socket);

private:
  QEventLoop     eLoop;
  QTcpSocket     sock;
  Authenticator& auth;
  const QMap<QString, AbstractService*>& services;
  };

#endif // RESTCLIENT_H
