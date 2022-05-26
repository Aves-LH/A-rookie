#ifndef __DEVICE_H__
#define __DEVICE_H__

class Device
{
public:
	Device() {}
	~Device() {}

	virtual void Authenticate() = 0;
	virtual void Intercom() = 0;
};

#endif
