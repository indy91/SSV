/****************************************************************************
  This file is part of Space Shuttle Vessel

  Main Propulsion System Ascent Thrust Vector Control Command Subsystem
  Operating Program definition


  Space Shuttle Vessel is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Vessel is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Vessel; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html or
  file SSV-LICENSE.txt for more details.

  **************************************************************************/
/******* SSV File Modification Notice *******
Date         Developer
2020/04/01   GLS
2020/04/07   GLS
2020/05/08   GLS
2020/05/10   GLS
2020/06/20   GLS
2020/07/11   GLS
2021/07/03   GLS
2021/08/23   GLS
2021/08/24   GLS
2021/10/23   GLS
2021/12/30   GLS
2022/08/05   GLS
********************************************/
/****************************************************************************
  This file is part of Space Shuttle Ultra

  Ascent Thrust Vector Control Subsystem Operating Program definition



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html or
  file Doc\Space Shuttle Ultra\GPL.txt for more details.

  **************************************************************************/
#ifndef _dps_MPS_ATVC_CMD_SOP_H_
#define _dps_MPS_ATVC_CMD_SOP_H_


#include "SimpleGPCSoftware.h"
#include <discsignals.h>


inline constexpr double STARTCONFIG_1P = 0.0;// [deg]
inline constexpr double STARTCONFIG_2P = 0.0;// [deg]
inline constexpr double STARTCONFIG_3P = 0.0;// [deg]
inline constexpr double STARTCONFIG_1Y = 0.0;// [deg]
inline constexpr double STARTCONFIG_2Y = 0.0;// [deg]
inline constexpr double STARTCONFIG_3Y = 0.0;// [deg]

inline constexpr double LAUNCHCONFIG_1P = 0.0;// [deg]
inline constexpr double LAUNCHCONFIG_2P = 0.0;// [deg]
inline constexpr double LAUNCHCONFIG_3P = 0.0;// [deg]
inline constexpr double LAUNCHCONFIG_1Y = 0.0;// [deg]
inline constexpr double LAUNCHCONFIG_2Y = -3.5;// [deg]
inline constexpr double LAUNCHCONFIG_3Y = 3.5;// [deg]

inline constexpr double MPSDUMPCONFIG_1P = 0.0;// [deg]
inline constexpr double MPSDUMPCONFIG_2P = 10.0;// [deg]
inline constexpr double MPSDUMPCONFIG_3P = 10.0;// [deg]
inline constexpr double MPSDUMPCONFIG_1Y = 0.0;// [deg]
inline constexpr double MPSDUMPCONFIG_2Y = -3.5;// [deg]
inline constexpr double MPSDUMPCONFIG_3Y = 3.5;// [deg]

inline constexpr double ENTRYSTOWCONFIG_1P = 0.0;// [deg]
inline constexpr double ENTRYSTOWCONFIG_2P = 10.0;// [deg]
inline constexpr double ENTRYSTOWCONFIG_3P = 10.0;// [deg]
inline constexpr double ENTRYSTOWCONFIG_1Y = 0.0;// [deg]
inline constexpr double ENTRYSTOWCONFIG_2Y = -3.5;// [deg]
inline constexpr double ENTRYSTOWCONFIG_3Y = 3.5;// [deg]

inline constexpr double ENTRYSTOWCHUTECONFIG_1P = -10.0;// [deg]
inline constexpr double ENTRYSTOWCHUTECONFIG_2P = 0.0;// [deg]
inline constexpr double ENTRYSTOWCHUTECONFIG_3P = 0.0;// [deg]
inline constexpr double ENTRYSTOWCHUTECONFIG_1Y = 0.0;// [deg]
inline constexpr double ENTRYSTOWCHUTECONFIG_2Y = -3.5;// [deg]
inline constexpr double ENTRYSTOWCHUTECONFIG_3Y = 3.5;// [deg]


using namespace discsignals;


namespace dps
{
	class MPS_ATVC_CMD_SOP:public SimpleGPCSoftware
	{
		private:
			DiscOutPort dopPpos[3];
			DiscOutPort dopYpos[3];

			double Ppos[3];
			double Ypos[3];

		public:
			explicit MPS_ATVC_CMD_SOP( SimpleGPCSystem* _gpc );
			~MPS_ATVC_CMD_SOP( void );

			void OnPostStep( double simt, double simdt, double mjd ) override;

			void Realize( void ) override;

			bool OnParseLine( const char* keyword, const char* value ) override;
			void OnSaveState( FILEHANDLE scn ) const override;

			bool OnMajorModeChange( unsigned int newMajorMode ) override;

			void SetSSMEActPos( unsigned int num, double Ppos, double Ypos );
	};
}


#endif// _dps_MPS_ATVC_CMD_SOP_H_
