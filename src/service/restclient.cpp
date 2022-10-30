/* 
 * **************************************************************************
 * 
 *  file:       restclient.cpp
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
#include <restclient.h>
#include <httprequest.h>
#include <httpresponse.h>
#include <httperror.h>
#include <abstractservice.h>
#include <authenticator.h>
#include <QApplication>
#include <QHostAddress>
#include <QThreadPool>
#include <QTcpSocket>
#include <QException>
#include <iostream>


RESTClient::RESTClient(qintptr socketDescriptor, Authenticator& auth, const QMap<QString, AbstractService*>& services, QObject* parent)
 : QObject(parent)
 , sock(&eLoop)
 , auth(auth)
 , services(services) {
  sock.setSocketDescriptor(socketDescriptor);
  connect(&sock, &QAbstractSocket::disconnected, this, &RESTClient::onDisconnect);
  connect(&sock, &QIODevice::readyRead, this, &RESTClient::read);
  connect(&sock, &QAbstractSocket::errorOccurred, this, &RESTClient::error);
  }


void RESTClient::onDisconnect() {
  disconnect(&sock, &QAbstractSocket::disconnected, this, &RESTClient::onDisconnect);
  disconnect(&sock, &QIODevice::readyRead, this, &RESTClient::read);
  disconnect(&sock, &QAbstractSocket::errorOccurred, this, &RESTClient::error);
  qDebug() << "client disconnected ...";
  eLoop.exit();
  }


void RESTClient::run() {
  qDebug() << "start client in new thread ...";
  QHostAddress pa = sock.peerAddress();
  QHostAddress pl = sock.localAddress();

  qDebug() << "connection from:" << pa << "\tto\t" << pl;

  eLoop.exec();

  qDebug() << "\tRESTClient ended ...\n";
  }


void RESTClient::runService(AbstractService* service, const HttpRequest& request, QTcpSocket& socket) {
  service->service(request, socket);
  socket.close();
  }


void RESTClient::read() {
  char buffer[1024];
  int  bytesRead = sock.read(buffer, 1024);

  qDebug() << "read" << bytesRead << "from socket";

  if (!bytesRead) return;
  if (bytesRead < 1024) buffer[bytesRead] = '\0';
  else buffer[1023] = '\0';
  HttpRequest req(HttpRequest().parse(buffer));

  if (services.contains(req.path())) {
     AbstractService* s = services[req.path()];

     qDebug() << "found service for URL (" << req.path() << ")";
     if (req.type() == s->requiredRequestType()) {
        qDebug() << "request-type did match";
        try {
            if (s->isRestricted()) {
               qDebug() << "requested URL is restricted";

               if (auth.isValid(&req)) {
                  qDebug() << "authentication passed, run service ...";
                  HttpResponse r(ResponseCode::Accepted);
                  QByteArray msg = r.toByteArray();

                  sock.write(msg);
                  runService(s, req, sock);
                  sock.flush();
                  sock.close();
                  }
               else {
                  qDebug() << "OUPS - authentication failed!";
                  HttpError err(ResponseCode::Unauthorized, QApplication::instance()->applicationName());
                  QString nonce = auth.genAuthToken(req.path());

                  qDebug() << "\tRESTclient: auth token to send is" << nonce;
                  err.setHeaderValue(QStringLiteral("nonce"), nonce);
                  QByteArray msg = err.toByteArray();

                  sock.write(msg);
                  sock.flush();
                  sock.close();
                  }
               }
            else {
               qDebug() << "URL is not restricted, so run service ...";
               runService(s, req, sock);
               }
            }
        catch (QException* e) {
            qDebug() << "\tERROR occurred:" << e->what();
            sock.flush();
            sock.close();
            }
        }
     else {
        qDebug() << "OUPS - request type does not match!" << req.type();
        HttpError err(ResponseCode::BadRequest, QApplication::instance()->applicationName());

        QByteArray msg = err.toByteArray();

        sock.write(msg);
        sock.flush();
        sock.close();
        }
     }
  else {
     qDebug() << "OUPS - requested URL not configured! (" << req.path() << ")";
     HttpError err(ResponseCode::NotFound, QApplication::instance()->applicationName());

     QByteArray msg = err.toByteArray();

     sock.write(msg);
     sock.flush();
     sock.close();
     }
  }


void RESTClient::error() {
  qDebug() << "OUPS:" << sock.errorString();
  }
