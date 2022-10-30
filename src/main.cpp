/* 
 * **************************************************************************
 * 
 *  file:       main.cpp
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
#include <authenticator.h>
#include <gateway.h>
#include <QApplication>
#include <QCryptographicHash>
#include <QSettings>
#include <QDateTime>
#include <QInputDialog>
#include <QLineEdit>
#include <QUnhandledException>
#include <QDebug>
#include <iostream>


void processArgs(const QStringList& a, int& mcPort, int& mcDelay, int& srvPort, QString& user, QString& pw) {
  for (int i=0; i < a.size(); ++i) {
      QString s = a.at(i);
      bool ok = false;
      int tmp;

      if (s.startsWith("-")) {
         QString id = s.mid(1);

         if (id == QStringLiteral("mc-port") && a.size() > i+1) {
            tmp = a.at(++i).toInt(&ok);

            if (ok) mcPort = tmp;
            }
         else if (id == QStringLiteral("mc-delay") && a.size() > i+1) {
            tmp = a.at(++i).toInt(&ok);

            if (ok) mcDelay = tmp;
            }
         else if (id == QStringLiteral("srv-port") && a.size() > i+1) {
            tmp = a.at(++i).toInt(&ok);

            if (ok) srvPort = tmp;
            }
         else if (id == QStringLiteral("user") && a.size() > i+1) {
            user = a.at(++i);
            bool    ok;
            QString text = QInputDialog::getText(nullptr
                                               , QStringLiteral("Password for Command-Channel")
                                               , QString(QStringLiteral("password for user %1")).arg(user)
                                               , QLineEdit::PasswordEchoOnEdit
                                               , QStringLiteral("unset")
                                               , &ok);
            if (!ok) throw new std::domain_error("user abort!");
            if (ok && !text.isEmpty()) {
               pw = QString(QCryptographicHash::hash(text.toLatin1(), QCryptographicHash::Md5).toHex()).toUpper();
               }
            }
         else if (id == "-help") {
            std::cout << "LCGateway [options]" << std::endl;
            std::cout << "\tOptions:" << std::endl;
            std::cout << "\t-mc-port\tnetwork multicast port" << std::endl;
            std::cout << "\t-mc-delay\tpause to wait between two status reads" << std::endl;
            std::cout << "\t-srv-port\tport for REST-service" << std::endl;
            std::cout << "\t-user\tusername to enable command channel" << std::endl;
            std::cout << "\t--help this text." << std::endl << std::endl;

            exit(-1);
            }
         }
      }
  }


void restore(const QString& an, int& mcPort, int& mcDelay, int& srvPort, QString& user, QString& pw) {
  QSettings cfg(QSettings::SystemScope, an);

  cfg.beginGroup(QStringLiteral("status"));
  mcPort  = cfg.value(QStringLiteral("MulticastPort"), 4321).toInt();
  mcDelay = cfg.value(QStringLiteral("MulticastDelay"), 100).toInt();
  cfg.endGroup();

  cfg.beginGroup(QStringLiteral("command"));
  srvPort = cfg.value(QStringLiteral("RESTServerPort"), 5432).toInt();
  user    = cfg.value(QStringLiteral("RESTUser"), QStringLiteral("unknown")).toString();
  pw      = cfg.value(QStringLiteral("RESTAuth"), QStringLiteral("not set")).toString();
  cfg.endGroup();
  }


void save(const QString& an, int mcPort, int mcDelay, int srvPort, QString user, QString pw) {
  QSettings cfg(QSettings::SystemScope, an);

  cfg.beginGroup(QStringLiteral("status"));
  cfg.setValue(QStringLiteral("MulticastPort"), mcPort);
  cfg.setValue(QStringLiteral("MulticastDelay"), mcDelay);
  cfg.endGroup();

  cfg.beginGroup(QStringLiteral("command"));
  cfg.setValue(QStringLiteral("RESTServerPort"), srvPort);
  cfg.setValue(QStringLiteral("RESTUser"), user);
  cfg.setValue(QStringLiteral("RESTAuth"), pw);
  cfg.endGroup();
  cfg.sync();
  }


int main(int argc, char *argv[]) {
  int rv = -1;

  try {
      QApplication  a(argc, argv);
      Authenticator auth;
      QString user, password;
      int     multicastPort  = 4321;
      int     multicastDelay = 100;
      int     serverPort     = 5432;

      QCoreApplication::setOrganizationName(QStringLiteral("SRD"));
      QCoreApplication::setOrganizationDomain(QStringLiteral("schwarzrot.de"));
      QCoreApplication::setApplicationName(QStringLiteral("LCGateway"));
      restore(a.applicationName(), multicastPort, multicastDelay, serverPort, user, password);
      processArgs(a.arguments(), multicastPort, multicastDelay, serverPort, user, password);
      save(a.applicationName(), multicastPort, multicastDelay, serverPort, user, password);
      if (password != "not set")
         auth.setup(QStringLiteral("/linuxcnc/command"), user, password);
      Gateway gw(auth, multicastPort, serverPort, multicastDelay);

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
