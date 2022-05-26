#ifndef __DEVICE_MANAGER_H__
#define __DEVICE_MANAGER_H__

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "HVR.h"
#include "IPCamera.h"
#include "NVR.h"

class DeviceManager;

typedef void(*PFN)();

struct CMD_MAP
{
    std::string pCmdName;
    std::string pCmdInfo;
    PFN pFn;
};

struct DeviceInfo
{
    std::string deviceName;
    void* pDevice;
};

class DeviceManager
{
public:
	DeviceManager();
	~DeviceManager();

    int CmdProc(const CMD_MAP *pCmdMap);

    static int GetHelp(const CMD_MAP *pCmdMap);
	static void AddDevice();
	static void DeleteDevice();
	static void ModifyDevice();
	static void FindDevice();
	static void ListDevice();
	static void SortDevice();
    static void OperateDevice();
    static void Exit();

private:
    void InputDeviceInfo();

private:
    static std::map<std::string, std::map<std::string, void*>> m_deviceMap;
};

#endif
