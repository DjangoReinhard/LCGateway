/* 
 * **************************************************************************
 * 
 *  file:       restserver.cpp
 *  project:    LCGateway
 *  subproject: main application
 *  purpose:    network gateway to linuxcnc nml interface         
 *  created:    27.10.2022 by Django Reinhard
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
#include <restserver.h>
#include <restclient.h>
#include <authenticator.h>
#include <QNetworkInterface>
#include <QThreadPool>
#include <QTcpServer>
#include <QDebug>
#include <QException>
#include <exception>


class ClientStarter : public QRunnable
{
public:
  explicit ClientStarter(qintptr socketDescriptor, Authenticator& auth, const QMap<QString, AbstractService*>& services)
   : sockDes(socketDescriptor)
   , auth(auth)
   , services(services) {
    }

  void run() {
    try {
        RESTClient* rc = new RESTClient(sockDes, auth, services);

        qDebug() << "new client connected ...";

        rc->run();
        }
    catch (QException* e) {
        qDebug() << "client failed with" << e->what();
        }
    catch (QException& e) {
        qDebug() << "client failed with" << e.what();
        }
    }

private:
  qintptr        sockDes;
  Authenticator& auth;
  const QMap<QString, AbstractService*>& services;
  };


RESTServer::RESTServer(Authenticator& auth, int serverPort, QObject *parent)
 : QTcpServer(parent)
 , auth(auth)
 , RESTport(serverPort) {
  }


void RESTServer::incomingConnection(qintptr socketDescriptor) {
  ClientStarter* cs = new ClientStarter(socketDescriptor, auth, services);

  QThreadPool::globalInstance()->start(cs);
  }


void RESTServer::setup(const QString &uri, AbstractService* service) {
  services.insert(uri, service);
  }


void RESTServer::startREST() {
  if (auth.isConfigured()) {      // start command channel with valid setup only
     QString sAddr;
     QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
     QHostAddress addr;

     // search first non-localhost IPv4 address
     for (int i = 0; i < ipAddressesList.size(); ++i) {
         if (ipAddressesList.at(i) != QHostAddress::LocalHost
          && ipAddressesList.at(i).toIPv4Address()) {
            addr = ipAddressesList.at(i);
            sAddr = addr.toString();
            break;
            }
         }
     // if we did not find one, use IPv4 localhost
     if (sAddr.isEmpty()) {
        addr = QHostAddress::LocalHost;
        sAddr = addr.toString();
        }

     if (!listen(addr, RESTport)) {
        throw new std::domain_error(tr("Unable to start the server: %1.")
                                   .arg(errorString()).toStdString());
        }

     qDebug() << "The server is running on IP:" << sAddr
              << "port:" << RESTport;
     }
  }
