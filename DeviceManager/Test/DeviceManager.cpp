#include "DeviceManager.h"

#include <algorithm>

std::map<std::string, std::map<std::string, void*>> DeviceManager::m_deviceMap;

DeviceManager::DeviceManager() {

}

DeviceManager::~DeviceManager() {

}

int DeviceManager::CmdProc(const CMD_MAP *pCmdMap) {
    std::cout << "Welcome to Device Manage System!" << std::endl;
    std::cout << "The system functions are as follows: " << std::endl;
    std::cout << "1、Add 2、Delete 3、Modify 4、Find 5、List 6、Sort 7、Help 8、Exit" << std::endl;
    if (NULL != pCmdMap) {
        const CMD_MAP *pTemp = NULL;
        std::string szBuf;

        while (true) {
            std::cout << "Command> ";
            pTemp = pCmdMap;
            std::cin >> szBuf;

            while (!pTemp->pCmdName.empty()) {
                if (szBuf == pTemp->pCmdName) {
                    if ("Help" == szBuf) {
                        GetHelp(pCmdMap);
                        break;
                    }
                    pTemp->pFn();
                    break;
                }
                pTemp++;
            }

            if (NULL == pTemp || pTemp->pCmdName.empty()) {
                std::cout << "Input Error!" << std::endl;
            }
        }
    }

    return 0;
}

int DeviceManager::GetHelp(const CMD_MAP *pCmdMap) {
    if (NULL != pCmdMap) {
        const CMD_MAP *pTemp = pCmdMap;

        while (!pTemp->pCmdName.empty()) {
            std::cout << pTemp->pCmdName << "\t\t:\t\t" << pTemp->pCmdInfo << std::endl;
            pTemp++;
        }
    }

    return 0;
}

void DeviceManager::AddDevice() {
    std::cout << "Add Device!" << std::endl;

    std::string deviceType;
    std::string strTemp;

    std::cout << "Please Input Device Type> ";
    std::cin >> deviceType;

    if ("HVR" == deviceType) {
        HVR *pHVR = new HVR;
        std::cout << "Please Input Device IP> ";
        std::cin >> strTemp;
        pHVR->SetIP(strTemp);
        std::cout << "Please Input Device Port> ";
        std::cin >> strTemp;
        pHVR->SetPort(strTemp);
        std::cout << "Please Input Device MAC> ";
        std::cin >> strTemp;
        pHVR->SetMAC(strTemp);
        m_deviceMap[std::string("HVR")].insert(std::make_pair(strTemp, (void*)pHVR));
    }
    else if ("NVR" == deviceType) {
        NVR *pNVR = new NVR;
        std::cout << "Please Input Device IP> ";
        std::cin >> strTemp;
        pNVR->SetIP(strTemp);
        std::cout << "Please Input Device Port> ";
        std::cin >> strTemp;
        pNVR->SetPort(strTemp);
        std::cout << "Please Input Device MAC> ";
        std::cin >> strTemp;
        pNVR->SetMAC(strTemp);
        m_deviceMap[std::string("NVR")].insert(std::make_pair(strTemp, (void*)pNVR));
    }
    else if ("IPCamera" == deviceType) {
        IPCamera *pIPCamera = new IPCamera;
        std::cout << "Please Input Device IP> ";
        std::cin >> strTemp;
        pIPCamera->SetIP(strTemp);
        std::cout << "Please Input Device Port> ";
        std::cin >> strTemp;
        pIPCamera->SetPort(strTemp);
        std::cout << "Please Input Device MAC> ";
        std::cin >> strTemp;
        pIPCamera->SetMAC(strTemp);
        m_deviceMap[std::string("IPCamera")].insert(std::make_pair(strTemp, (void*)pIPCamera));
    }
}

void DeviceManager::DeleteDevice() {
    std::cout << "Delete Device!" << std::endl;
    if (m_deviceMap.empty()) {
        return;
    }

    std::string mac;
    std::cout << "Please Input the MAC of Device> ";
    std::cin >> mac;

    for (auto m : m_deviceMap) {
        auto itr = m.second.find(mac);
        if (itr != m.second.end()) {
            if ("HVR" == m.first) {
                m_deviceMap["HVR"].erase(itr->first);
            }
            else if ("NVR" == m.first) {
                m_deviceMap["NVR"].erase(itr->first);
            }
            else if ("IPCamera" == m.first) {
                m_deviceMap["IPCamera"].erase(itr->first);
            }
        }
    }
}

void DeviceManager::ModifyDevice() {
    std::cout << "Modify Device!" << std::endl;

    std::string mac;

    std::cout << "Please Input the MAC of Device> ";
    std::cin >> mac;

    for (auto m : m_deviceMap) {
        auto itr = m.second.find(mac);
        if (itr != m.second.end()) {
            if ("HVR" == m.first) {
                HVR *pHVR = (HVR*)itr->second;
                std::string strTemp;
                std::cout << "Please Input Device IP> ";
                std::cin >> strTemp;
                pHVR->SetIP(strTemp);
                std::cout << "Please Input Device Port> ";
                std::cin >> strTemp;
                pHVR->SetPort(strTemp);
                std::cout << "Please Input Device MAC> ";
                std::cin >> strTemp;
                pHVR->SetMAC(strTemp);
            }
            else if ("NVR" == m.first) {
                NVR *pNVR = (NVR*)itr->second;
                std::string strTemp;
                std::cout << "Please Input Device IP> ";
                std::cin >> strTemp;
                pNVR->SetIP(strTemp);
                std::cout << "Please Input Device Port> ";
                std::cin >> strTemp;
                pNVR->SetPort(strTemp);
                std::cout << "Please Input Device MAC> ";
                std::cin >> strTemp;
                pNVR->SetMAC(strTemp);
            }
            else if ("IPCamera" == m.first) {
                IPCamera *pIPCamera = (IPCamera*)itr->second;
                std::string strTemp;
                std::cout << "Please Input Device IP> ";
                std::cin >> strTemp;
                pIPCamera->SetIP(strTemp);
                std::cout << "Please Input Device Port> ";
                std::cin >> strTemp;
                pIPCamera->SetPort(strTemp);
                std::cout << "Please Input Device MAC> ";
                std::cin >> strTemp;
                pIPCamera->SetMAC(strTemp);
            }
        }
    }
}

void DeviceManager::FindDevice() {
    std::cout << "Find Device!" << std::endl;

    std::string mac;

    std::cout << "Please Input the MAC of Device> ";
    std::cin >> mac;

    for (auto m : m_deviceMap) {
        auto itr = m.second.find(mac);
        if (itr != m.second.end()) {
            std::cout << "DeviceType\t\tDeviceMAC\t\tDeviceIP\t\tDevicePort" << std::endl;
            if ("HVR" == m.first) {
                HVR *pHVR = (HVR*)itr->second;
                std::cout << "HVR\t\t\t" << pHVR->GetMAC() << "\t\t\t" << pHVR->GetIP() << "\t\t\t" << pHVR->GetPort() << std::endl;
            }
            else if ("NVR" == m.first) {
                NVR *pNVR = (NVR*)itr->second;
                std::cout << "NVR\t\t\t" << pNVR->GetMAC() << "\t\t\t" << pNVR->GetIP() << "\t\t\t" << pNVR->GetPort() << std::endl;
            }
            else if ("IPCamera" == m.first) {
                IPCamera *pIPCamera = (IPCamera*)itr->second;
                std::cout << "IPCamera\t\t" << pIPCamera->GetMAC() << "\t\t\t" << pIPCamera->GetIP() << "\t\t\t" << pIPCamera->GetPort() << std::endl;
            }
            std::cout << "*********************" << std::endl;
        }
    }
}

void DeviceManager::ListDevice() {
    std::cout << "List Device!" << std::endl;
    std::cout << "DeviceType\t\tDeviceMAC\t\tDeviceIP\t\tDevicePort" << std::endl;
    for (auto m : m_deviceMap) {
        for (auto n : m.second) {
            if ("HVR" == m.first) {
                std::cout << "HVR\t\t\t" << ((HVR*)n.second)->GetMAC() << "\t\t\t" << ((HVR*)n.second)->GetIP() << "\t\t\t" << ((HVR*)n.second)->GetPort() << std::endl;
            }
            else if ("NVR" == m.first) {
                std::cout << "NVR\t\t\t" << ((NVR*)n.second)->GetMAC() << "\t\t\t" << ((NVR*)n.second)->GetIP() << "\t\t\t" << ((NVR*)n.second)->GetPort() << std::endl;
            }
            else if ("IPCamera" == m.first) {
                std::cout << "IPCamera\t\t" << ((IPCamera*)n.second)->GetMAC() << "\t\t\t" << ((IPCamera*)n.second)->GetIP() << "\t\t\t" << ((IPCamera*)n.second)->GetPort() << std::endl;
            }
            std::cout << "*********************" << std::endl;
        }
    }
}

void DeviceManager::SortDevice() {
    std::cout << "Sort Device!" << std::endl;

    std::vector<std::string> vec;

    for (auto m : m_deviceMap) {
        for (auto n : m.second) {
            vec.push_back(n.first);
        }
    }
    
    std::sort(vec.begin(), vec.end());

    std::cout << "DeviceType\t\tDeviceMAC\t\tDeviceIP\t\tDevicePort" << std::endl;
    for (auto m : m_deviceMap) {
        for (size_t i = 0; i < vec.size(); i++) {
            auto itr = m.second.find(vec[i]);
            if (itr != m.second.end()) {
                if ("HVR" == m.first) {
                    HVR *pHVR = (HVR*)itr->second;
                    std::cout << "HVR\t\t\t" << pHVR->GetMAC() << "\t\t\t" << pHVR->GetIP() << "\t\t\t" << pHVR->GetPort() << std::endl;
                }
                else if ("NVR" == m.first) {
                    NVR *pNVR = (NVR*)itr->second;
                    std::cout << "NVR\t\t\t" << pNVR->GetMAC() << "\t\t\t" << pNVR->GetIP() << "\t\t\t" << pNVR->GetPort() << std::endl;
                }
                else if ("IPCamera" == m.first) {
                    IPCamera *pIPCamera = (IPCamera*)itr->second;
                    std::cout << "IPCamera\t\t" << pIPCamera->GetMAC() << "\t\t\t" << pIPCamera->GetIP() << "\t\t\t" << pIPCamera->GetPort() << std::endl;
                }
                std::cout << "*********************" << std::endl;
            }
        }
    }
}

void DeviceManager::OperateDevice() {
    std::cout << "Operate Device!" << std::endl;

    std::string mac;

    std::cout << "Please Input the MAC of Device> ";
    std::cin >> mac;

    for (auto m : m_deviceMap) {
        auto itr = m.second.find(mac);
        if (itr != m.second.end()) {
            if ("HVR" == m.first) {
                std::cout << "The type of this device is HVR!" << std::endl;
                std::cout << "The functions of the HVR device are as follows: " << std::endl;
                std::cout << "1、Authenticate 2、Intercom 3、MultiChannelRTC 4、Replay" << std::endl;

                int operation;
                std::cout << "Please Input Operation(1, 2, 3, 4)> ";
                std::cin >> operation;

                HVR *pHVR = (HVR*)itr->second;

                switch (operation) {
                case 1:
                    pHVR->Authenticate();
                    break;
                case 2:
                    pHVR->Intercom();
                    break;
                case 3:
                    pHVR->MultiChannelRTC();
                    break;
                case 4:
                    pHVR->Replay();
                    break;
                default:
                    break;
                }
            }
            else if ("NVR" == m.first) {
                std::cout << "The type of this device is NVR!" << std::endl;
                std::cout << "The functions of the NVR device are as follows: " << std::endl;
                std::cout << "1、Authenticate 2、Intercom 3、MultiChannelRTC 4、Replay" << std::endl;

                int operation;
                std::cout << "Please Input Operation(1, 2, 3, 4)> ";
                std::cin >> operation;

                NVR *pNVR = (NVR*)itr->second;

                switch (operation) {
                case 1:
                    pNVR->Authenticate();
                    break;
                case 2:
                    pNVR->Intercom();
                    break;
                case 3:
                    pNVR->MultiChannelRTC();
                    break;
                case 4:
                    pNVR->Replay();
                    break;
                default:
                    break;
                }
            }
            else if ("IPCamera" == m.first) {
                std::cout << "The type of this device is IPCamera!" << std::endl;
                std::cout << "The functions of the IPCamera device are as follows: " << std::endl;
                std::cout << "1、Authenticate 2、Intercom 3、SingleChannelRTC 4、CloudStorage" << std::endl;

                int operation;
                std::cout << "Please Input Operation(1, 2, 3, 4)> ";
                std::cin >> operation;

                IPCamera *pIPCamera = (IPCamera*)itr->second;

                switch (operation) {
                case 1:
                    pIPCamera->Authenticate();
                    break;
                case 2:
                    pIPCamera->Intercom();
                    break;
                case 3:
                    pIPCamera->SingleChannelRTC();
                    break;
                case 4:
                    pIPCamera->CloudStorage();
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void DeviceManager::Exit() {
    exit(0);
}
