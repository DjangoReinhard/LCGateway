/* 
 * **************************************************************************
 * 
 *  file:       httpresponse.h
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
#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H
#include <httpmessage.h>
#include <QLocale>


enum ResponseCode
{
  Unknown
, OK                  = 200
, Accepted            = 202
, BadRequest          = 400
, Unauthorized        = 401
, PaymentRequired     = 402
, Forbidden           = 403
, NotFound            = 404
, MethodNotAllowed    = 405
, NotAcceptable       = 406
, RequestTimeout      = 408
, Payload2Large       = 413
, InternalServerError = 500
, NotImplemented      = 501
, BadGateway          = 502
, ServiceUnavailable  = 503
, GatewayTimeout      = 504
, Invalid
};


class HttpResponse : public HttpMessage
{
public:
  explicit HttpResponse(ResponseCode rc = OK, QObject *parent = nullptr);
  explicit HttpResponse(const HttpResponse& other);
  virtual ~HttpResponse() = default;

  virtual QByteArray toByteArray() const override;

  HttpResponse& operator=(const HttpResponse& other);
  HttpResponse& parse(const QString& line);
  ResponseCode  responseCode() const { return rc; }
  QString       responseType(int rc) const;

private:
  ResponseCode   rc;
  static QLocale usLocale;
  };
#endif // HTTPRESPONSE_H
