// Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <map>
#include <set>
#include <algorithm>

#include "DeviceManager.h"

CMD_MAP g_CmdMap[] = { { "Add", "Add a device!", &DeviceManager::AddDevice },
                        { "Delete", "Delete a device!", &DeviceManager::DeleteDevice },
                        { "Modify", "Modify a device info!", &DeviceManager::ModifyDevice },
                        { "Find", "Find a device!", &DeviceManager::FindDevice },
                        { "List", "List devices!", &DeviceManager::ListDevice },
                        { "Sort", "Sort devices!", &DeviceManager::SortDevice },
                        { "Operate", "Operate device!", &DeviceManager::OperateDevice },
                        { "Help", "Get help info!", (PFN)&DeviceManager::GetHelp },
                        { "Exit", "Exit system!", &DeviceManager::Exit },
                        { "", "", NULL } };

int main()
{
    DeviceManager deviceManager;

    deviceManager.CmdProc(g_CmdMap);


	return 0;
}
