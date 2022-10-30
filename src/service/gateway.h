/* 
 * **************************************************************************
 * 
 *  file:       gateway.h
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
#ifndef GATEWAY_H
#define GATEWAY_H
#include <broadcaster.h>
#include <statusreader.h>
#include <QObject>
#include <QTimer>
class RESTServer;
class Authenticator;
#define WANT_NML_STUFF


class Gateway : public QObject
{
  Q_OBJECT
public:
  Gateway(Authenticator& auth, int port = 4321, int srvPort = 80, int delay = 500, QObject* parent = nullptr);

protected:
  void ping();

private:
  RESTServer*     h;
#ifdef WANT_NML_STUFF
  Broadcaster     b;
  StatusNmlReader r;
  RedStatus*      s;
  QTimer          t;
#endif
  };
#endif // GATEWAY_H
