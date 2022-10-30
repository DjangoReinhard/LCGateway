/* 
 * **************************************************************************
 * 
 *  file:       redstatus.h
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
#ifndef REDSTATUS_H
#define REDSTATUS_H
class EMC_STAT;
class EmcPose;


struct RedStatus
{
public:
  char      afEnabled;
  char      atmReady;
  char      eStop;
  char      execState;
  char      flood;
  char      interpState;
  char      j0Enabled;
  char      j0Homed;
  char      j1Enabled;
  char      j1Homed;
  char      j2Enabled;
  char      j2Homed;
  char      j3Enabled;
  char      j3Homed;
  char      j4Enabled;
  char      j4Homed;
  char      j5Enabled;
  char      j5Homed;
  char      j6Enabled;
  char      j6Homed;
  char      j7Enabled;
  char      j7Homed;
  char      j8Enabled;
  char      j8Homed;
  char      mist;
  char      ovrEnabled;
  char      progUnits;
  char      sp0Dir;
  char      sp0Enabled;
  char      sp0FOEnabled;
  char      sp0Homed;
  char      sp0Increasing;
  char      sp0OvrEnabled;
  char      taskFile[255];
  char      taskMode;
  char      taskState;
  char      toolInSpindle;
  int       axisMask;
  long      motionLine;
  double    actVelocity;
  double    feedScale;
  double    maxVelocity;
  double    nomVelocity;
  double    rapidScale;
  double    scaledMaxVel;
  double    sp0ActSpeed;
  double    sp0NomSpeed;
  double    sp0Scale;
  double    xyRotation;
  double    G5xOffset[9];
  double    G92Offset[9];
  double    dtg[9];
  double    toolOffset[9];
  double    trajActPos[9];
  double    trajPosition[9];
  int       activeGCodes[17];
  int       activeMCodes[10];

  explicit   RedStatus(const EMC_STAT* status = nullptr);

  RedStatus& operator=(const EMC_STAT* status);

private:
  void copyPose(double* dst, const EmcPose& src);
  };
#endif // REDSTATUS_H
