/* 
 * **************************************************************************
 * 
 *  file:       statusreader.cpp
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
#include "statusreader.h"
#include "errorreader.h"
#include "redstatus.h"
#include "sysevent.h"
#include <stat_msg.hh>
#include <cmd_msg.hh>
#include <emc_nml.hh>
#include <config.h>


StatusNmlReader::StatusNmlReader()
  : cStatus(nullptr)
  , lcStat(nullptr)
  , errReader(nullptr) {
  cStatus = new RCS_STAT_CHANNEL(emcFormat, "emcStatus", "xemc", EMC2_DEFAULT_NMLFILE);
  if (!cStatus || !cStatus->valid()) {
     delete cStatus;
     cStatus = nullptr;
     throw std::system_error(-2, std::system_category(), "ERROR creating status Channel");
     }
  else {
     lcStat = static_cast<EMC_STAT*>(cStatus->get_address());
     errReader = new ErrorReader();
     }
  }


void StatusNmlReader::read() {
  errReader->check4Error();
  cStatus->peek();  // get a copy :(
  }
