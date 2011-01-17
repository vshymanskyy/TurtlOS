#include "DeviceManager.h"
#include "Console.h"

namespace DreamDDM {

DeviceManager* DeviceManager::Instance() {
	static DeviceManager instance;
	return &instance;
}

DeviceManager::~DeviceManager() {
	_devices.DeleteAll();
	_listeners.DeleteAll();
}

bool DeviceManager::RegisterDevice(iDevice* device) {
	_devices.Append(device);
	for (ListenerList::It it = _listeners.First(); it != _listeners.End(); ++it)
		_listeners[it]->OnDriverRegistered(device);
	return true;
}

bool DeviceManager::UnregisterDevice(iDevice* device) {
	_devices.RemoveAt(_devices.FindFirst(device));
	for (ListenerList::It it = _listeners.First(); it != _listeners.End(); ++it)
		_listeners[it]->OnDriverUnregistered(device);
	return true;
}

bool DeviceManager::RegisterListener(Listener* lsn) {
	_listeners.Append(lsn);
	for (DeviceList::It it = _devices.First(); it != _devices.End(); ++it)
		lsn->OnDriverRegistered(_devices[it]);
	return true;
}

bool DeviceManager::UnregisterListener(Listener* lsn) {
	_listeners.RemoveAt(_listeners.FindFirst(lsn));
	return true;
}

int DeviceManager::GetDeviceQty() {
	return _devices.Count();
}

void DeviceManager::Print() {
	(*console) << "======== Registered devices: ================" << endl;
	for (DeviceList::It it = _devices.First(); it != _devices.End(); ++it) {
		(*console) << _devices[it]->GetDescription() << endl;
	}
	(*console) << "=============================================" << endl;
}

}
