#pragma once

class ImpsParser {

public:
	static ImpsParser* Instance();
	size_t GetLapicAddr();

private:
	ImpsParser();
	~ImpsParser();

	void AddCPU(struct cteCPU *cpu);
	void AddBUS(struct cteBUS *bus);
	void AddIOAPIC(struct cteIOAPIC *ioapic);
	void AddINT(struct cteINT *interr);
	void AddLINT(struct cteINT *interr);

private:
	struct FPS*			_fps;
	struct ConfigTable*	_configTable;
	size_t				_lapicAddr;
};
