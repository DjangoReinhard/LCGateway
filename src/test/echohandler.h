/* 
 * **************************************************************************
 * 
 *  file:       echohandler.h
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
#ifndef ECHOHANDLER_H
#define ECHOHANDLER_H
#include <abstractservice.h>
class Authenticator;


class EchoHandler : public AbstractService
{
  Q_OBJECT
public:
  explicit EchoHandler(Authenticator& auth, QObject* parent = nullptr);
  virtual ~EchoHandler() = default;

  virtual bool    isRestricted() const;
  virtual QString requiredRequestType() const;
  virtual void    service(const HttpRequest& request, QTcpSocket& socket);

private:
  Authenticator& auth;
  };
#endif // ECHOHANDLER_H
