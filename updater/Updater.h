#pragma once

#include <thread>
#include <atomic>
#include <map>
#include <string>
#include "GXDLMSObjectCollection.h"

class DataUpdater {
public:
    DataUpdater(CGXDLMSObjectCollection& items, int id);
    ~DataUpdater();

    void Start();
    void Stop();

    bool GetValue(const std::string& obis, double& value);

private:
    void Run();

    std::atomic<bool> m_Running;
    std::thread m_Thread;
    CGXDLMSObjectCollection& m_Items;
    int m_Id;  // Ідентифікатор лічильника для логів

    std::map<std::string, double> m_Values;
};
