/****************************************************************************
  This file is part of Space Shuttle Vessel

  Transition Digital Auto Pilot definition


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
2020/05/08   GLS
2020/06/20   GLS
2021/07/03   GLS
2021/08/23   GLS
2021/08/24   GLS
2021/12/30   GLS
********************************************/
/****************************************************************************
  This file is part of Space Shuttle Ultra

  Transition Digital Auto Pilot definition



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
#ifndef _dps_TRANS_DAP_H_
#define _dps_TRANS_DAP_H_


#include "SimpleGPCSoftware.h"
#include <discsignals.h>


using namespace discsignals;


namespace dps
{
	class SSME_Operations;

	class TransitionDAP:public SimpleGPCSoftware
	{
		private:
			SSME_Operations* pSSME_Operations;

			DiscOutPort ZTransCommand;
			DiscOutPort RotThrusterCommands[3];

			bool MinusZTranslation;

			double ETSepMinusZDV;
			double ETSepMinusZDT;
		public:
			explicit TransitionDAP( SimpleGPCSystem* _gpc );
			~TransitionDAP( void );

			void OnPostStep( double simt, double simdt, double mjd ) override;

			void Realize( void ) override;

			bool OnMajorModeChange( unsigned int newMajorMode ) override;

			void MinusZTranslationCommand();

			void NullRates( double simdt );
	};
}


#endif// _dps_TRANS_DAP_H_