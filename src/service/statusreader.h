/* 
 * **************************************************************************
 * 
 *  file:       statusreader.h
 *  project:    LCGateway
 *  subproject: main application
 *  purpose:    network gateway to linuxcnc nml interface         
 *  created:    9.10.2022 by Django Reinhard
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
#ifndef STATUSREADER_H
#define STATUSREADER_H
class RCS_STAT_CHANNEL;
class EMC_STAT;
class ErrorReader;


class StatusNmlReader
{
public:
  StatusNmlReader();

  void read();
  const EMC_STAT* status() const { return lcStat; }

private:
  RCS_STAT_CHANNEL* cStatus;
  EMC_STAT*         lcStat;
  ErrorReader*      errReader;
  };
#endif // STATUSREADER_H
