/******* SSV File Modification Notice *******
Date         Developer
2020/06/20   GLS
2020/06/23   GLS
2021/08/24   GLS
2022/03/24   GLS
2022/03/26   GLS
2022/04/20   GLS
2022/04/28   GLS
2022/08/05   GLS
2022/09/06   GLS
********************************************/
#include "PanelR13U.h"
#include "..\Atlantis.h"
#include "..\ParameterValues.h"
#include "ThumbWheel.h"
#include "PushWheel.h"
#include "vc_defs.h"
#include "..\meshres_vc_r13u.h"


namespace vc
{
	PanelR13U::PanelR13U( Atlantis* _sts, const std::string &orbiter ):AtlantisPanel( _sts, "R13U" )
	{
		DefineMesh( MESHNAME_PANELR13U );

		if ((orbiter == "Discovery") || (orbiter == "Endeavour")) thumbwheels = false;
		else thumbwheels = true;

		if (thumbwheels)
		{
			Add( pParameterSelect100 = new ThumbWheel( _sts, "PARAMETER SELECT 100" ) );
			Add( pParameterSelect10 = new ThumbWheel( _sts, "PARAMETER SELECT 10" ) );
			Add( pParameterSelect1 = new ThumbWheel( _sts, "PARAMETER SELECT 1" ) );

			Add( pLimitSetValue1 = new ThumbWheel( _sts, "LIMIT SET VALUE 1" ) );
			Add( pLimitSetValue01 = new ThumbWheel( _sts, "LIMIT SET VALUE 01" ) );
			Add( pLimitSetValue001 = new ThumbWheel( _sts, "LIMIT SET VALUE 001" ) );
		}
		else
		{
			Add( pParameterSelect100 = new PushWheel( _sts, "PARAMETER SELECT 100" ) );
			Add( pParameterSelect10 = new PushWheel( _sts, "PARAMETER SELECT 10" ) );
			Add( pParameterSelect1 = new PushWheel( _sts, "PARAMETER SELECT 1" ) );

			Add( pLimitSetValue1 = new PushWheel( _sts, "LIMIT SET VALUE 1" ) );
			Add( pLimitSetValue01 = new PushWheel( _sts, "LIMIT SET VALUE 01" ) );
			Add( pLimitSetValue001 = new PushWheel( _sts, "LIMIT SET VALUE 001" ) );
		}
	}

	PanelR13U::~PanelR13U()
	{
	}

	void PanelR13U::DefineVC()
	{
		const VECTOR3 switch_rot = _V( 0.0, 0.0, -1.0 );

		AddAIDToMouseEventList( AID_R13U );

		
		pParameterSelect100->DefineOutputs( 0, 0 );
		pParameterSelect100->DefineOutputs( 1, 1 );
		pParameterSelect100->DefineOutputs( 2, 0 );
		pParameterSelect100->DefineOutputs( 3, 1 );
		pParameterSelect100->DefineOutputs( 4, 0 );
		pParameterSelect100->DefineOutputs( 5, 1 );
		pParameterSelect100->DefineOutputs( 6, 0 );
		pParameterSelect100->DefineOutputs( 7, 1 );
		pParameterSelect100->DefineOutputs( 8, 0 );
		pParameterSelect100->DefineOutputs( 9, 1 );
		pLimitSetValue1->SetBounds( 0, 5 );
		pLimitSetValue001->DefineOutputs( 0, 0 );
		pLimitSetValue001->DefineOutputs( 1, 5 );
		pLimitSetValue001->DefineOutputs( 2, 0 );
		pLimitSetValue001->DefineOutputs( 3, 5 );
		pLimitSetValue001->DefineOutputs( 4, 0 );
		pLimitSetValue001->DefineOutputs( 5, 5 );
		pLimitSetValue001->DefineOutputs( 6, 0 );
		pLimitSetValue001->DefineOutputs( 7, 5 );
		pLimitSetValue001->DefineOutputs( 8, 0 );
		pLimitSetValue001->DefineOutputs( 9, 5 );
		if (thumbwheels)
		{
			pParameterSelect100->DefineWheelGroup( GRP_S4_THUMBWHEEL_100_R13U_VC );
			pParameterSelect100->SetReference( _V( 1.423215, 2.330835, 0.0 ), switch_rot );
			pParameterSelect100->SetMouseRegion( AID_R13U, 0.676663f, 0.714185f, 0.718559f, 0.801454f );///////////////////////

			pParameterSelect10->DefineWheelGroup( GRP_S4_THUMBWHEEL_10_R13U_VC );
			pParameterSelect10->SetReference( _V( 1.423215, 2.330835, 0.0 ), switch_rot );
			pParameterSelect10->SetMouseRegion( AID_R13U, 0.676663f, 0.714185f, 0.718559f, 0.801454f );///////////////////////
			
			pParameterSelect1->DefineWheelGroup( GRP_S4_THUMBWHEEL_1_R13U_VC );
			pParameterSelect1->SetReference( _V( 1.423215, 2.330835, 0.0 ), switch_rot );
			pParameterSelect1->SetMouseRegion( AID_R13U, 0.676663f, 0.714185f, 0.718559f, 0.801454f );///////////////////////


			pLimitSetValue1->DefineWheelGroup( GRP_S6_THUMBWHEEL_1_R13U_VC );
			pLimitSetValue1->SetReference( _V( 1.361861, 2.290523, 0.0 ), switch_rot );
			pLimitSetValue1->SetMouseRegion( AID_R13U, 0.676663f, 0.714185f, 0.718559f, 0.801454f );///////////////////////

			pLimitSetValue01->DefineWheelGroup( GRP_S6_THUMBWHEEL_01_R13U_VC );
			pLimitSetValue01->SetReference( _V( 1.361861, 2.290523, 0.0 ), switch_rot );
			pLimitSetValue01->SetMouseRegion( AID_R13U, 0.676663f, 0.714185f, 0.718559f, 0.801454f );///////////////////////

			pLimitSetValue001->DefineWheelGroup( GRP_S6_THUMBWHEEL_005_R13U_VC );
			pLimitSetValue001->SetReference( _V( 1.361861, 2.290523, 0.0 ), switch_rot );
			pLimitSetValue001->SetMouseRegion( AID_R13U, 0.676663f, 0.714185f, 0.718559f, 0.801454f );///////////////////////
		}
		else
		{
			const VECTOR3 pusher_dir = _V( 0.54928, -0.83564, 0.0 );

			pParameterSelect100->DefineWheelGroup( GRP_S4_PUSHWHEEL_100_R13U_VC );
			dynamic_cast<PushWheel*>(pParameterSelect100)->DefinePusherGroups( GRP_S4_PUSHWHEEL_PISTON_UP_100_R13U_VC, -1 );
			dynamic_cast<PushWheel*>(pParameterSelect100)->DefinePusherDirection( pusher_dir );
			pParameterSelect100->SetReference( _V( 1.422242, 2.332317, 0.0 ), switch_rot );
			pParameterSelect100->SetMouseRegion( AID_R13U, 0.676663f, 0.714185f, 0.718559f, 0.801454f );///////////////////////

			pParameterSelect10->DefineWheelGroup( GRP_S4_PUSHWHEEL_10_R13U_VC );
			dynamic_cast<PushWheel*>(pParameterSelect10)->DefinePusherGroups( GRP_S4_PUSHWHEEL_PISTON_UP_10_R13U_VC, GRP_S4_PUSHWHEEL_PISTON_DOWN_10_R13U_VC );
			dynamic_cast<PushWheel*>(pParameterSelect10)->DefinePusherDirection( pusher_dir );
			pParameterSelect10->SetReference( _V( 1.422242, 2.332317, 0.0 ), switch_rot );
			pParameterSelect10->SetMouseRegion( AID_R13U, 0.676663f, 0.714185f, 0.718559f, 0.801454f );///////////////////////
			
			pParameterSelect1->DefineWheelGroup( GRP_S4_PUSHWHEEL_1_R13U_VC );
			dynamic_cast<PushWheel*>(pParameterSelect1)->DefinePusherGroups( GRP_S4_PUSHWHEEL_PISTON_UP_1_R13U_VC, GRP_S4_PUSHWHEEL_PISTON_DOWN_1_R13U_VC );
			dynamic_cast<PushWheel*>(pParameterSelect1)->DefinePusherDirection( pusher_dir );
			pParameterSelect1->SetReference( _V( 1.422242, 2.332317, 0.0 ), switch_rot );
			pParameterSelect1->SetMouseRegion( AID_R13U, 0.676663f, 0.714185f, 0.718559f, 0.801454f );///////////////////////


			pLimitSetValue1->DefineWheelGroup( GRP_S6_PUSHWHEEL_1_R13U_VC );
			dynamic_cast<PushWheel*>(pLimitSetValue1)->DefinePusherGroups( GRP_S6_PUSHWHEEL_PISTON_UP_1_R13U_VC, GRP_S6_PUSHWHEEL_PISTON_DOWN_1_R13U_VC );
			dynamic_cast<PushWheel*>(pLimitSetValue1)->DefinePusherDirection( pusher_dir );
			pLimitSetValue1->SetReference( _V( 1.360887, 2.292005, 0.0 ), switch_rot );
			pLimitSetValue1->SetMouseRegion( AID_R13U, 0.676663f, 0.714185f, 0.718559f, 0.801454f );///////////////////////

			pLimitSetValue01->DefineWheelGroup( GRP_S6_PUSHWHEEL_01_R13U_VC );
			dynamic_cast<PushWheel*>(pLimitSetValue01)->DefinePusherGroups( GRP_S6_PUSHWHEEL_PISTON_UP_01_R13U_VC, GRP_S6_PUSHWHEEL_PISTON_DOWN_01_R13U_VC );
			dynamic_cast<PushWheel*>(pLimitSetValue01)->DefinePusherDirection( pusher_dir );
			pLimitSetValue01->SetReference( _V( 1.360887, 2.292005, 0.0 ), switch_rot );
			pLimitSetValue01->SetMouseRegion( AID_R13U, 0.676663f, 0.714185f, 0.718559f, 0.801454f );///////////////////////

			pLimitSetValue001->DefineWheelGroup( GRP_S6_PUSHWHEEL_005_R13U_VC );
			dynamic_cast<PushWheel*>(pLimitSetValue001)->DefinePusherGroups( GRP_S6_PUSHWHEEL_PISTON_UP_005_R13U_VC, -1 );
			dynamic_cast<PushWheel*>(pLimitSetValue001)->DefinePusherDirection( pusher_dir );
			pLimitSetValue001->SetReference( _V( 1.360887, 2.292005, 0.0 ), switch_rot );
			pLimitSetValue001->SetMouseRegion( AID_R13U, 0.676663f, 0.714185f, 0.718559f, 0.801454f );///////////////////////
		}
		return;
	}

	void PanelR13U::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_R13U, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_R13U,
			_V( 1.54125, 2.43524, 12.561 ) + ofs, _V( 1.54125, 2.43524, 12.2564 ) + ofs,
			_V( 1.30501, 2.28002, 12.561 ) + ofs, _V( 1.30501, 2.28002, 12.2564 ) + _V( 0.001, 0.001, 0.001 ) + ofs );
		return;
	}

	void PanelR13U::Realize()
	{
		/*DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "?????", 16 );
		pParameterSelect100->Connect( pBundle, 0, 0 );

		pParameterSelect10->Connect( pBundle, 1, 0 );
		pParameterSelect10->Connect( pBundle, 2, 1 );
		pParameterSelect10->Connect( pBundle, 3, 2 );
		pParameterSelect10->Connect( pBundle, 4, 3 );

		pParameterSelect1->Connect( pBundle, 5, 0 );
		pParameterSelect1->Connect( pBundle, 6, 1 );
		pParameterSelect1->Connect( pBundle, 7, 2 );
		pParameterSelect1->Connect( pBundle, 8, 3 );

		pLimitSetValue1->Connect( pBundle, 8, 0 );
		pLimitSetValue1->Connect( pBundle, 9, 1 );
		pLimitSetValue1->Connect( pBundle, 10, 2 );

		pLimitSetValue01->Connect( pBundle, 11, 0 );
		pLimitSetValue01->Connect( pBundle, 12, 1 );
		pLimitSetValue01->Connect( pBundle, 13, 2 );
		pLimitSetValue01->Connect( pBundle, 14, 3 );

		pLimitSetValue001->Connect( pBundle, 15, 0 );*/

		AtlantisPanel::Realize();
		return;
	}

	void PanelR13U::VisualCreated( void )
	{
		// hide thumbwheels or pushwheels
		DEVMESHHANDLE hDevMesh = STS()->GetDevMesh( STS()->Get_vis(), GetVCMeshIndex() );
		if (hDevMesh != NULL)
		{
			GROUPEDITSPEC grpSpec;
			grpSpec.flags = GRPEDIT_SETUSERFLAG;
			grpSpec.UsrFlag = 0x00000003;// hide group and shadow
			if (thumbwheels)
			{
				oapiEditMeshGroup( hDevMesh, GRP_S4_PUSHWHEEL_1_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S4_PUSHWHEEL_10_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S4_PUSHWHEEL_100_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S4_PUSHWHEEL_CASE_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S4_PUSHWHEEL_PISTON_DOWN_1_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S4_PUSHWHEEL_PISTON_DOWN_10_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S4_PUSHWHEEL_PISTON_UP_1_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S4_PUSHWHEEL_PISTON_UP_10_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S4_PUSHWHEEL_PISTON_UP_100_R13U_VC, &grpSpec );

				oapiEditMeshGroup( hDevMesh, GRP_S6_PUSHWHEEL_005_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S6_PUSHWHEEL_01_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S6_PUSHWHEEL_1_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S6_PUSHWHEEL_CASE_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S6_PUSHWHEEL_PISTON_DOWN_01_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S6_PUSHWHEEL_PISTON_DOWN_1_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S6_PUSHWHEEL_PISTON_UP_005_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S6_PUSHWHEEL_PISTON_UP_01_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S6_PUSHWHEEL_PISTON_UP_1_R13U_VC, &grpSpec );
			}
			else
			{
				oapiEditMeshGroup( hDevMesh, GRP_S4_THUMBWHEEL_1_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S4_THUMBWHEEL_10_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S4_THUMBWHEEL_100_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S4_THUMBWHEEL_CASE_R13U_VC, &grpSpec );

				oapiEditMeshGroup( hDevMesh, GRP_S6_THUMBWHEEL_005_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S6_THUMBWHEEL_01_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S6_THUMBWHEEL_1_R13U_VC, &grpSpec );
				oapiEditMeshGroup( hDevMesh, GRP_S6_THUMBWHEEL_CASE_R13U_VC, &grpSpec );
			}
		}

		AtlantisPanel::VisualCreated();
		return;
	}
};
