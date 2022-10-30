/* 
 * **************************************************************************
 * 
 *  file:       httperror.h
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
#ifndef HTTPERROR_H
#define HTTPERROR_H
#include <httpresponse.h>


class HttpError : public HttpResponse
{
public:
  explicit HttpError(ResponseCode error, const QString& serverID, QObject *parent = nullptr);
  explicit HttpError(const HttpError& other);
  virtual ~HttpError();

  HttpError& operator=(const HttpError& other);

private:
  static const QString ErrTemplate;
  };
#endif // HTTPERROR_H
