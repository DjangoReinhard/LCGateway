/* 
 * **************************************************************************
 * 
 *  file:       commandhandler.h
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
#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H
#include <abstractservice.h>
class RCS_CMD_CHANNEL;
class RCS_STAT_CHANNEL;
class RCS_CMD_MSG;
class EMC_STAT;


class CommandHandler : public AbstractService
{
  Q_OBJECT
public:
  explicit CommandHandler(QObject *parent = nullptr);
  virtual ~CommandHandler() = default;

  bool    isRestricted() const override;
  void    service(const HttpRequest& r, QTcpSocket& s) override;
  QString requiredRequestType() const override;

protected:
  void sleep(double seconds);

private:
  RCS_CMD_CHANNEL*  cCmd;
  RCS_STAT_CHANNEL* cStat;
  EMC_STAT*         status;

  static const int    EMC_COMMAND_TIMEOUT;  // how long to wait until timeout
  static const double EMC_COMMAND_DELAY;    // seconds to sleep between checks
  };
#endif // COMMANDHANDLER_H
