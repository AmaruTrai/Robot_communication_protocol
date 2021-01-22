#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Structs.h"


// Virtual data class
class ACTypeMSG
{
public:
	virtual int marshall(char* data);
	virtual int unmarshall(char* data, int data_len);
	virtual char* toString(char* buff, int buff_len);
	virtual int get_mem_size();
};


// Message Main Class
class CMessage
{
private:
	// Header
	unsigned int size_data = 0;
	Type_msg type_msg = static_cast<Type_msg>(0);
	Addr adrr_from;
	Addr adrr_to;
	Delivery delivery_confirm = static_cast<Delivery>(0);
	EPriority priority = static_cast<EPriority>(0);
	unsigned char CRC_8 = 0;
	
	// Data
	ACTypeMSG *data = nullptr;
	char *mess = nullptr;

	bool pack();
	char CRC(const char* buff);
	void content();

public:

	CMessage(char* buff, unsigned int buff_len);
	CMessage(Type_msg type_msg, Addr adrr_from, Addr adrr_to, Delivery delivery_confirm, EPriority priority, ACTypeMSG* data);
	~CMessage();
	char* get_mess();
};


// Data class for R_NAVIGATION
class CR_Navig : public ACTypeMSG
{
public:
	SR_Navig sdata;


	CR_Navig() {};
	CR_Navig(SR_Navig *data);
	int marshall(char *data);
	int unmarshall(char *data, int data_len);
	char* toString(char *buff, int buff_len);
	int get_mem_size();
};

// Data class for R_TARGET_POINT
class CR_TP : public ACTypeMSG
{
private:
	SR_TP sdata;

public:
	CR_TP() {};
	CR_TP(SR_TP* data);
	int marshall(char* data);
	int unmarshall(char* data, int data_len);
	char* toString(char* buff, int buff_len);
};

// Data class for R_COMMAND_STATE
class CR_State : public ACTypeMSG
{
private:
	SR_State sdata;

public:
	CR_State() {};
	CR_State(SR_State* data);
	int marshall(char* data);
	int unmarshall(char* data, int data_len);
	char* toString(char* buff, int buff_len);
};

// Data class for R_CONNETCTED_COMPONENTS
class CR_CC : public ACTypeMSG
{
private:
	SR_ListCMP sdata;

public:
	CR_CC() {};
	CR_CC(SR_ListCMP* data);
	int marshall(char* data);
	int unmarshall(char* data, int data_len);
	char* toString(char* buff, int buff_len);
};

// Data class for R_IS_ALIVE
class CR_IA : public ACTypeMSG
{
private:
	SR_IA sdata;

public:
	CR_IA() {};
	CR_IA(SR_IA* data);
	int marshall(char* data);
	int unmarshall(char* data, int data_len);
	char* toString(char* buff, int buff_len);
};

// Data class for TIMING
class C_Timing : public ACTypeMSG
{
private:
	S_Timing sdata;

public:
	C_Timing() {};
	C_Timing(S_Timing* data);
	int marshall(char* data);
	int unmarshall(char* data, int data_len);
	char* toString(char* buff, int buff_len);
};

// Data class for CALIBRATION
class C_Calibration : public ACTypeMSG
{
private:
	S_Calibration sdata;

public:
	C_Calibration() {};
	C_Calibration(S_Calibration* data);
	int marshall(char* data);
	int unmarshall(char* data, int data_len);
	char* toString(char* buff, int buff_len);
};

// Data class for SET_OPERATION_MODE
class C_OpMode : public ACTypeMSG
{
private:
	S_OpMode sdata;

public:
	C_OpMode() {};
	C_OpMode(S_OpMode* data);
	int marshall(char* data);
	int unmarshall(char* data, int data_len);
	char* toString(char* buff, int buff_len);
};

// Data class for CHANGE_PARAMETERS
class C_Change : public ACTypeMSG
{
private:
	S_Change sdata;

public:
	C_Change() {};
	C_Change(S_Change* data);
	int marshall(char* data);
	int unmarshall(char* data, int data_len);
	char* toString(char* buff, int buff_len);
};