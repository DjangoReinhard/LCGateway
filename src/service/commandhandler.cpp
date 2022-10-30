/* 
 * **************************************************************************
 * 
 *  file:       commandhandler.cpp
 *  project:    LCGateway
 *  subproject: main application
 *  purpose:    network gateway to linuxcnc nml interface         
 *  created:    25.10.2022 by Django Reinhard
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
#include <QDebug>
#include <QThread>
#include <commandhandler.h>
#include <httprequest.h>
#include <stat_msg.hh>
#include <cmd_msg.hh>
#include <emc_nml.hh>
#include <config.h>


CommandHandler::CommandHandler(QObject *parent)
 : AbstractService(parent) {
  cCmd = new RCS_CMD_CHANNEL(emcFormat, "emcCommand", "xemc", EMC2_DEFAULT_NMLFILE);
  if (!cCmd || !cCmd->valid()) {
     delete cCmd;
     cCmd = NULL;
     }
  cStat = new RCS_STAT_CHANNEL(emcFormat, "emcStatus", "xemc", EMC2_DEFAULT_NMLFILE);
  if (!cStat || !cStat->valid()) {
     delete cCmd;
     delete cStat;
     cCmd  = NULL;
     cStat = NULL;
     }
  else {
     status = static_cast<EMC_STAT*>(cStat->get_address());
     }
  }


bool CommandHandler::isRestricted() const {
  return true;
  }


QString CommandHandler::requiredRequestType() const {
  return QStringLiteral("POST");
  }


// command(L: 24 ) to send is:  "f90100000000000018000000000000000000000002000000"
// received command(L: 24 ) is: "7006d1d8707f0000f9b2b065c05500006007d1d8707f0000"
//
void CommandHandler::service(const HttpRequest& r, QTcpSocket&) {
  QString test(r.toByteArray());

  qDebug() << "\t>>> request ===============";
  qDebug() << test;
  qDebug() << "\t<<< request ---------------";

//  QByteArray   buf(QByteArray::fromBase64(r.payload(), QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));
  QByteArray   buf(QByteArray::fromBase64(r.payload()));
  RCS_CMD_MSG* msg = (RCS_CMD_MSG*)(buf.data());
  test = QString(buf.toHex());

  qDebug() << "received command(L:" << buf.size() << ") is:" << test;
  if (!cCmd->write(msg)) {
     double end = 0;

     qDebug() << "command written";
     while (end < EMC_COMMAND_TIMEOUT) {
           cStat->peek();
           end += EMC_COMMAND_DELAY;

//           qDebug() << status->echo_serial_number << "\t<==>\t" << msg->serial_number;

           if ((status->echo_serial_number - msg->serial_number) >= 0) return;
           QThread::msleep(10);
           }
     }
  else {
     qDebug() << "failed to write command!";
     }
  }


void CommandHandler::sleep(double seconds) {
  struct timeval tv;

  tv.tv_sec  = seconds;
  tv.tv_usec = seconds * 1000000;
  select(0, NULL, NULL, NULL, &tv);
  }


const int    CommandHandler::EMC_COMMAND_TIMEOUT = 5.0;  // how long to wait until timeout
const double CommandHandler::EMC_COMMAND_DELAY   = 0.01; // seconds to sleep between checks
