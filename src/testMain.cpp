/* 
 * **************************************************************************
 * 
 *  file:       testMain.cpp
 *  project:    LCGateway
 *  subproject: main application
 *  purpose:    network gateway to linuxcnc nml interface         
 *  created:    21.10.2022 by Django Reinhard
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
#include <testclient.h>
#include <authenticator.h>
#include <QApplication>
#include <QCryptographicHash>
#include <QUnhandledException>
#include <QThreadPool>
#include <iostream>


int main(int argc, char *argv[]) {
  int rv = -1;

  try {
      QApplication  a(argc, argv);
      Authenticator auth;
//      TestClient    c(QHostAddress("10.9.9.19"), 62345, auth);
      TestClient    c(QHostAddress("10.9.9.19"), 80, auth);
      QString       pw = QString(QCryptographicHash::hash("ich bins", QCryptographicHash::Md5).toHex()).toUpper();

      auth.setup("/linuxcnc/command", "lcuser", pw);
      c.run();
      rv = a.exec();
      }
  catch (std::exception& e) {
      std::cerr << "!!! Exception occurred: " << e.what() << std::endl;
      }
  catch (std::exception* e) {
      std::cerr << "!!! Exception occurred: " << e->what() << std::endl;
      }
  return rv;
  }
