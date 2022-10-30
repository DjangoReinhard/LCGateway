/* 
 * **************************************************************************
 * 
 *  file:       authhandler.h
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
#ifndef AUTHHANDLER_H
#define AUTHHANDLER_H
#include <abstractservice.h>
class Authenticator;


class AuthHandler : public AbstractService
{
  Q_OBJECT
public:
  explicit AuthHandler(Authenticator& auth, QObject *parent = nullptr);
  virtual ~AuthHandler() = default;

  bool    isRestricted() const override { return false; }
  void    service(const HttpRequest& r, QTcpSocket& s) override;
  QString requiredRequestType() const override { return QStringLiteral("GET"); }


private:
  Authenticator& auth;
  };
#endif // AUTHHANDLER_H
