/****************************************************************************
  This file is part of Space Shuttle Vessel

  FILT2 definition


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
2020/08/24   GLS
2021/08/24   GLS
2022/07/16   GLS
2022/08/05   GLS
********************************************/
/****************************************************************************
  This file is part of Space Shuttle Ultra

  FILT2 definition



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
#ifndef _FILT2_H
#define _FILT2_H


/**
* Implementation of a digital IIR second-order filter.
*/
class FILT2
{
	private:
		double G1;
		double G2;
		double G3;
		double G4;
		double G5;
		double LIM_L;
		double LIM_U;

		double Y0[2];
		double X0[2];


	public:
		FILT2( void );
		FILT2( double G1, double G2, double G3, double G4, double G5 );
		FILT2(double LIM_L, double LIM_U );
		FILT2( double G1, double G2, double G3, double G4, double G5, double LIM_L, double LIM_U );
		~FILT2();

		void SaveState( char* line ) const;
		void LoadState( const char* line );

		double GetValue( double Xn );
		void SetGains( double G1, double G2, double G3, double G4, double G5 );
		void SetLimits( double LIM_L, double LIM_U );
		void SetInitialValue( double IC );
};

#endif// _FILT2_H
