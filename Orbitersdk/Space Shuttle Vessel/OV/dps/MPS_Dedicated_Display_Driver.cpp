/******* SSV File Modification Notice *******
Date         Developer
2020/04/01   GLS
2020/04/07   GLS
2020/04/28   GLS
2020/06/20   GLS
2021/01/20   GLS
2021/08/23   GLS
2021/08/24   GLS
2022/05/29   GLS
2022/08/05   GLS
********************************************/
#include "MPS_Dedicated_Display_Driver.h"
#include "SSME_SOP.h"
#include "ETSepSequence.h"
#include <cassert>


namespace dps
{
	MPS_Dedicated_Display_Driver::MPS_Dedicated_Display_Driver( SimpleGPCSystem *_gpc ):SimpleGPCSoftware( _gpc, "MPS_Dedicated_Display_Driver" )
	{
		return;
	}

	MPS_Dedicated_Display_Driver::~MPS_Dedicated_Display_Driver( void )
	{
		return;
	}

	void MPS_Dedicated_Display_Driver::OnPostStep( double simt, double simdt, double mjd )
	{
		if (pETSepSequence->GetETSEPCommandFlag())
		{
			// all lights off
			AmberStatusLight[0] = false;
			AmberStatusLight[1] = false;
			AmberStatusLight[2] = false;

			RedStatusLight[0] = false;
			RedStatusLight[1] = false;
			RedStatusLight[2] = false;
		}
		else
		{
			for (int i = 1; i <= 3; i++)// red lights
			{
				if ((pSSME_SOP->GetLimitExceededFlag( i ) == true) ||
					(pSSME_SOP->GetShutdownPhaseFlag( i ) == true) ||
					(pSSME_SOP->GetPostShutdownPhaseFlag( i ) == true))
				{
					// red light on
					RedStatusLight[i - 1] = true;
				}
				else
				{
					// red light off
					RedStatusLight[i - 1] = false;
				}
			}

			for (int i = 1; i <= 3; i++)// amber lights
			{
				if ((pSSME_SOP->GetElectricalLockupModeFlag( i ) == true) ||
					(pSSME_SOP->GetHydraulicLockupModeFlag( i ) == true) ||
					(pSSME_SOP->GetFlightDataPathFailureFlag( i ) == true) ||
					(pSSME_SOP->GetCommandPathFailureFlag( i ) == true))
				{
					// amber light on
					AmberStatusLight[i - 1] = true;
				}
				else
				{
					// amber light off
					AmberStatusLight[i - 1] = false;
				}
			}
		}

		if (AmberStatusLight[0]) WriteCOMPOOL_IS( SCP_FF1_IOM2_CH1_DATA, ReadCOMPOOL_IS( SCP_FF1_IOM2_CH1_DATA ) | 0x0001 );
		if (AmberStatusLight[1]) WriteCOMPOOL_IS( SCP_FF2_IOM2_CH1_DATA, ReadCOMPOOL_IS( SCP_FF2_IOM2_CH1_DATA ) | 0x0001 );
		if (AmberStatusLight[2]) WriteCOMPOOL_IS( SCP_FF3_IOM2_CH1_DATA, ReadCOMPOOL_IS( SCP_FF3_IOM2_CH1_DATA ) | 0x0001 );
		if (RedStatusLight[0]) WriteCOMPOOL_IS( SCP_FF1_IOM10_CH1_DATA, ReadCOMPOOL_IS( SCP_FF1_IOM10_CH1_DATA ) | 0x0001 );
		if (RedStatusLight[1]) WriteCOMPOOL_IS( SCP_FF2_IOM10_CH1_DATA, ReadCOMPOOL_IS( SCP_FF2_IOM10_CH1_DATA ) | 0x0001 );
		if (RedStatusLight[2]) WriteCOMPOOL_IS( SCP_FF3_IOM10_CH1_DATA, ReadCOMPOOL_IS( SCP_FF3_IOM10_CH1_DATA ) | 0x0001 );
		return;
	}

	void MPS_Dedicated_Display_Driver::Realize( void )
	{
		pSSME_SOP = dynamic_cast<SSME_SOP*> (FindSoftware( "SSME_SOP" ));
		assert( (pSSME_SOP != NULL) && "MPS_Dedicated_Display_Driver::Realize.pSSME_SOP" );

		pETSepSequence = dynamic_cast<ETSepSequence*> (FindSoftware( "ETSepSequence" ));
		assert( (pETSepSequence != NULL) && "MPS_Dedicated_Display_Driver::Realize.pETSepSequence" );
		return;
	}

	bool MPS_Dedicated_Display_Driver::OnMajorModeChange( unsigned int newMajorMode )
	{
		switch (newMajorMode)
		{
			case 101:
			case 102:
			case 103:
			case 104:
			case 601:
				return true;
			default:
				return false;
		}
	}
}