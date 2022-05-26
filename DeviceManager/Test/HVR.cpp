#include "HVR.h"

HVR::HVR() {
    m_sIP = "";
    m_sMAC = "";
    m_sPort = "";
	m_sForm = "HVR";
}

HVR::~HVR() {

}

void HVR::Authenticate() {
	std::cout << "HVR::Authenticate" << std::endl;
}

void HVR::Intercom() {
	std::cout << "HVR::Intercom" << std::endl;
}

void HVR::MultiChannelRTC() {
	std::cout << "HVR::MultiChannelRTC" << std::endl;
}

void HVR::Replay() {
	std::cout << "HVR::Replay" << std::endl;
}

std::string HVR::GetIP() {
	return m_sIP;
}

void HVR::SetIP(std::string IP) {
	m_sIP = IP;
}

std::string HVR::GetMAC() {
	return m_sMAC;
}

void HVR::SetMAC(std::string MAC) {
	m_sMAC = MAC;
}

std::string HVR::GetPort() {
	return m_sPort;
}

void HVR::SetPort(std::string port) {
	m_sPort = port;
}

std::string HVR::GetForm() {
	return m_sForm;
}
