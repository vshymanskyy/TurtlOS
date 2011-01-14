#pragma once
#include <std/List.h>
#include "iDevice.h"
#include "iDevFileSystem.h"

using namespace DreamDDM;

namespace DreamDDM {

class DeviceManager {

public:

	class Listener {
	public:
		virtual void OnDriverRegistered(iDevice*) {
		}
		virtual void OnDriverUnregistered(iDevice*) {
		}
	};

	bool RegisterDevice(iDevice* device);
	bool UnregisterDevice(iDevice* device);
	bool RegisterListener(Listener* lsn);
	bool UnregisterListener(Listener* lsn);

	List<iDevFileSystem*> GetFileSystems() const {
		List<iDevFileSystem*> result;
		for (DeviceList::It it = mDevices.First(); it != mDevices.End(); ++it) {
			if (mDevices[it]->GetType() == iDevice::DevTypeFileSystem)
				result.Append((iDevFileSystem*) mDevices[it]);
		}
		return result;
	}

	int GetDeviceCount();

	void Print();

	static DeviceManager* Instance();

	~DeviceManager();

private:
	typedef List<iDevice*> DeviceList;
	typedef List<Listener*> ListenerList;
	DeviceList mDevices;
	ListenerList mListeners;
};

}
