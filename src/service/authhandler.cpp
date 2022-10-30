/* 
 * **************************************************************************
 * 
 *  file:       authhandler.cpp
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
#include <authhandler.h>
#include <authenticator.h>
#include <httprequest.h>
#include <QCryptographicHash>
#include <QDateTime>
#include <QTcpSocket>
#include <QDebug>


AuthHandler::AuthHandler(Authenticator& auth, QObject *parent)
 : AbstractService(parent)
 , auth(auth) {
  }


void AuthHandler::service(const HttpRequest& r, QTcpSocket& s) {
  QString token = auth.genAuthToken(r.path());
  s.write(token.toLatin1());
  s.flush();
  s.close();
  }
