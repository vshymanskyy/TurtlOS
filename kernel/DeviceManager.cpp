#include "DeviceManager.h"
#include "Console.h"

namespace DreamDDM {

DeviceManager* DeviceManager::Instance() {
	static DeviceManager instance;
	return &instance;
}

DeviceManager::~DeviceManager() {
// TODO: _devices.DeleteAll();
	// TODO: 	_listeners.DeleteAll();
}

bool DeviceManager::RegisterDevice(iDevice* device) {
	_devices.Append(device);
	for (ListenerList::It it = _listeners.First(); it != _listeners.End(); ++it)
		_listeners[it]->OnDriverRegistered(device);
	return true;
}

bool DeviceManager::UnregisterDevice(iDevice* device) {
	DeviceList::It dev = _devices.FindAfter(_devices.First(), device);
	_devices.Remove(dev);
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
	ListenerList::It it = _listeners.FindAfter(_listeners.First(), lsn);
	_listeners.Remove(it);
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
