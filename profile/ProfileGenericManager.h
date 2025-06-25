#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <functional>
#include "GXDLMSProfileGeneric.h"
#include "GXDLMSClock.h"
#include "GXDLMSRegister.h"

class ProfileGenericManager {
public:
    ProfileGenericManager(const std::string& dataFilePath);

    CGXDLMSProfileGeneric* CreateProfileGeneric(CGXDLMSClock* clock, CGXDLMSRegister* energyRegister);
    void AppendNewProfileEntry(double energyValue);
    void StartPeriodicUpdate(std::function<double()> getEnergyValue);
    void StopPeriodicUpdate();
    int GetDataCount();
    void LoadFromFile(CGXDLMSProfileGeneric* profile);

    void GetIndicesByTimeRange(CGXDateTime& start, CGXDateTime& end, int& rowBegin, int& rowEnd);
    void Clear();

private:
    std::string m_DataFile;
    std::atomic<bool> m_Running {false};
    std::thread m_BackgroundThread;
    std::function<double()> m_GetEnergyValue;
};
