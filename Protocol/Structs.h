#pragma once

#define SOH 'V'
#define ERROR -1
#define NO_ERROR 0
#define MESSAGE_LEN 1032

// Enumerating of Message Types
enum Type_msg
{
	// Requests
	NAVIGATION,
	TARGET_POINT,
	SYSTEM_STATE,
	CONNETCTED_COMPONENTS,
	IS_ALIVE,

	// Commands
	START,
	STOP,
	PAUSE,
	CHANGE_CONFIG,
	CALIBRATION,
	SET_TARGET_POINT,
	SET_OPERATION_MODE,
	CHANGE_PARAMETERS,

	// Returns
	R_NAVIGATION,
	R_TARGET_POINT,
	R_COMMAND_STATE,
	R_CONNETCTED_COMPONENTS,
	R_IS_ALIVE,

	// Events
	INIT,
	CRASH,
	MISSION_COMPLETE,
	MISSION_IMPOSSIBLE,

	// Service Messages
	TIMING
};

// Enumerating of Command States
enum Command_state
{
	COMPLETED,
	EXECUTING,
	INTERRUPTED,
	SUSPENDED
};

// Enumerating of Error Types
enum Error
{
	Broken
};

// Enumerating of Operation Mode
enum Operation_mode
{
	TO_POINT,
	LINE,
	SPLINE
};
// Enumerating of Priority Levels
enum EPriority
{
	PRIORITY_MIN,
	PRIORITY_NORMAL,
	PRIORITY_MAX,
	PRIORITY_CRITICAL
};

// Delivery confirmation mode
enum Delivery
{
	NOT_REQUIRED,
	REQUIRED,
	CONFIRMATION
};


// Address Structure
struct Addr
{
	unsigned char addr_high_man = 0;
	unsigned char addr_migl_man = 0;
	unsigned char addr_component = 0;
};

enum EParameters
{
	TestName
};

#pragma pack(push, 1)
// Data struct for R_NAVIGATION
struct SR_Navig
{
	float x = 0;
	float y = 0;
	float z = 0;
	float dx = 0;
	float dy = 0;
	float dz = 0;
	float ddx = 0;
	float ddy = 0;
	float ddz = 0;
	float alpha = 0;
	float betta = 0;
	float gamma = 0;
	float dalpha = 0;
	float dbetta = 0;
	float dgamma = 0;
	float ddalpha = 0;
	float ddbetta = 0;
	float ddgamma = 0;
};

// Data struct for R_TARGET_POINT
struct SR_TP
{
	float x= 0;
	float y= 0;
	float z= 0;
	float dx= 0;
	float dy= 0;
	float dz= 0;
	float alpha= 0;
	float betta= 0;
	float gamma= 0;
	float dalpha= 0;
	float dbetta= 0;
	float dgamma= 0;
	float V= 0;
};

// Data struct for R_COMMAND_STATE
struct SR_State
{
	Type_msg command;
	Command_state state;
};

// Data struct for R_CONNETCTED_COMPONENTS
struct SR_ListCMP
{
	unsigned char count;
	Addr addrr[15];
	char name[15][64];
};

// Data struct for R_IS_ALIVE
struct SR_IA
{
	Command_state state;
};

// Data struct for TIMING
struct S_Timing
{
	long time = 0;
};

// Data strut for CALIBRATION
struct S_Calibration
{
	char nameDevice[256];
};

// Data struct for SET_OPERATION_MODE
struct S_OpMode
{
	Operation_mode mode;
};

// Data struct for CHANGE_PARAMETERS
struct S_Change
{
	EParameters type;
	unsigned char size = 0;
	char parameter[256];
};
#pragma pack (pop)