#ifndef __NVR_H__
#define __NVR_H__

#include <iostream>

#include "Device.h"

class NVR : public Device
{
public:
	NVR();
	~NVR();

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
