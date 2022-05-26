#ifndef __IPCAMERA_H__
#define __IPCAMERA_H__

#include "Device.h"
#include <iostream>

class IPCamera : public Device
{
public:
	IPCamera();
	~IPCamera();

	void Authenticate();
	void Intercom();
	void SingleChannelRTC();
	void CloudStorage();

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
