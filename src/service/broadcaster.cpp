/* 
 * **************************************************************************
 * 
 *  file:       broadcaster.cpp
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
#include "broadcaster.h"
#include "redstatus.h"
#define WANT_MULTICAST


Broadcaster::Broadcaster(int port, QObject* parent)
 : QObject(parent)
 , udpSock4(this)
#ifdef WANT_MULTICAST
 , groupAddr4(QStringLiteral("226.1.1.1"))
#else
 , groupAddr4(QHostAddress::Broadcast)
#endif
 , port(port) {
#ifdef WANT_MULTICAST
  udpSock4.bind(QHostAddress(QHostAddress::AnyIPv4), 0);
  udpSock4.setSocketOption(QAbstractSocket::KeepAliveOption,    1);
  udpSock4.setSocketOption(QAbstractSocket::LowDelayOption,     1);
  udpSock4.setSocketOption(QAbstractSocket::MulticastTtlOption, 1);
#endif
  }


void Broadcaster::sendStatus(const RedStatus& status) {
  QByteArray datagram((char*)&status, sizeof(RedStatus));

  udpSock4.writeDatagram(datagram, groupAddr4, port);
  }
