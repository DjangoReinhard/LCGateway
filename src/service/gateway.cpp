/* 
 * **************************************************************************
 * 
 *  file:       gateway.cpp
 *  project:    LCGateway
 *  subproject: main application
 *  purpose:    network gateway to linuxcnc nml interface         
 *  created:    26.10.2022 by Django Reinhard
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
#include <gateway.h>
#include <authhandler.h>
#include <echohandler.h>
#include <commandhandler.h>
#include <redstatus.h>
#include <restserver.h>
#include <QDebug>
//#define WANT_BENCH


Gateway::Gateway(Authenticator& a, int port, int srvPort, int delay, QObject* parent)
 : QObject(parent)
 , h(new RESTServer(a, srvPort, this))
#ifdef WANT_NML_STUFF
 , b(port) {
  s = new RedStatus(r.status());

  qDebug() << "start NML-multicast-gateway with port:" << port << "and refresh-rate:" << delay;

  connect(&t, &QTimer::timeout, this, &Gateway::ping);
  t.start(delay);
#else
  {
#endif
  /*
   *  setup service-host
   */
  h->startREST();
#ifdef WANT_NML_STUFF
  h->setup(QStringLiteral("/linuxcnc/command"), new CommandHandler());
#else
  h->setup(QStringLiteral("/linuxcnc/command"), new EchoHandler(a));
#endif
  }


void Gateway::ping() {
#ifdef WANT_NML_STUFF
#ifdef WANT_BENCH
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
#endif

  r.read();
  b.sendStatus(*s = r.status());

#ifdef WANT_BENCH
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

  qDebug() << "gateway ping took" << time_span.count() * 1000 << " milli-seconds";
#endif
#endif
  }
