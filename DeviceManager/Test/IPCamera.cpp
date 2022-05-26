#include "IPCamera.h"

IPCamera::IPCamera() {
    m_sIP = "";
    m_sMAC = "";
    m_sPort = "";
	m_sForm = "IPCamera";
}

IPCamera::~IPCamera() {

}

void IPCamera::Authenticate() {
	std::cout << "IPCamera: Authenticate" << std::endl;
}

void IPCamera::Intercom() {
	std::cout << "IPCamera: Intercom" << std::endl;
}

void IPCamera::SingleChannelRTC() {
	std::cout << "IPCamera: SingleChannelRTC" << std::endl;
}

void IPCamera::CloudStorage() {
	std::cout << "IPCamera: SingleChannelRTC" << std::endl;
}

std::string IPCamera::GetIP() {
	return m_sIP;
}

void IPCamera::SetIP(std::string IP) {
	m_sIP = IP;
}

std::string IPCamera::GetMAC() {
	return m_sMAC;
}

void IPCamera::SetMAC(std::string MAC) {
	m_sMAC = MAC;
}

std::string IPCamera::GetPort() {
	return m_sPort;
}

void IPCamera::SetPort(std::string port) {
	m_sPort = port;
}

std::string IPCamera::GetForm() {
	return m_sForm;
}
