#include "NVR.h"

NVR::NVR() {
    m_sIP = "";
    m_sMAC = "";
    m_sPort = "";
	m_sForm = "NVR";
}

NVR::~NVR() {

}

void NVR::Authenticate() {
	std::cout << "NVR::Authenticate" << std::endl;
}

void NVR::Intercom() {
	std::cout << "NVR::Intercom" << std::endl;
}

void NVR::MultiChannelRTC() {
	std::cout << "NVR::MultiChannelRTC" << std::endl;
}

void NVR::Replay() {
	std::cout << "NVR::Replay" << std::endl;
}

std::string NVR::GetIP() {
	return m_sIP;
}

void NVR::SetIP(std::string IP) {
	m_sIP = IP;
}

std::string NVR::GetMAC() {
	return m_sMAC;
}

void NVR::SetMAC(std::string MAC) {
	m_sMAC = MAC;
}

std::string NVR::GetPort() {
	return m_sPort;
}

void NVR::SetPort(std::string port) {
	m_sPort = port;
}

std::string NVR::GetForm() {
	return m_sForm;
}
