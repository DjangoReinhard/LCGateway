/* 
 * **************************************************************************
 * 
 *  file:       restserver.h
 *  project:    LCGateway
 *  subproject: main application
 *  purpose:    network gateway to linuxcnc nml interface         
 *  created:    16.10.2022 by Django Reinhard
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
#ifndef RESTSERVER_H
#define RESTSERVER_H
#include <QTcpServer>
#include <QMap>
class Authenticator;
class AbstractService;


class RESTServer : public QTcpServer
{
  Q_OBJECT
public:
  explicit RESTServer(Authenticator& auth, int serverPort = 80, QObject *parent = nullptr);

  void setup(const QString& uri, AbstractService* service);
  void startREST();

protected:
  void incomingConnection(qintptr socketDescriptor);

private:
  Authenticator&                  auth;
  int                             RESTport;
  QMap<QString, AbstractService*> services;
  };

#endif // RESTSERVER_H
