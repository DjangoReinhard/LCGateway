/* 
 * **************************************************************************
 * 
 *  file:       authenticator.cpp
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
#include <authenticator.h>
#include <httprequest.h>
#include <QDateTime>
#include <QCryptographicHash>
#include <QDebug>


Authenticator::Authenticator(QObject *parent)
 : QObject(parent) {
  }


QString Authenticator::calculateRealm(const QString& uri, const QString& authToken) const {
  Credentials c     = config.value(uri);
  QString     src   = QStringLiteral("%1%2%3").arg(authToken, c.user, c.password);
  QString     token = QString(QCryptographicHash::hash(src.toLatin1(), QCryptographicHash::Md5).toHex()).toUpper();

  qDebug() << "calculated token:" << token;

  return token;
  }


bool Authenticator::contains(const QString &uri) const {
  return config.contains(uri);
  }


QString Authenticator::genAuthToken(const QString& uri) {
  qint64  value = 0;
  QString token = authToken(value);

  authMap.insert(uri, value);

  qDebug() << "A::genAuthToken(" << value << ") for " << uri << "\t=>\t" << token;

  return token;
  }


bool Authenticator::isConfigured() const {
  return config.size() > 0;
  }


bool Authenticator::isValid(const HttpRequest* request) const {
  if (!request
   || !config.contains(request->path())) return false;

  qDebug() << "A::isValid(): uri ........: " << request->path();
  qDebug() << "A::isValid(): client realm: " << request->headerValue(QStringLiteral("Realm"));

  QString token = getToken(request);
  QString realm = calculateRealm(request->path(), token);

  qDebug() << "A::isValid(): client token ...:" << token;
  qDebug() << "A::isValid(): calculated realm:" << realm;

  return request->headerValue(QStringLiteral("Realm")) == realm;
  }


void Authenticator::setup(const QString &uri, const QString &user, const QString &password) {
//  QString password = QString(QCryptographicHash::hash(rawPW.toLatin1(), QCryptographicHash::Md5).toHex()).toUpper();
  config.insert(uri, Credentials(user, password));
  }


QString Authenticator::authToken(qint64& value) const {
  qint64  now = ((QDateTime::currentSecsSinceEpoch() + 1) / 2) * 2;

  value = const_cast<QRandomGenerator64*>(&random)->generate() & std::numeric_limits<qint64>::max();
  QString src = QStringLiteral("%1%2").arg(now, 0, 16).arg(value, 0, 16);

//  qDebug() << "A::authToken(): value to save" << value;

  return QString(QCryptographicHash::hash(src.toLatin1(), QCryptographicHash::Md5).toHex()).toUpper();
  }


QString Authenticator::getToken(const HttpRequest* request) const {
  qint64  now = ((QDateTime::currentSecsSinceEpoch() + 1) / 2) * 2;
  qint64  val = authMap[request->path()];
  QString src = QStringLiteral("%1%2").arg(now, 0, 16).arg(val, 0, 16);

  qDebug() << "getToken(): restored value:" << val;

  return QString(QCryptographicHash::hash(src.toLatin1(), QCryptographicHash::Md5).toHex()).toUpper();
  }
