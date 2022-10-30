/* 
 * **************************************************************************
 * 
 *  file:       testclient.h
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
#ifndef TESTCLIENT_H
#define TESTCLIENT_H
#include <QObject>
#include <QRunnable>
#include <QHostAddress>
class QEventLoop;
class QTcpSocket;
class Authenticator;


class TestClient : public QObject, public QRunnable
{
  Q_OBJECT
public:
  explicit TestClient(const QHostAddress& addr, int port, Authenticator& auth, QObject *parent = nullptr);

  void onConnected();
  void onDisconnect();
  void onDataRead();
  void run();

private:
  QHostAddress   addr;
  int            port;
  Authenticator& auth;
//  QEventLoop*    eLoop;
  QTcpSocket*    sock;
  int            lastRead;
  QString        hash;
  };

#endif // TESTCLIENT_H
