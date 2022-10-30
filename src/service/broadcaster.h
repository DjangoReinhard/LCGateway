/* 
 * **************************************************************************
 * 
 *  file:       broadcaster.h
 *  project:    LCGateway
 *  subproject: main application
 *  purpose:    network gateway to linuxcnc nml interface         
 *  created:    13.10.2022 by Django Reinhard
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
#ifndef BROADCASTER_H
#define BROADCASTER_H
#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
class RedStatus;


class Broadcaster : public QObject
{
  Q_OBJECT
public:  
  explicit Broadcaster(int port = 4321, QObject* parent = nullptr);
  virtual ~Broadcaster() = default;

  void sendStatus(const RedStatus& status);

private:
  QUdpSocket        udpSock4;
  QHostAddress      groupAddr4;
  int               port;
  };
#endif // BROADCASTER_H
