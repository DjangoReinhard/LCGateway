/* 
 * **************************************************************************
 * 
 *  file:       testclient.cpp
 *  project:    LCGateway
 *  subproject: main application
 *  purpose:    network gateway to linuxcnc nml interface         
 *  created:    17.10.2022 by Django Reinhard
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
#include <testclient.h>
#include <authenticator.h>
#include <httprequest.h>
#include <QEventLoop>
#include <QTcpSocket>
#include <QThread>
#include <QDebug>


TestClient::TestClient(const QHostAddress& addr, int port, Authenticator& auth, QObject *parent)
 : QObject(parent)
 , addr(addr)
 , port(port)
 , auth(auth)
 , sock(nullptr)
 , lastRead(0)
  {
  }


void TestClient::onConnected() {
  qDebug() << "socket connected ...";
  HttpRequest r;

  if (!lastRead) {
     connect(sock, &QTcpSocket::readyRead, this, &TestClient::onDataRead);
     r = HttpRequest("/linuxcnc/auth", QByteArray());

     r.setRequestType("GET");
     }
  else {
     QString body("was geht ab?");
     r = HttpRequest("/linuxcnc/command", body.toLatin1());
     r.setHeaderValue(QStringLiteral("Realm"), auth.calculateRealm(r.path()
                                                                 , hash));
     }
  qint64 bytesWritten = sock->write(r.toByteArray());

  qDebug() << "wrote" << bytesWritten << "bytes";
  }


void TestClient::onDisconnect() {
  qDebug() << "Bye";
  }


void TestClient::onDataRead() {
  char buffer[1024];
  int  bytesRead = sock->read(buffer, 1024);

  if (!bytesRead) return;
  if (bytesRead < 1024) buffer[bytesRead] = '\0';
  hash = QString(buffer);

  qDebug() << "read from socket: " << hash << " " << hash.size() << "bytes";
  sock->close();
  if (!lastRead) lastRead = hash.size();

//  QThread::sleep(3); // test invalid auth-token

  sock->connectToHost(addr, port);
  }


void TestClient::run() {
  sock  = new QTcpSocket();
  sock->setSocketOption(QTcpSocket::LowDelayOption, 1);
  connect(sock, &QTcpSocket::connected, this, &TestClient::onConnected);
  connect(sock, &QTcpSocket::disconnected, this, &TestClient::onDisconnect);
  sock->connectToHost(addr, port);
  }
