#include "CMessage.h"
#include <iostream>





// Message unpacking constructor
CMessage::CMessage(char* buff, unsigned int buff_len)
{
	unsigned char crc;
	if (buff_len > 7)
	{
		for (int i = 0; i < buff_len - 8; i++)
		{
			if (buff[i] == SOH && (buff[i + 6] & 0xf) == 0x6)
			{
				size_data = ((0 | buff[i + 1]) << 8 | buff[i + 2]) >> 6;
				if (i + size_data + 7 <= buff_len)
				{
					crc = buff[i + 7];
					buff[i + 7] = 0;
					CRC_8 = CRC(buff);
					if (crc == CRC_8)
					{
						this->type_msg = static_cast<Type_msg>((0 | buff[i + 2]) & 0x3f);
						this->adrr_from.addr_high_man = buff[i + 3] >> 4;
						this->adrr_from.addr_migl_man = buff[i + 3] & 0xf;
						this->adrr_from.addr_component = buff[i + 4] >> 4;
						this->adrr_to.addr_high_man = buff[i + 4] & 0xf;
						this->adrr_to.addr_migl_man = buff[i + 5] >> 4;
						this->adrr_to.addr_component = buff[i + 5] & 0xf;
						this->delivery_confirm = static_cast<Delivery>(buff[i + 6] >> 6);
						this->priority = static_cast<EPriority>((buff[i + 6] & 0x3f) >> 4);
						if (size_data != 0)
						{
							content();
							this->data->unmarshall(&buff[i + 8], size_data);
						}
						break;
					}
				}
			}
		}
	}
};

// Message packing constructor
CMessage::CMessage(Type_msg type_msg, Addr adrr_from, Addr adrr_to, Delivery delivery_confirm, EPriority priority, ACTypeMSG* data)
{
	this->type_msg = static_cast<Type_msg>(type_msg);
	this->adrr_from = adrr_from;
	this->adrr_to = adrr_to;
	this->delivery_confirm = static_cast<Delivery>(delivery_confirm);
	this->priority = static_cast<EPriority>(priority);
	mess = new char[MESSAGE_LEN];
	content();
	if (data->get_mem_size() != 0)
	{
		memcpy(this->data, data, data->get_mem_size());
	}
	else
	{
		std::cout << "FUCK" << std::endl;
	}
	this->size_data = this->data->marshall(mess + 8);
	pack();

};


CMessage::~CMessage()
{
	/*if (data != nullptr)
	{
		delete data;
	}
	if (mess != nullptr)
	{
		delete[] mess;
	}*/
};

char* CMessage::get_mess()
{
	return mess;
};

// CRC 8 function
// poly: 00011100 - 28 d
// start 00000000 - 0 d
char CMessage::CRC(const char* buff)
{
	char CRC = 0;
	unsigned int len = size_data + 8;
	while (len--)
	{
		CRC ^= *buff++;
		for (int i = 0; i < 8; i++)
		{
			CRC = CRC & 128 ? (CRC << 1) ^ 28 : (CRC << 1);
		}
	}
	return CRC;
}


// Message pack function
bool CMessage::pack()
{
	bool state = false;
	mess[0] = SOH;
	mess[1] = size_data >> 2 | 0;
	mess[2] = size_data << 6 | type_msg;
	mess[3] = adrr_from.addr_high_man << 4 | adrr_from.addr_migl_man;
	mess[4] = adrr_from.addr_component << 4 | adrr_to.addr_high_man;
	mess[5] = adrr_to.addr_migl_man << 4 | adrr_to.addr_component;
	mess[6] = delivery_confirm << 6 | static_cast<int>(priority) << 4 | 0x6;
	mess[7] = 0;
	CRC_8 = CRC(mess);
	mess[7] = CRC_8;

	return state;
};

// Âata type definition function
void CMessage::content()
{
	switch (type_msg)
	{
	case R_NAVIGATION:
		data = new CR_Navig;
		break;
	case R_TARGET_POINT:
		data = new CR_TP;
		break;
	case R_COMMAND_STATE:
		data = new CR_State;
		break;
	case R_CONNETCTED_COMPONENTS:
		data = new CR_CC;
		break;
	case	R_IS_ALIVE:
		data = new CR_IA;
		break;
	case TIMING:
		data = new C_Timing;
		break;
	case CALIBRATION:
		data = new C_Calibration;
		break;
	case SET_OPERATION_MODE:
		data = new C_OpMode;
		break;
	case CHANGE_PARAMETERS:
		data = new C_Change;
		break;
	default:
		data = new ACTypeMSG;
		break;
	}
};

// Zero class methods
int ACTypeMSG::marshall(char* data)
{
	return 0;
};

int ACTypeMSG::unmarshall(char* data, int data_len)
{
	return 1;
};

char* ACTypeMSG::toString(char* buff, int buff_len)
{
	memset(buff, 0, buff_len);
	return buff;
};

int ACTypeMSG::get_mem_size()
{
	return 0;
};

// CR_Navig class methods
CR_Navig::CR_Navig(SR_Navig *data)
{
	sdata = *data;
};

int CR_Navig::marshall(char* data)
{
	memset(data, 0, sizeof(SR_Navig));
	memcpy(data, &sdata, sizeof(SR_Navig));
	return sizeof(SR_Navig);
};

int CR_Navig::unmarshall(char* data, int data_len)
{
	int state = -1;
	if (data_len == sizeof(SR_Navig))
	{
		memcpy(&sdata, data, sizeof(SR_Navig));
		state = 1;
	}
	return state;
};

char* CR_Navig::toString(char* buff, int buff_len)
{
	unsigned int len = buff_len;
	char* bstr = new char[524]();
	sprintf(bstr, "%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;",
		sdata.alpha, sdata.dalpha, sdata.ddalpha, sdata.betta, sdata.dbetta, sdata.ddbetta, sdata.gamma, sdata.dgamma, sdata.ddgamma, sdata.x, sdata.dx, sdata.ddx,
		sdata.y, sdata.dy, sdata.ddy, sdata.z, sdata.dz, sdata.ddz);
	memset(buff, 0, buff_len);
	if (buff_len > 524)
	{
		len = 524;
	}
	memcpy(buff, bstr, len);
	buff[buff_len - 1] = NULL;
	delete[] bstr;
	return buff;
};

int CR_Navig::get_mem_size()
{
	return sizeof(CR_Navig);
};


// CR_TP class methods
CR_TP::CR_TP(SR_TP* data)
{
	sdata = *data;
};

int CR_TP::marshall(char* data)
{
	memset(data, 0, sizeof(SR_TP));
	memcpy(data, &sdata, sizeof(SR_TP));
	return sizeof(SR_TP);
};

int CR_TP::unmarshall(char* data, int data_len)
{
	int state = -1;
	if (data_len == sizeof(SR_TP))
	{
		memcpy(&sdata, data, sizeof(SR_TP));
		state = 1;
	}
	return state;
};

char* CR_TP::toString(char* buff, int buff_len)
{
	char* bstr = new char[524]();
	sprintf(bstr, "%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;%0.7g;",
		sdata.alpha, sdata.dalpha, sdata.betta, sdata.dbetta, sdata.gamma, sdata.dgamma, sdata.x, sdata.dx,
		sdata.y, sdata.dy, sdata.z, sdata.dz);
	memset(buff, 0, buff_len);
	memcpy(buff, bstr, buff_len - 1);
	buff[buff_len - 1] = NULL;
	delete[] bstr;
	return buff;
};

// CR_State class methods
CR_State::CR_State(SR_State* data)
{
	sdata = *data;
};

int CR_State::marshall(char* data)
{
	memset(data, 0, sizeof(SR_State));
	memcpy(data, &sdata, sizeof(SR_State));
	return sizeof(SR_State);
};

int CR_State::unmarshall(char* data, int data_len)
{
	int state = -1;
	if (data_len == sizeof(SR_State))
	{
		memcpy(&sdata, data, sizeof(SR_State));
		state = 1;
	}
	return state;
};

char* CR_State::toString(char* buff, int buff_len)
{
	char* bstr = new char[50]();
	sprintf(bstr, "%d;%d;", sdata.command, sdata.state);
	memset(buff, 0, buff_len);
	memcpy(buff, bstr, buff_len - 1);
	buff[buff_len - 1] = NULL;
	delete[] bstr;
	return buff;
};

// CR_CC class methods
CR_CC::CR_CC(SR_ListCMP* data)
{
	sdata = *data;
};

int CR_CC::marshall(char* data)
{
	memset(data, 0, sizeof(SR_ListCMP));
	memcpy(data, &sdata, sizeof(SR_ListCMP));
	return sizeof(SR_ListCMP);
};

int CR_CC::unmarshall(char* data, int data_len)
{
	int state = -1;
	if (data_len == sizeof(SR_ListCMP))
	{
		memcpy(&sdata, data, sizeof(SR_ListCMP));
		state = 1;
	}
	return state;
};

char* CR_CC::toString(char* buff, int buff_len)
{
	char* bstr = new char[1048]();
	int count = 4;
	sprintf(bstr, "%d;", sdata.count);
	for (int i = 0; i < 15; i++)
	{
		sprintf(bstr+count, "%d.%d.%d;", sdata.addrr[i].addr_high_man, sdata.addrr[i].addr_migl_man, sdata.addrr[i].addr_component);
		count = count + 10;
		for (int j = 0; j < 64; j++)
		{
			sprintf(bstr + count, "%c", sdata.name[i][j]);
			count++;
		}
	}
	*(bstr + count) = ';';
	memset(buff, 0, buff_len);
	memcpy(buff, bstr, buff_len - 1);
	buff[buff_len - 1] = NULL;
	delete[] bstr;
	return buff;
};

// CR_IA class methods
CR_IA::CR_IA(SR_IA* data)
{
	sdata = *data;
};

int CR_IA::marshall(char* data)
{
	memset(data, 0, sizeof(SR_IA));
	memcpy(data, &sdata, sizeof(SR_IA));
	return sizeof(SR_IA);
};

int CR_IA::unmarshall(char* data, int data_len)
{
	int state = -1;
	if (data_len == sizeof(SR_IA))
	{
		memcpy(&sdata, data, sizeof(SR_IA));
		state = 1;
	}
	return state;
};

char* CR_IA::toString(char* buff, int buff_len)
{
	char* bstr = new char[10]();
	sprintf(bstr, "%d;", sdata.state);
	memset(buff, 0, buff_len);
	memcpy(buff, bstr, buff_len - 1);
	buff[buff_len - 1] = NULL;
	delete[] bstr;
	return buff;
};

// C_Timing class methods
C_Timing::C_Timing(S_Timing* data)
{
	sdata = *data;
};

int C_Timing::marshall(char* data)
{
	memset(data, 0, sizeof(S_Timing));
	memcpy(data, &sdata, sizeof(S_Timing));
	return sizeof(S_Timing);
};

int C_Timing::unmarshall(char* data, int data_len)
{
	int state = -1;
	if (data_len == sizeof(S_Timing))
	{
		memcpy(&sdata, data, sizeof(S_Timing));
		state = 1;
	}
	return state;
};

char* C_Timing::toString(char* buff, int buff_len)
{
	char* bstr = new char[48]();
	sprintf(bstr, "%d;", sdata.time);
	memset(buff, 0, buff_len);
	memcpy(buff, bstr, buff_len - 1);
	buff[buff_len - 1] = NULL;
	delete[] bstr;
	return buff;
};

// C_Calibration class methods
C_Calibration::C_Calibration(S_Calibration* data)
{
	sdata = *data;
};

int C_Calibration::marshall(char* data)
{
	memset(data, 0, sizeof(S_Calibration));
	memcpy(data, &sdata, sizeof(S_Calibration));
	return sizeof(S_Calibration);
};

int C_Calibration::unmarshall(char* data, int data_len)
{
	int state = -1;
	if (data_len == sizeof(S_Calibration))
	{
		memcpy(&sdata, data, sizeof(S_Calibration));
		state = 1;
	}
	return state;
};

char* C_Calibration::toString(char* buff, int buff_len)
{
	char* bstr = new char[260]();
	for (int i = 0; i < 256; i++)
	{
		sprintf(bstr+i, "%c", sdata.nameDevice[i]);
	}
	memset(buff, 0, buff_len);
	memcpy(buff, bstr, buff_len - 1);
	buff[buff_len - 1] = NULL;
	delete[] bstr;
	return buff;
};

// C_OpMode class methods
C_OpMode::C_OpMode(S_OpMode* data)
{
	sdata = *data;
};

int C_OpMode::marshall(char* data)
{
	memset(data, 0, sizeof(S_OpMode));
	memcpy(data, &sdata, sizeof(S_OpMode));
	return sizeof(S_OpMode);
};

int C_OpMode::unmarshall(char* data, int data_len)
{
	int state = -1;
	if (data_len == sizeof(S_OpMode))
	{
		memcpy(&sdata, data, sizeof(S_OpMode));
		state = 1;
	}
	return state;
};

char* C_OpMode::toString(char* buff, int buff_len)
{
	char* bstr = new char[10]();
	sprintf(bstr, "%d;", sdata.mode);
	memset(buff, 0, buff_len);
	memcpy(buff, bstr, buff_len - 1);
	buff[buff_len - 1] = NULL;
	delete[] bstr;
	return buff;
};

// C_Change class methods
C_Change::C_Change(S_Change* data)
{
	sdata = *data;
};

int C_Change::marshall(char* data)
{
	memset(data, 0, sizeof(S_Change));
	memcpy(data, &sdata, sizeof(S_Change));
	return sizeof(S_Change);
};

int C_Change::unmarshall(char* data, int data_len)
{
	int state = -1;
	if (data_len == sizeof(S_Change))
	{
		memcpy(&sdata, data, sizeof(S_Change));
		state = 1;
	}
	return state;
};

char* C_Change::toString(char* buff, int buff_len)
{
	char* bstr = new char[280]();
	sprintf(bstr, "%d;", sdata.type);
	sprintf(bstr+5, "%d;", sdata.size);
	int i = 15;
	for (; i < 256+15; i++)
	{
		sprintf(bstr + i, "%c", sdata.parameter[i-15]);
	}
	*(bstr + i) = ';';
	memset(buff, 0, buff_len);
	memcpy(buff, bstr, buff_len - 1);
	buff[buff_len - 1] = NULL;
	delete[] bstr;
	return buff;
};