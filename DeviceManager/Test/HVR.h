#ifndef __HVR_H__
#define __HVR_H__

#include <iostream>
#include <string>

#include "Device.h"

class HVR : public Device
{
public:
	HVR();
	~HVR();

	void Authenticate();
	void Intercom();
	void MultiChannelRTC();
	void Replay();

	std::string GetIP();
	void SetIP(std::string IP);

	std::string GetMAC();
	void SetMAC(std::string MAC);

	std::string GetPort();
	void SetPort(std::string port);

	std::string GetForm();

private:
	std::string m_sIP;
	std::string m_sMAC;
	std::string m_sPort;
	std::string m_sForm;
};

#endif
