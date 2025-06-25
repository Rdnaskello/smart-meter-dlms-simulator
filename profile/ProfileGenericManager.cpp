#include "ProfileGenericManager.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <atomic>
#include <functional>

ProfileGenericManager::ProfileGenericManager(const std::string& dataFilePath)
    : m_DataFile(dataFilePath)
{
}

CGXDLMSProfileGeneric* ProfileGenericManager::CreateProfileGeneric(CGXDLMSClock* clock, CGXDLMSRegister* energyRegister) {
    auto* profile = new CGXDLMSProfileGeneric("1.0.99.1.0.255");
    profile->SetCapturePeriod(60);
    profile->SetSortMethod(DLMS_SORT_METHOD_FIFO);
    profile->SetSortObject(clock);
    profile->SetAccess(2, DLMS_ACCESS_MODE_READ);
    profile->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_ARRAY));

    if (clock && energyRegister) {
        profile->GetCaptureObjects().push_back({clock, new CGXDLMSCaptureObject(2, 0)});
        profile->GetCaptureObjects().push_back({energyRegister, new CGXDLMSCaptureObject(2, 0)});
        std::cout << "[ProfileGenericManager] CaptureObjects додано успішно." << std::endl;
    }

    // Створення файлу, якщо не існує
    FILE* f = fopen(m_DataFile.c_str(), "r");
    if (!f) {
        f = fopen(m_DataFile.c_str(), "w");
        if (f) {
            CGXDateTime now = CGXDateTime::Now();
            char buffer[32];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &now.GetValue());
            double energy = energyRegister ? energyRegister->GetValue().ToDouble() : 0.0;
            fprintf(f, "%s;%.6f\n", buffer, energy);
            fclose(f);
            std::cout << "[ProfileGenericManager] Створено файл " << m_DataFile << " з початковим записом." << std::endl;
        }
    } else {
        fclose(f);
    }

    // Підрахунок рядків
    int count = GetDataCount();
    profile->SetEntriesInUse(count);
    profile->SetProfileEntries(count);

    std::cout << "[ProfileGenericManager] EntriesInUse = " << count << std::endl;

    return profile;
}


int ProfileGenericManager::GetDataCount() {
    int count = 0;
    FILE* f = fopen(m_DataFile.c_str(), "r");
    if (f) {
        char line[128];
        while (fgets(line, sizeof(line), f)) {
            ++count;
        }
        fclose(f);
    }
    return count;
}

void ProfileGenericManager::AppendNewProfileEntry(double energyValue) {
    FILE* f = fopen(m_DataFile.c_str(), "a");
    if (!f) {
        std::cerr << "[ProfileGenericManager] ❌ Не вдалося відкрити файл для запису: " << m_DataFile << std::endl;
        return;
    }

    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

    fprintf(f, "%s;%.6f\n", buffer, energyValue);
    fclose(f);

    std::cout << "[ProfileGenericManager] ➕ Додано рядок: " << buffer << "; " << energyValue << std::endl;
}

void ProfileGenericManager::StartPeriodicUpdate(std::function<double()> getEnergyValue) {
    if (m_Running) return;
    m_GetEnergyValue = getEnergyValue;
    m_Running = true;
    m_BackgroundThread = std::thread([this]() {
        while (m_Running) {
            if (m_GetEnergyValue) {
                double energy = m_GetEnergyValue();
                std::cout << "[DEBUG] Потік живий, енергія: " << energy << std::endl;
                AppendNewProfileEntry(energy);
            }
            std::this_thread::sleep_for(std::chrono::seconds(10)); // Змінюй інтервал, як потрібно
        }
    });
    std::cout << "[ProfileGenericManager] 🟢 Потік генерації даних запущено" << std::endl;
}



void ProfileGenericManager::StopPeriodicUpdate() {
    m_Running = false;
    if (m_BackgroundThread.joinable()) {
        m_BackgroundThread.join();
        std::cout << "[ProfileGenericManager] 🛑 Потік генерації даних зупинено" << std::endl;
    }
}


void ProfileGenericManager::LoadFromFile(CGXDLMSProfileGeneric* profile) {
    if (!profile) return;

    profile->GetBuffer().clear(); // Очищаємо старі дані

    FILE* f = fopen(m_DataFile.c_str(), "r");
    if (!f) {
        std::cerr << "[ProfileGenericManager] ❌ Не вдалося відкрити файл для читання: " << m_DataFile << std::endl;
        return;
    }

    char line[128];
    int year, month, day, hour, minute, second;
    double energy;
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d.%d.%d %d:%d:%d;%lf", &day, &month, &year, &hour, &minute, &second, &energy) == 7) {
            CGXDateTime dt(year, month, day, hour, minute, second, 0, 0x8000);
            std::vector<CGXDLMSVariant> row;
            row.push_back(dt);
            row.push_back(energy);
            profile->GetBuffer().push_back(row);
        }
    }

    fclose(f);

    // Оновлюємо EntriesInUse і ProfileEntries
    int count = profile->GetBuffer().size();
    profile->SetEntriesInUse(count);
    profile->SetProfileEntries(count);

    std::cout << "[ProfileGenericManager] 📥 Завантажено " << count << " рядків з файлу " << m_DataFile << std::endl;
}


void ProfileGenericManager::GetIndicesByTimeRange(CGXDateTime& start, CGXDateTime& end, int& rowBegin, int& rowEnd) {
    rowBegin = -1;
    rowEnd = -1;

    FILE* f = fopen(m_DataFile.c_str(), "r");
    if (!f) {
        std::cerr << "[ProfileGenericManager] ❌ Не вдалося відкрити файл для читання: " << m_DataFile << std::endl;
        return;
    }

    char line[128];
    int year, month, day, hour, minute, second;
    double energy;
    int index = 1; // DLMS рядки нумеруються з 1

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d-%d-%d %d:%d:%d;%lf", &year, &month, &day, &hour, &minute, &second, &energy) == 7) {
            CGXDateTime timestamp(year, month, day, hour, minute, second, 0, 0x8000);
            if (timestamp.CompareTo(start) >= 0 && rowBegin == -1) {
                rowBegin = index;
            }
            if (timestamp.CompareTo(end) <= 0) {
                rowEnd = index;
            }
        }
        index++;
    }

    fclose(f);

    if (rowBegin == -1) rowBegin = 1;
    if (rowEnd == -1) rowEnd = index - 1;

    std::cout << "[ProfileGenericManager] 🌙 Діапазон рядків: " << rowBegin << " - " << rowEnd << std::endl;
}

void ProfileGenericManager::Clear() {
    FILE* f = fopen(m_DataFile.c_str(), "w");
    if (f) {
        fclose(f);
        std::cout << "[ProfileGenericManager] ⚠️ Файл профілю очищено: " << m_DataFile << std::endl;
    } else {
        std::cerr << "[ProfileGenericManager] ❌ Не вдалося очистити файл: " << m_DataFile << std::endl;
    }
}