/* 
 * **************************************************************************
 * 
 *  file:       unittests.h
 *  project:    LCGateway
 *  subproject: main application
 *  purpose:    network gateway to linuxcnc nml interface         
 *  created:    23.10.2022 by Django Reinhard
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
#ifndef UNITTESTS_H
#define UNITTESTS_H
#include <QtTest/QtTest>
#include <QObject>
class Authenticator;
class QTcpSocket;


class UnitTests : public QObject
{
  Q_OBJECT
public slots:
  void onConnected();
  void onDisconnect();
  void onDataRead();


private slots:
  void testRequest();
  void testResponse();
  void testError();
  void testEStopCommand();
  void testHostAddress();
  void testRequestParsing();
  void testResponseParsing();
  void testAuthRequest();

private:
  QTcpSocket*    sock;
  QEventLoop*    eLoop;
  Authenticator* auth;
  QByteArray     body;
  QString        nonce;
  };
#endif // UNITTESTS_H
