/* 
 * **************************************************************************
 * 
 *  file:       redstatus.cpp
 *  project:    LCGateway
 *  subproject: main application
 *  purpose:    network gateway to linuxcnc nml interface         
 *  created:    10.10.2022 by Django Reinhard
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
#include "redstatus.h"
#include <stat_msg.hh>
#include <cmd_msg.hh>
#include <emc_nml.hh>
#include <string.h>


RedStatus::RedStatus(const EMC_STAT* status) {
  if (status) *this = status;
  }

RedStatus& RedStatus::operator=(const EMC_STAT* status) {
  eStop         = status->io.aux.estop;
  flood         = status->io.coolant.flood;
  mist          = status->io.coolant.mist;
  atmReady      = status->io.tool.pocketPrepped;
  toolInSpindle = status->io.tool.toolInSpindle;
  j0Enabled     = status->motion.joint[0].enabled;
  j0Homed       = status->motion.joint[0].homed ? true : false;
  j1Enabled     = status->motion.joint[1].enabled;
  j1Homed       = status->motion.joint[1].homed ? true : false;
  j2Enabled     = status->motion.joint[2].enabled;
  j2Homed       = status->motion.joint[2].homed ? true : false;
  j3Enabled     = status->motion.joint[3].enabled;
  j3Homed       = status->motion.joint[3].homed ? true : false;
  j4Enabled     = status->motion.joint[4].enabled;
  j4Homed       = status->motion.joint[4].homed ? true : false;
  j5Enabled     = status->motion.joint[5].enabled;
  j5Homed       = status->motion.joint[5].homed ? true : false;
  j6Enabled     = status->motion.joint[6].enabled;
  j6Homed       = status->motion.joint[6].homed ? true : false;
  j7Enabled     = status->motion.joint[7].enabled;
  j7Homed       = status->motion.joint[7].homed ? true : false;
  j8Enabled     = status->motion.joint[8].enabled;
  j8Homed       = status->motion.joint[8].homed ? true : false;
  sp0Dir        = status->motion.spindle[0].direction;
  sp0Enabled    = status->motion.spindle[0].enabled;
  sp0Homed      = status->motion.spindle[0].homed;
  sp0Increasing = status->motion.spindle[0].increasing;
  sp0OvrEnabled = status->motion.spindle[0].spindle_override_enabled;
  afEnabled     = status->motion.traj.adaptive_feed_enabled;
  ovrEnabled    = status->motion.traj.feed_override_enabled;
  execState     = status->task.execState;
  interpState   = status->task.interpState;
  taskMode      = status->task.mode;
  progUnits     = status->task.programUnits;
  taskState     = status->task.state;
  axisMask      = status->motion.traj.axis_mask;
  motionLine    = status->task.motionLine;
  sp0NomSpeed   = status->motion.spindle[0].speed;
  sp0ActSpeed   = status->motion.spindle[0].speed * status->motion.spindle[0].spindle_scale;
  sp0Scale      = status->motion.spindle[0].spindle_scale;
  nomVelocity   = status->motion.traj.current_vel * 60;
  actVelocity   = status->motion.traj.current_vel * 60 * status->motion.traj.scale;
  maxVelocity   = status->motion.traj.maxVelocity * 60;
  scaledMaxVel  = status->motion.traj.maxVelocity * 60 * status->motion.traj.rapid_scale;
  rapidScale    = status->motion.traj.rapid_scale;
  feedScale     = status->motion.traj.scale;
  xyRotation    = status->task.rotation_xy;
  strncpy(taskFile, status->task.file, 255);
  copyPose(dtg, status->motion.traj.dtg);
  copyPose(trajPosition, status->motion.traj.position);
  copyPose(trajActPos,   status->motion.traj.actualPosition);
  copyPose(G5xOffset,    status->task.g5x_offset);
  copyPose(G92Offset,    status->task.g92_offset);
  copyPose(toolOffset,   status->task.toolOffset);
  memcpy(activeGCodes, status->task.activeGCodes, sizeof(int) * 17);
  memcpy(activeMCodes, status->task.activeMCodes, sizeof(int) * 10);

  return *this;
  }


void RedStatus::copyPose(double *dst, const EmcPose &src) {
  dst[0] = src.tran.x;
  dst[1] = src.tran.y;
  dst[2] = src.tran.z;
  dst[3] = src.a;
  dst[4] = src.b;
  dst[5] = src.c;
  dst[6] = src.u;
  dst[7] = src.v;
  dst[8] = src.w;
  }
