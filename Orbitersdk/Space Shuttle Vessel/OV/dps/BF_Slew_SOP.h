/****************************************************************************
  This file is part of Space Shuttle Vessel

  Body Flap Slew Subsystem Operating Program definition


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

#ifndef _dps_BF_SLEW_SOP_H_
#define _dps_BF_SLEW_SOP_H_


#include "SimpleGPCSoftware.h"


namespace dps
{
	using namespace discsignals;

	/**
	 * @brief	Implementation of the Body Flap Slew Subsystem Operating Program software that runs in the GPCs.
	 *
	 * This class generates manual Body Flap drive signals for AerojetDAP.
	 */
	class BF_Slew_SOP:public SimpleGPCSoftware
	{
		public:
			explicit BF_Slew_SOP( SimpleGPCSystem* _gpc );
			~BF_Slew_SOP( void );

			void OnPostStep( double simt, double simdt, double mjd ) override;

			bool OnMajorModeChange( unsigned int newMajorMode ) override;
	};
}

#endif// _dps_BF_SLEW_SOP_H_
