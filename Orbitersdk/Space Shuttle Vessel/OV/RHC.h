/****************************************************************************
  This file is part of Space Shuttle Vessel

  Rotational Hand Controller subsystem definition


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
2020/05/10   GLS
2020/06/20   GLS
2021/06/28   GLS
2021/07/03   GLS
2021/08/23   GLS
2021/08/24   GLS
2021/09/20   GLS
2021/12/27   GLS
2021/12/30   GLS
2022/08/05   GLS
********************************************/
/****************************************************************************
  This file is part of Space Shuttle Ultra

  Rotational Hand Controller definition



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
#ifndef __RHC_H_
#define __RHC_H_


#include "AtlantisSubsystem.h"
#include <discsignals.h>


using namespace discsignals;


class RHC:public AtlantisSubsystem
{
	friend class Atlantis;

	private:
		unsigned short ID;

		DiscInPort dipPower;

		DiscOutPort dopRHC[9];// pitch A, pitch B, pitch C, roll A, roll B, roll C, yaw A, yaw B, yaw C
		DiscOutPort dopRHCTrim[8];// pitch + A, pitch + B, pitch - A, pitch - B, roll + A, roll + B, roll - A, roll - B

	public:
		RHC( AtlantisSubsystemDirector* _director, const string& _ident, unsigned short ID );
		virtual ~RHC( void );

		void Realize( void ) override;
		void OnPreStep( double simt, double simdt, double mjd ) override;
};


#endif// __RHC_H_