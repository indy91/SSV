/******* SSV File Modification Notice *******
Date         Developer
2020/04/07   GLS
2020/05/08   GLS
2020/05/24   GLS
2020/06/12   GLS
2020/06/20   GLS
2021/01/19   GLS
2021/06/01   GLS
2021/08/24   GLS
2022/03/24   GLS
2022/04/17   GLS
2022/07/17   GLS
2022/07/24   GLS
2022/08/05   GLS
2022/08/17   GLS
2022/09/15   GLS
2022/09/29   GLS
2022/10/02   GLS
2022/10/09   GLS
2022/10/11   GLS
2022/10/12   GLS
2022/10/13   GLS
2022/10/21   GLS
2022/10/25   GLS
********************************************/
#include "IDP.h"
#include "../Atlantis.h"
#include "../vc/MDU.h"
#include "SimpleGPCSystem.h"
#include <MathSSV.h>
#include "ADC.h"
#include "IO_Control.h"
#include "SSME_Operations.h"
#include "AscentDAP.h"
#include "AerojetDAP.h"
#include "Landing_SOP.h"
#include "OMSBurnSoftware.h"
#include "DedicatedDisplay_SOP.h"


namespace dps {

	IDP::IDP( AtlantisSubsystemDirector* _director, const string& _ident, unsigned short _usIDPID )
		: AtlantisSubsystem( _director, _ident ), usIDPID(_usIDPID)
	{
		usSPEC=dps::MODE_UNDEFINED;
		usDISP=dps::MODE_UNDEFINED;
		majfunc=GNC;
		cScratchPadLine[0] = 0;
		syntaxerr = false;
	}

	IDP::~IDP()
	{
	}

	void IDP::Realize()
	{
		pIO_Control = dynamic_cast<IO_Control*> (STS()->pSimpleGPC->FindSoftware( "IO_Control" ));
		assert( (pIO_Control != NULL) && "IDP::Realize.pIO_Control" );
		pSSME_Operations = dynamic_cast<SSME_Operations*> (STS()->pSimpleGPC->FindSoftware( "SSME_Operations" ));
		assert( (pSSME_Operations != NULL) && "IDP::Realize.pSSME_Operations" );
		pAscentDAP = dynamic_cast<AscentDAP*> (STS()->pSimpleGPC->FindSoftware( "AscentDAP" ));
		assert( (pAscentDAP != NULL) && "IDP::Realize.pAscentDAP" );
		pAerojetDAP = dynamic_cast<AerojetDAP*> (STS()->pSimpleGPC->FindSoftware( "AerojetDAP" ));
		assert( (pAerojetDAP != NULL) && "IDP::Realize.pAerojetDAP" );
		pLanding_SOP = dynamic_cast<Landing_SOP*> (STS()->pSimpleGPC->FindSoftware( "Landing_SOP" ));
		assert( (pLanding_SOP != NULL) && "IDP::Realize.pLanding_SOP" );
		pOMSBurnSoftware = dynamic_cast<OMSBurnSoftware*> (STS()->pSimpleGPC->FindSoftware( "OMSBurnSoftware" ));
		assert( (pOMSBurnSoftware != NULL) && "IDP::Realize.pOMSBurnSoftware" );
		pDedicatedDisplay_SOP = dynamic_cast<DedicatedDisplay_SOP*> (STS()->pSimpleGPC->FindSoftware( "DedicatedDisplay_SOP" ));
		assert( (pDedicatedDisplay_SOP != NULL) && "IDP::Realize.pDedicatedDisplay_SOP" );
		pADC1 = dynamic_cast<ADC*>(director->GetSubsystemByName( (usIDPID <= 2) ? "ADC1A" : "ADC1B" ));
		assert( (pADC1 != NULL) && "IDP::Realize.pADC1" );
		pADC2 = dynamic_cast<ADC*>(director->GetSubsystemByName( (usIDPID <= 2) ? "ADC2A" : "ADC2B" ));
		assert( (pADC2 != NULL) && "IDP::Realize.pADC2" );

		DiscreteBundle* pBundle = BundleManager()->CreateBundle( "C2_A12A1_A12A2_IDP", 14 );
		Power.Connect( pBundle, usIDPID );
		MajorFuncPL.Connect( pBundle, usIDPID + 3 );
		MajorFuncGNC.Connect( pBundle, usIDPID + 7 );
		KeybSelectA.Connect( pBundle, 12 );// not used by IDP4
		KeybSelectB.Connect( pBundle, 13 );// not used by IDP4
		return;
	}

	unsigned short IDP::GetIDPID() const {
		return usIDPID;
	}

	unsigned short IDP::GetDisp() const
	{
		return usDISP;
	}

	unsigned short IDP::GetSpec() const
	{
		return usSPEC;
	}

	MAJORFUNCTION IDP::GetMajfunc() const
	{
		return majfunc;
	}

	bool IDP::IsCompleteLine() const
	{
		size_t len = strlen( cScratchPadLine );
		if (len != 0)
		{
			if ((cScratchPadLine[len - 1] == SSV_KEY_EXEC) ||
				(cScratchPadLine[len - 1] == SSV_KEY_PRO) ||
				(cScratchPadLine[len - 1] == SSV_KEY_RESUME) ||
				(cScratchPadLine[len - 1] == SSV_KEY_SYSSUMM) ||
				(cScratchPadLine[len - 1] == SSV_KEY_FAULTSUMM)) return true;
		}
		return false;
	}

	int IDP::GetActiveKeyboard( void ) const
	{
		int kb = 0;

		switch (usIDPID)
		{
			case 1:
				if (KeybSelectA.IsSet()) kb = 1;
				break;
			case 2:
				if (KeybSelectB.IsSet() == false) kb = 2;
				break;
			case 3:
				if (KeybSelectA.IsSet() == false) kb = 1;
				if (KeybSelectB.IsSet()) kb += 2;
				break;
		}
		return kb;
	}

	bool IDP::IsKeyboardSelected( unsigned short usKeyboardID ) const
	{
		switch (usIDPID)
		{
			case 1:
				if ((usKeyboardID == 1) && (KeybSelectA.IsSet())) return true;
				else return false;
			case 2:
				if ((usKeyboardID == 2) && (KeybSelectB.IsSet() == false)) return true;
				else return false;
			case 3:
				if ((usKeyboardID == 1) && (KeybSelectA.IsSet() == false)) return true;
				else if ((usKeyboardID == 2) && (KeybSelectB.IsSet())) return true;
				else return false;
			case 4:
				if (usKeyboardID == 3) return true;
				else return false;
			default:
				return false;
		}
	}

	bool IDP::PutKey(unsigned short usKeyboardID, char cKey)
	{
		if (IsKeyboardSelected( usKeyboardID ) == false) return false;

		switch(cKey) {
			case SSV_KEY_RESUME:
				OnResume();
				ClearScratchPadLine();
				AppendScratchPadLine( cKey );
				break;
			case SSV_KEY_CLEAR:
				OnClear();
				break;
			case SSV_KEY_EXEC:
				if(IsCompleteLine()) ClearScratchPadLine();
				OnExec();
				AppendScratchPadLine(cKey);
				break;
			case SSV_KEY_PRO:
				OnPro();
				AppendScratchPadLine(cKey);
				break;
			case SSV_KEY_ITEM:
			case SSV_KEY_SPEC:
			case SSV_KEY_OPS:
			case SSV_KEY_GPCCRT:
			case SSV_KEY_IORESET:
				if(IsCompleteLine()) ClearScratchPadLine();
				AppendScratchPadLine(cKey);
				break;
			case SSV_KEY_SYSSUMM:
				OnSysSummary();
				ClearScratchPadLine();
				AppendScratchPadLine( cKey );
				break;
			case SSV_KEY_FAULTSUMM:
				OnFaultSummary();
				ClearScratchPadLine();
				AppendScratchPadLine( cKey );
				break;
			case SSV_KEY_MSGRESET:
				OnMsgReset();
				break;
			case SSV_KEY_ACK:
				OnAck();
				break;
			default:
				if(IsCompleteLine()) ClearScratchPadLine();
				AppendScratchPadLine(cKey);
				break;
		}
		return true;
	}

	void IDP::SetDisp(unsigned short disp)
	{
		usDISP=disp;
	}

	void IDP::SetSpec(unsigned short spec)
	{
		usSPEC=spec;
	}

	void IDP::SetMajFunc(MAJORFUNCTION func)
	{
		majfunc=func;
	}

	void IDP::ClearScratchPadLine()
	{
		cScratchPadLine[0] = 0;
		syntaxerr = false;
	}

	void IDP::ConnectToMDU(vc::MDU* pMDU, bool bPrimary)
	{
		if(pMDU) {
			if(bPrimary) {
				pMDU->SetPrimaryIDP(this);
			}
			else pMDU->SetSecondaryIDP(this);
		}
	}

	void IDP::OnSaveState(FILEHANDLE scn) const
	{
		oapiWriteScenario_int( scn, "SPEC", usSPEC );
		oapiWriteScenario_int( scn, "DISP", usDISP );
	}

	bool IDP::OnParseLine( const char* line )
	{
		if (!_strnicmp( line, "SPEC", 4 ) )
		{
			sscanf_s( line + 4, "%hu", &usSPEC );
			return true;
		}
		else if (!_strnicmp( line, "DISP", 4 ) )
		{
			sscanf_s( line + 4, "%hu", &usDISP );
			return true;
		}
		return false;
	}

	void IDP::OnAck( void )
	{
		STS()->pSimpleGPC->AckPressed();
		return;
	}

	void IDP::OnClear() {

		if(IsCompleteLine() && !syntaxerr) {
			ClearScratchPadLine();
		} else {
			syntaxerr = false;
			DelFromScratchPadLine();
		}

	}

	void IDP::OnExec() {
		// check if EXEC was pressed without any ITEM input
		if(cScratchPadLine[0] == 0 || IsCompleteLine()) {
			STS()->pSimpleGPC->ExecPressed(GetSpec());
		}
		else {
			std::string scratchPad=GetScratchPadLineString();
			int ITEM=scratchPad.find("ITEM ");
			int IORESET = scratchPad.find( "I/O RESET" );
			int GPCCRT = scratchPad.find( "GPC/CRT " );

			if (ITEM == 0)
			{
				scratchPad=scratchPad.erase( 0, 5 );

				// parse entry
				vector<pair<int,string>> items;
				string Data = "";
				int item = 0;

				// find item number
				if (scratchPad.length() > 0)// must have something after ITEM
				{
					if ((scratchPad[0] >= '0') && (scratchPad[0] <= '9'))// first must be number
					{
						if (scratchPad.length() > 1)// check for 2-digit item, or data
						{
							if ((scratchPad[1] >= '0') && (scratchPad[1] <= '9'))// second must be number...
							{
								if (scratchPad.length() > 2)// check for data
								{
									if ((scratchPad[2] == '+') || (scratchPad[2] == '-'))
									{
										// 2-digit item, with data
										item = ((scratchPad[0] - 48) * 10) + (scratchPad[1] - 48);
										scratchPad.erase( 0, 2 );
									}
									else syntaxerr = true;
								}
								else
								{
									if ((scratchPad[0] != '0') || (scratchPad[1] != '0'))// can't be both 0
									{
										// 2-digit item, no data
										items.push_back( make_pair( ((scratchPad[0] - 48) * 10) + (scratchPad[1] - 48), "" ) );
									}
									else syntaxerr = true;
								}
							}
							else if ((scratchPad[1] == '+') || (scratchPad[1] == '-'))// ... or delimiter
							{
								// 1-digit item, with data
								item = scratchPad[0] - 48;
								scratchPad.erase( 0, 1 );
							}
							else syntaxerr = true;
						}
						else
						{
							if (scratchPad[0] != '0')// can't be 0
							{
								// 1-digit item, no data
								items.push_back( make_pair( scratchPad[0] - 48, "" ) );
							}
							else syntaxerr = true;
						}
					}
					else syntaxerr = true;
				}
				else syntaxerr = true;

				// find data
				if ((item > 0) && (!syntaxerr))// if 0 the entry is only an item with no data
				{
					bool lastchardelimiter = false;
					for (unsigned int i = 0; i < scratchPad.length(); i++)
					{
						if ((scratchPad[i] == '+') || (scratchPad[i] == '-'))// delimiter
						{
							if (i == 0)// first run
							{
								Data += scratchPad[i];
							}
							else
							{
								if (lastchardelimiter == false)// skip item if delimiter is repeated
								{
									items.push_back( make_pair( item, Data.c_str() ) );
								}
								item++;
								Data = "";
								Data += scratchPad[i];
							}
							lastchardelimiter = true;
						}
						else
						{
							lastchardelimiter = false;
							Data += scratchPad[i];
						}
					}
					if (lastchardelimiter)
					{
						syntaxerr = true;
					}
					else
					{
						items.push_back( make_pair( item, Data.c_str() ) );
					}
				}

				// pass inputs to GPCs if no error
				if (!syntaxerr)
				{
					for (unsigned int k = 0; k < items.size(); k++)
					{
						// HACK pick actual shown display
						STS()->pSimpleGPC->ItemInput( (GetDisp() == dps::MODE_UNDEFINED) ? GetSpec() : GetDisp(), items[k].first, items[k].second.c_str(), usIDPID );
					}
				}
			}
			else if (IORESET == 0)
			{
				scratchPad = scratchPad.erase( 0, 9 );
				if (scratchPad.length() == 0)
				{
					// HACK sent i/o reset
					STS()->pSimpleGPC->IORESET();
				}
				else syntaxerr = true;
			}
			else if (GPCCRT == 0)
			{
				scratchPad = scratchPad.erase( 0, 8 );
				if (scratchPad.length() == 2)
				{
					if ((scratchPad[0] >= '0') && (scratchPad[0] <= '5'))// GPC between 0 and 5
					{
						if ((scratchPad[1] >= '1') && (scratchPad[1] <= '4'))// IDP between 1 and 4
						{
							// TODO
						}
						else syntaxerr = true;
					}
					else syntaxerr = true;
				}
				else syntaxerr = true;
			}
			else syntaxerr = true;
		}
		return;
	}

	void IDP::OnPro()
	{
		std::string scratchPad=GetScratchPadLineString();
		int OPS=scratchPad.find("OPS ");
		int SPEC=scratchPad.find("SPEC ");

		if (OPS == 0)
		{
			scratchPad.erase( 0, 4 );

			if (scratchPad.length() == 3)
			{
				if ((scratchPad[0] >= '0') && (scratchPad[0] <= '9') && (scratchPad[1] >= '0') && (scratchPad[1] <= '9') && (scratchPad[2] >= '0') && (scratchPad[2] <= '9'))
				{
					unsigned int newMM = ((scratchPad[0] - 48) * 100) + ((scratchPad[1] - 48) * 10) + (scratchPad[2] - 48);

					unsigned short oldMM = STS()->pSimpleGPC->GetMajorMode();
					if (STS()->pSimpleGPC->SetMajorModeKB( newMM, usIDPID ))
					{
						// if OPS transition, clear SPEC and DISP displays
						// HACK only clears the displays on this IDP
						if ((int)(newMM / 100) != (int)(oldMM / 100))
						{
							SetSpec( dps::MODE_UNDEFINED );
							SetDisp( dps::MODE_UNDEFINED );
						}
					}
				}
				else syntaxerr = true;
			}
			else syntaxerr = true;
		}
		else if (SPEC == 0)
		{
			scratchPad.erase( 0, 5 );

			int newSpec;
			if (scratchPad.length() > 0)
			{
				if ((scratchPad[0] >= '0') && (scratchPad[0] <= '9'))
				{
					newSpec = scratchPad[0] - 48;
					if (scratchPad.length() > 1)
					{
						if ((scratchPad[1] >= '0') && (scratchPad[1] <= '9'))
						{
							newSpec = (newSpec * 10) + scratchPad[1] - 48;
							if (scratchPad.length() > 2)
							{
								if ((scratchPad[2] >= '0') && (scratchPad[2] <= '9'))
								{
									newSpec = (newSpec * 10) + scratchPad[2] - 48;
									if (scratchPad.length() > 3) syntaxerr = true;
								}
								else syntaxerr = true;
							}
						}
						else syntaxerr = true;
					}
				}
				else syntaxerr = true;
			}
			else syntaxerr = true;

			if (!syntaxerr)
			{
				// HACK, this should be set in GPC
				unsigned short tmp = STS()->pSimpleGPC->SetSPECDISP( newSpec, usIDPID );
				if (tmp == 1)
				{
					SetSpec( static_cast<unsigned short>(newSpec) );
					SetDisp( dps::MODE_UNDEFINED );
				}
				else if (tmp == 2)
				{
					SetDisp( static_cast<unsigned short>(newSpec) );
				}
			}
		}
		else syntaxerr = true;

		return;
	}

	void IDP::OnResume()
	{
		if(GetDisp() != dps::MODE_UNDEFINED) {
			SetDisp(dps::MODE_UNDEFINED);
		}
		else if(GetSpec() != dps::MODE_UNDEFINED) {
			SetSpec(dps::MODE_UNDEFINED);
		}
	}

	void IDP::OnFaultSummary( void )
	{
		SetDisp( 99 );
		return;
	}


	void IDP::OnMMChange(unsigned short usNewMM) {
		ClearScratchPadLine();
	}

	bool IDP::OnPaint(vc::MDU* pMDU) {

		//Clear text buffer, if needed

		//delegate painting to software

		if(GetDisp() != dps::MODE_UNDEFINED)
			return STS()->pSimpleGPC->OnPaint(GetDisp(), pMDU);
		else
			return STS()->pSimpleGPC->OnPaint(GetSpec(), pMDU);
	}

	void IDP::OnSysSummary()
	{
		// TODO check here if DISP valid in current OPS?
		if (GetMajfunc() == dps::GNC)
		{
			if (GetDisp() == 18) SetDisp( 19 );
			else SetDisp( 18 );
		}
		else if (GetMajfunc() == dps::SM)
		{
			if (GetDisp() == 78) SetDisp( 79 );
			else SetDisp( 78 );
		}
		return;
	}

	void IDP::OnMsgReset( void )
	{
		STS()->pSimpleGPC->MsgResetPressed( usIDPID );
		return;
	}


	const char* IDP::GetScratchPadLineScan() const {
		return cScratchPadLine;
	}

	void IDP::AppendScratchPadLine( char cKey )
	{
		size_t len = strlen( cScratchPadLine );
		if (len < 63)
		{
			cScratchPadLine[len] = cKey;
			cScratchPadLine[len + 1] = 0;
		}
		return;
	}

	void IDP::DelFromScratchPadLine()
	{
		size_t len = strlen( cScratchPadLine );
		if (len != 0) cScratchPadLine[len - 1] = 0;
		return;
	}

	const char* IDP::GetScratchPadLineString() const
	{
		static char pszBuffer[250];// TODO fix this hole!
		unsigned short i = 0;
		size_t len = strlen( cScratchPadLine );
		pszBuffer[0] = 0;

		while (i < len)
		{
			switch (cScratchPadLine[i])
			{
				case SSV_KEY_ITEM:
					strcat_s(pszBuffer, "ITEM ");
					break;
				case SSV_KEY_GPCCRT:
					strcat_s(pszBuffer, "GPC/CRT ");
					break;
				case SSV_KEY_OPS:
					strcat_s(pszBuffer, "OPS ");
					break;
				case SSV_KEY_SPEC:
					strcat_s(pszBuffer, "SPEC ");
					break;
				case SSV_KEY_A:
					strcat_s(pszBuffer, "A");
					break;
				case SSV_KEY_B:
					strcat_s(pszBuffer, "B");
					break;
				case SSV_KEY_C:
					strcat_s(pszBuffer, "C");
					break;
				case SSV_KEY_D:
					strcat_s(pszBuffer, "D");
					break;
				case SSV_KEY_E:
					strcat_s(pszBuffer, "E");
					break;
				case SSV_KEY_F:
					strcat_s(pszBuffer, "F");
					break;
				case SSV_KEY_1:
					strcat_s(pszBuffer, "1");
					break;
				case SSV_KEY_2:
					strcat_s(pszBuffer, "2");
					break;
				case SSV_KEY_3:
					strcat_s(pszBuffer, "3");
					break;
				case SSV_KEY_4:
					strcat_s(pszBuffer, "4");
					break;
				case SSV_KEY_5:
					strcat_s(pszBuffer, "5");
					break;
				case SSV_KEY_6:
					strcat_s(pszBuffer, "6");
					break;
				case SSV_KEY_7:
					strcat_s(pszBuffer, "7");
					break;
				case SSV_KEY_8:
					strcat_s(pszBuffer, "8");
					break;
				case SSV_KEY_9:
					strcat_s(pszBuffer, "9");
					break;
				case SSV_KEY_0:
					strcat_s(pszBuffer, "0");
					break;
				case SSV_KEY_DOT:
					strcat_s(pszBuffer, ".");
					break;
				case SSV_KEY_PLUS:
					strcat_s(pszBuffer, "+");
					break;
				case SSV_KEY_MINUS:
					strcat_s(pszBuffer, "-");
					break;
				case SSV_KEY_EXEC:
					strcat_s(pszBuffer, " EXEC");
					break;
				case SSV_KEY_PRO:
					strcat_s(pszBuffer, " PRO");
					break;
				case SSV_KEY_IORESET:
					strcat_s(pszBuffer, "I/O RESET");
					break;
				case SSV_KEY_RESUME:
					strcat_s( pszBuffer, "RESUME" );
					break;
				case SSV_KEY_SYSSUMM:
					strcat_s( pszBuffer, "SYS SUMM" );
					break;
				case SSV_KEY_FAULTSUMM:
					strcat_s( pszBuffer, "FAULT SUMM" );
					break;
				default:
					break;
			}
			i++;
		}

		return pszBuffer;
	}

	const char* IDP::GetScratchPadLineString_B( void ) const
	{
		const char* cbufin = GetScratchPadLineString();
		static char cbufout[250];
		int len = strlen( cbufin );
		int j = 0;
		bool firstsign = false;
		int item = 0;

		for (int i = 0; i <= len; i++)
		{
			if ((cbufin[i] == '+') || (cbufin[i] == '-'))
			{
				if (firstsign == false)
				{// first item number behind
					firstsign = true;

					if ((cbufout[j - 1] >= '0') && (cbufout[j - 1] <= '9'))
					{
						if ((cbufout[j - 2] >= '0') && (cbufout[j - 2] <= '9'))
						{
							// two-digit
							item = (cbufout[j - 1] - 48) + ((cbufout[j - 2] - 48) * 10);

							sprintf_s( cbufout + j - 2, 32, "(%d)", item );
							j += 2;
						}
						else
						{
							// one-digit
							item = cbufout[j - 1] - 48;

							sprintf_s( cbufout + j - 1, 32, "(0%d)", item );
							j += 3;
						}
					}
				}
				else
				{// data behind... or sign

					if ((cbufin[i - 1] == '+') || (cbufin[i - 1] == '-'))
					{
						//_(24)+
						item++;
						sprintf_s( cbufout + j - 4, 32, "%02d)", item );
						j -= 1;
					}
					else
					{
						item++;
						sprintf_s( cbufout + j, 32, " (%02d)", item );
						j += 5;
					}
				}
			}
			cbufout[j++] = cbufin[i];
		}

		return cbufout;
	}

	void IDP::PrintScratchPadLine( vc::MDU* pMDU ) const
	{
		const char* str = GetScratchPadLineString_B();

		int flashingentry = GetFlashingEntry();
		if (flashingentry != 0)
		{
			char str2[16];
			strncpy_s( str2, str, flashingentry );
			pMDU->mvprint( 1, 25, str2, dps::DEUATT_FLASHING );
		}
		pMDU->mvprint( 1 + flashingentry, 25, str + flashingentry );

		// TODO check for line overflow
		if (syntaxerr) pMDU->mvprint( static_cast<unsigned short>(strlen( str )) + 1, 25, " ERR", dps::DEUATT_FLASHING );
		return;
	}

	int IDP::GetFlashingEntry( void ) const
	{
		if (IsCompleteLine()) return 0;

		switch (cScratchPadLine[0])
		{
			case SSV_KEY_OPS:
				return 3;
			case SSV_KEY_SPEC:
			case SSV_KEY_ITEM:
				return 4;
			case SSV_KEY_GPCCRT:
				return 7;
			case SSV_KEY_IORESET:
				return 9;
			default:
				return 0;
		}
	}

	void IDP::PrintFaultMessageLine( vc::MDU* pMDU ) const
	{
		// get from GPC
		bool flash = false;
		char cFaultMessageLine[64];
		memset( cFaultMessageLine, 0, 64 );
		STS()->pSimpleGPC->GetFaultMsg( cFaultMessageLine, flash, usIDPID );

		if (cFaultMessageLine[0]) pMDU->mvprint( 0, 24, cFaultMessageLine, flash ? dps::DEUATT_FLASHING : dps::DEUATT_NORMAL );
		return;
	}

	int IDP::GetADIAttitude( void )
	{
		switch (usIDPID)
		{
			case 1:
				return pIO_Control->GetSWPos( SW_ADI_ATTITUDE_F6 );
			case 2:
				return pIO_Control->GetSWPos( SW_ADI_ATTITUDE_F8 );
			case 4:
				return pIO_Control->GetSWPos( SW_ADI_ATTITUDE_A6U );
			default:
				return 1;// switch in LVLH
		}
	}

	int IDP::GetADIError( void )
	{
		switch (usIDPID)
		{
			case 1:
				return pIO_Control->GetSWPos( SW_ADI_ERROR_F6 );
			case 2:
				return pIO_Control->GetSWPos( SW_ADI_ERROR_F8 );
			case 4:
				return pIO_Control->GetSWPos( SW_ADI_ERROR_A6U );
			default:
				return 1;// switch in MED
		}
	}

	int IDP::GetADIRate( void )
	{
		switch (usIDPID)
		{
			case 1:
				return pIO_Control->GetSWPos( SW_ADI_RATE_F6 );
			case 2:
				return pIO_Control->GetSWPos( SW_ADI_RATE_F8 );
			case 4:
				return pIO_Control->GetSWPos( SW_ADI_RATE_A6U );
			default:
				return 1;// switch in MED
		}
	}

	bool IDP::GetMECOConfirmedFlag( void ) const
	{
		return pSSME_Operations->GetMECOConfirmedFlag();
	}

	bool IDP::GetAutoThrottleState( void ) const
	{
		return pAscentDAP->GetAutoThrottleState();
	}

	VECTOR3 IDP::GetAttitudeErrors_AscentDAP( void ) const
	{
		return pAscentDAP->GetAttitudeErrors();
	}

	VECTOR3 IDP::GetAttitudeErrors_AerojetDAP( void ) const
	{
		return pAerojetDAP->GetAttitudeErrors();
	}

	VECTOR3 IDP::GetRates( void ) const
	{
		return pAerojetDAP->GetRates();
	}

	VECTOR3 IDP::GetAttitudeCommandErrors( void ) const
	{
		return pOMSBurnSoftware->GetAttitudeCommandErrors();
	}

	bool IDP::GetAutoPitchState( void ) const
	{
		return pAerojetDAP->GetAutoPitchState();
	}

	bool IDP::GetAutoRollYawState( void ) const
	{
		return pAerojetDAP->GetAutoRollYawState();
	}

	bool IDP::GetAutoSpeedbrakeState( void ) const
	{
		return pAerojetDAP->GetAutoSpeedbrakeState();
	}

	bool IDP::GetAerosurfacePositions( double& LOB, double& LIB, double& RIB, double& ROB, double& Aileron, double& Rudder, double& BodyFlap, double& SpeedBrake_Pos, double& SpeedBrake_Cmd ) const
	{
		switch (STS()->GetGPCMajorMode())
		{
			case 304:
			case 305:
			case 602:
			case 603:
			case 801:
			case 901:
				break;
			default:
				return false;
		}

		unsigned short data = pADC1->GetData( 1 );// body flap
		BodyFlap = (100.0 * data) / 2048;

		data = pADC1->GetData( 2 );// aileron
		Aileron = ((10.0 * data) / 2048) - 5.0;

		data = pADC1->GetData( 3 );// lib
		LIB = ((55.0 * data) / 2048) - 35.0;

		data = pADC1->GetData( 4 );// lob
		LOB = ((55.0 * data) / 2048) - 35.0;

		data = pADC1->GetData( 5 );// rib
		RIB = ((55.0 * data) / 2048) - 35.0;

		data = pADC1->GetData( 6 );// rob
		ROB = ((55.0 * data) / 2048) - 35.0;

		data = pADC1->GetData( 7 );// spd bk pos
		SpeedBrake_Pos = (100.0 * data) / 2048;

		data = pADC1->GetData( 8 );// rudder
		Rudder = ((60.0 * data) / 2048) - 30.0;

		data = pADC1->GetData( 10 );// spd bk cmd
		SpeedBrake_Cmd = (100.0 * data) / 2048;
		return true;
	}

	bool IDP::GetOMSdata( unsigned short& PC_L, unsigned short& PC_R, unsigned short& He_L, unsigned short& He_R, unsigned short& N2_L, unsigned short& N2_R ) const
	{
		unsigned short data = pADC1->GetData( 24 );// he left
		He_L = Round( (5000.0 * data) / 2048 );

		data = pADC1->GetData( 25 );// n2 left
		N2_L = Round( (3000.0 * data) / 2048 );

		data = pADC1->GetData( 26 );// pc left
		PC_L = Round( (160.0 * data) / 2048 );

		data = pADC1->GetData( 28 );// he right
		He_R = Round( (5000.0 * data) / 2048 );

		data = pADC1->GetData( 29 );// n2 right
		N2_R = Round( (3000.0 * data) / 2048 );

		data = pADC1->GetData( 30 );// pc right
		PC_R = Round( (160.0 * data) / 2048 );
		return true;
	}

	bool IDP::GetMPSdata( unsigned short& PC_C, unsigned short& PC_L, unsigned short& PC_R, unsigned short& HeTk_C, unsigned short& HeTk_L, unsigned short& HeTk_R, unsigned short& HeTk_Pneu, unsigned short& HeReg_C, unsigned short& HeReg_L, unsigned short& HeReg_R, unsigned short& HeReg_Pneu, unsigned short& LH2_Manif, unsigned short& LO2_Manif ) const
	{
		unsigned short data = pADC1->GetData( 11 );// pc center
		PC_C = Round( (115.0 * data) / 2048 );

		data = pADC1->GetData( 12 );// pc left
		PC_L = Round( (115.0 * data) / 2048 );

		data = pADC1->GetData( 13 );// pc right
		PC_R = Round( (115.0 * data) / 2048 );

		data = pADC1->GetData( 14 );// lh2 manif
		LH2_Manif = Round( (100.0 * data) / 2048 );

		data = pADC1->GetData( 15 );// he tk left
		HeTk_L = Round( (5000.0 * data) / 2048 );

		data = pADC1->GetData( 16 );// he reg left
		HeReg_L = Round( (1000.0 * data) / 2048 );

		data = pADC1->GetData( 17 );// he tk pneu
		HeTk_Pneu = Round( (5000.0 * data) / 2048 );

		data = pADC1->GetData( 18 );// he reg pneu
		HeReg_Pneu = Round( (1000.0 * data) / 2048 );

		data = pADC1->GetData( 19 );// lo2 manif
		LO2_Manif = Round( (300.0 * data) / 2048 );

		data = pADC1->GetData( 20 );// he tk center
		HeTk_C = Round( (5000.0 * data) / 2048 );

		data = pADC1->GetData( 21 );// he reg left
		HeReg_C = Round( (1000.0 * data) / 2048 );

		data = pADC1->GetData( 22 );// he tk right
		HeTk_R = Round( (5000.0 * data) / 2048 );

		data = pADC1->GetData( 23 );// he reg left
		HeReg_R = Round( (1000.0 * data) / 2048 );
		return true;
	}

	bool IDP::GetAPUdata( unsigned short& FuQty_1, unsigned short& FuQty_2, unsigned short& FuQty_3, unsigned short& Fu_Press_1, unsigned short& Fu_Press_2, unsigned short& Fu_Press_3, unsigned short& H2OQty_1, unsigned short& H2OQty_2, unsigned short& H2OQty_3, unsigned short& OilIn_1, unsigned short& OilIn_2, unsigned short& OilIn_3 ) const
	{
		unsigned short data = pADC2->GetData( 1 );// fu qty 1
		FuQty_1 = Round( (100.0 * data) / 2048 );

		data = pADC2->GetData( 2 );// h2o qty 1
		H2OQty_1 = Round( (100.0 * data) / 2048 );

		data = pADC2->GetData( 3 );// fu qty 2
		FuQty_2 = Round( (100.0 * data) / 2048 );

		data = pADC2->GetData( 4 );// h2o qty 2
		H2OQty_2 = Round( (100.0 * data) / 2048 );

		data = pADC2->GetData( 5 );// h2o qty 3
		H2OQty_3 = Round( (100.0 * data) / 2048 );

		data = pADC2->GetData( 6 );// fu qty 3
		FuQty_3 = Round( (100.0 * data) / 2048 );

		data = pADC2->GetData( 7 );// fu press 1
		Fu_Press_1 = Round( (500.0 * data) / 2048 );

		data = pADC2->GetData( 8 );// oil in 1
		OilIn_1 = Round( (500.0 * data) / 2048 );

		data = pADC2->GetData( 12 );// fu press 2
		Fu_Press_2 = Round( (500.0 * data) / 2048 );

		data = pADC2->GetData( 13 );// oil in 2
		OilIn_2 = Round( (500.0 * data) / 2048 );

		data = pADC2->GetData( 17 );// fu press 3
		Fu_Press_3 = Round( (500.0 * data) / 2048 );

		data = pADC2->GetData( 18 );// oil in 3
		OilIn_3 = Round( (500.0 * data) / 2048 );
		return true;
	}

	bool IDP::GetHYDdata( unsigned short& Qty_1, unsigned short& Qty_2, unsigned short& Qty_3, unsigned short& Press_1, unsigned short& Press_2, unsigned short& Press_3 ) const
	{
		unsigned short data = pADC2->GetData( 10 );// press 1
		Press_1 = Round( (4000.0 * data) / 2048 );
		
		data = pADC2->GetData( 11 );// qty 1
		Qty_1 = Round( (100.0 * data) / 2048 );
		
		data = pADC2->GetData( 15 );// press 2
		Press_2 = Round( (4000.0 * data) / 2048 );
		
		data = pADC2->GetData( 16 );// qty 2
		Qty_2 = Round( (100.0 * data) / 2048 );
		
		data = pADC2->GetData( 20 );// press 3
		Press_3 = Round( (4000.0 * data) / 2048 );
		
		data = pADC2->GetData( 21 );// qty 3
		Qty_3 = Round( (100.0 * data) / 2048 );
		return true;
	}

	bool IDP::GetWOW( void ) const
	{
		return pLanding_SOP->GetWOWLON();
	}

	double IDP::GetNZError( void ) const
	{
		return pAerojetDAP->GetNZError();
	}

	bool IDP::GetPrefinalState( void ) const
	{
		return pAerojetDAP->GetPrefinalState();
	}

	double IDP::GetYRunwayPositionError( void ) const
	{
		return pAerojetDAP->GetYRunwayPositionError();
	}

	bool IDP::GetOnHACState( void ) const
	{
		return pAerojetDAP->GetOnHACState();
	}

	double IDP::GetHACRadialError( void ) const
	{
		return pAerojetDAP->GetHACRadialError();
	}

	double IDP::GetTimeToHAC( void ) const
	{
		return pAerojetDAP->GetTimeToHAC();
	}

	double IDP::GetdeltaAZ( void ) const
	{
		return pAerojetDAP->GetdeltaAZ();
	}

	void IDP::GetSelectedRunway( char* rw ) const
	{
		pAerojetDAP->GetSelectedRunway( rw );
		return;
	}

	bool IDP::GetApproachAndLandState( void ) const
	{
		return pAerojetDAP->GetApproachAndLandState();
	}

	double IDP::GetVacc( void ) const
	{
		return pAerojetDAP->GetVacc();
	}

	double IDP::GetHTA( void ) const
	{
		return pAerojetDAP->GetHTA();
	}

	double IDP::GetGlideSlopeDistance( void ) const
	{
		return pAerojetDAP->GetGlideSlopeDistance();
	}

	double IDP::GetNZ( void ) const
	{
		return pAerojetDAP->GetNZ();
	}

	double IDP::GetdeltaAZLimit( void ) const
	{
		return pAerojetDAP->GetdeltaAZLimit();
	}

	double IDP::GetSelectedRunwayHeading( void ) const
	{
		return pAerojetDAP->GetSelectedRunwayHeading();
	}

	double IDP::GetTargetHeading( void ) const
	{
		return pAscentDAP->GetTargetHeading();
	}

	bool IDP::GetFCSmode( void ) const
	{
		return pAscentDAP->GetFCSmode();
	}

	double IDP::GetAltitude( void ) const
	{
		return pAerojetDAP->GetAltitude();
	}

	double IDP::GetAltitudeRate( void ) const
	{
		return pAerojetDAP->GetAltitudeRate();
	}

	double IDP::GetVrel( void ) const
	{
		return pAerojetDAP->GetVrel();
	}

	double IDP::GetSelectedRunwayRange( void ) const
	{
		return pDedicatedDisplay_SOP->GetSelectedRunwayRange();
	}

	double IDP::GetHACCRange( void ) const
	{
		return pDedicatedDisplay_SOP->GetHACCRange();
	}

	double IDP::GetPrimaryBearing( void ) const
	{
		return pDedicatedDisplay_SOP->GetPrimaryBearing();
	}

	char IDP::GetPrimaryBearingType( void ) const
	{
		return pDedicatedDisplay_SOP->GetPrimaryBearingType();
	}

	double IDP::GetSecondaryBearing( void ) const
	{
		return pDedicatedDisplay_SOP->GetSecondaryBearing();
	}

	char IDP::GetSecondaryBearingType( void ) const
	{
		return pDedicatedDisplay_SOP->GetSecondaryBearingType();
	}

	double IDP::GetCourseDeviation( void ) const
	{
		return pDedicatedDisplay_SOP->GetCourseDeviation();
	}

	double IDP::GetCourseDeviationScale( void ) const
	{
		return pDedicatedDisplay_SOP->GetCourseDeviationScale();
	}

	double IDP::GetGlideSlopeDeviation( void ) const
	{
		return pDedicatedDisplay_SOP->GetGlideSlopeDeviation();
	}

	double IDP::GetGlideSlopeDeviationScale( void ) const
	{
		return pDedicatedDisplay_SOP->GetGlideSlopeDeviationScale();
	}

	bool IDP::GetGSFlag( void ) const
	{
		return pDedicatedDisplay_SOP->GetGSFlag();
	}
}
