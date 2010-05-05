#include "DeviceManager.h"
#include "Console.h"

namespace DreamDDM {

DeviceManager* DeviceManager::Instance() {
	static DeviceManager instance;
	return &instance;
}

DeviceManager::~DeviceManager() {
	mDevices.DeleteAll();
	mListeners.DeleteAll();
}

bool DeviceManager::RegisterDevice(iDevice* device) {
	mDevices.Append(device);
	for (ListenerList::Iterator it = mListeners.First(); it != mListeners.End(); ++it)
		mListeners[it]->OnDriverRegistered(device);
	return true;
}

bool DeviceManager::UnregisterDevice(iDevice* device) {
	mDevices.RemoveAt(mDevices.FindFirst(device));
	for (ListenerList::Iterator it = mListeners.First(); it != mListeners.End(); ++it)
		mListeners[it]->OnDriverUnregistered(device);
	return true;
}

bool DeviceManager::RegisterListener(Listener* lsn) {
	mListeners.Append(lsn);
	for (DeviceList::Iterator it = mDevices.First(); it != mDevices.End(); ++it)
		lsn->OnDriverRegistered(mDevices[it]);
	return true;
}

bool DeviceManager::UnregisterListener(Listener* lsn) {
	mListeners.RemoveAt(mListeners.FindFirst(lsn));
	return true;
}

int DeviceManager::GetDeviceCount() {
	return mDevices.Count();
}

void DeviceManager::Print() {
	(*console) << "======== Registered devices: ================" << endl;
	for (DeviceList::Iterator it = mDevices.First(); it != mDevices.End(); ++it) {
		(*console) << mDevices[it]->GetDescription() << endl;
	}
	(*console) << "=============================================" << endl;
}

}
