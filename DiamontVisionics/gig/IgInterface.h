////////////////////////////////////////////////////////////////////////////////
//
// P-3C Visual Modification Source Code: Developed by Diamond Visionics, 2003.
//
// Distribution authorized to the Department of Defense (DoD) and DoD
// contractors only (critical technology) (15 Oct 93).  Other requests shall
// be referred to the Training System Program Office, Attention:
//             Naval Air Warfare Center Training Systems Division
//             12350 Research Parkway
//             Orlando, Florida 32826-3224
//
//
// $Workfile: IgInterface.h $
//
// DESCRIPTION:
// This file defines the message data types to be sent to the IG from the Host,
// and to the Host from the IG.
//
//
// $History: $
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------- User Includes -------------------------------

//------------------------------ System Includes ------------------------------

#ifndef DVC_IgInterfaceP3LeveLD_H
#define DVC_IgInterfaceP3LeveLD_H

#include <CigiBaseSOF.h>

#pragma pack(1)
#ifdef	__cplusplus
extern "C" {  // compliant with ANSI standard C
namespace dvc { 
namespace IgInterface {
#endif

enum IgBoolean { OFF = 0, ON };

enum IgMsgType
{
	NO_MESSAGE=0,				//0
	ENV_VIS,					//1
	ENV_RUNWAY,					//2
	ENV_STORM,					//3
	ENV_CLOUD,					//4
	ENV_WIND,					//5
	ENV_OCEAN,					//6
	ENV_RAIN,					//7
	LIGHTING_AIRFIELD,			//8
	LIGHTING_OWNSHIP,			//9
	LIGHTING_CULTURAL,			//10
	LIGHTING_FORMATION_AC,		//11
	TIME,						//12
	ENTITY,						//13
	CRASH,						//14
	TEST_PAGE,					//15
	ENTITY_ELEV_REQUEST,		//16
	TERRAIN_ELEV_REQUEST,		//17
	LOS_REQUEST,				//18
	DEL_ALL_ENTITIES,			//19
	IG_MSG_TYPE_RNG				//20
};


enum TestPageType
{
	BLACK_FIELD=0, UNIFORM_FIELD, SPHERE_PATTERN, LINEAR_CROSSHATCH, GREYSCALE,
	NINE_WHITE_SQUARES, WHITE_RECTANGLES, BLACK_SQUARE, PEAK_WHITE_RECTANGLE,
	CHECKERBOARD, RESOLUTION, COLOR_BLOCK, CYLINDRICAL_PATTERN,
	TEST_PAGE_TYPE_MAX
};

enum UniforomFeildModifiers
{
	FULL_WHITE=0, LIGHT_GRAY, UNIFORM_FEILD_MAX
};

enum SpherePatternModifiers
{
	HIGHLIGHTING_OFF=0, HIGHLIGHTING_ON, LINE_OF_CENTER_DOTS, BOX_OF_CENTER_DOTS, ONE_DEGREE_LINES, ONE_DEGREE_LINES_BLACK_BACK, CHECKERBOARD_WHITE_CENTER, CHECKERBOARD_WITH_DOTS, SPHERE_PATTERN_MAX
};

enum CylinderPatternModifiers
{
	CYLINDER_BACKGROUND_OFF, CYLINDER_BACKGROUND_ON, CYLINDER_PATTERN_MAX
};

enum GreyscalePatternModifiers
{
	BLACK_BOTTOM=0, BLACK_LEFT, BLACK_TOP, BLACK_RIGHT, GREYSCALE_MAX
};

enum ResolutionPatternModifiers
{
	RES_CENTER=0, RES_UPPER_LEFT, RES_UPPER_RIGHT, RES_LOWER_RIGHT, RES_LOWER_LEFT, RESOLUTION_MAX
};

struct TestPageMsg
{
	IgMsgType		msg;
	IgBoolean		test_page_active;
	int				test_channel;
	TestPageType	test_page_number;
	int				pattern_modifier;
	float			res_pattern_size;
};




#ifdef __cplusplus
}

} } // namespace
#endif
#pragma pack()
#endif // DVC_IgInterfaceP3LeveLD_H
