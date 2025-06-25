// ObisObjectModel.cpp
#include "ObisObjectModel.h"
#include "GXDLMSClock.h"
#include "GXDLMSRegister.h"
#include "GXDLMSData.h"
#include "GXDLMSAssociationLogicalName.h"
#include <cstdio> // Для sprintf

ObisObjectModel::ObisObjectModel() {}

ObisObjectModel::~ObisObjectModel() {}

// === Головна функція для додавання об'єктів ===
void ObisObjectModel::AddBasicTestObisObjects(CGXDLMSObjectCollection& items, unsigned long serialNumber) {
    Group_Identification(items, serialNumber);
    Group_EnergyRegisters(items);
    Group_TariffRegisters(items);
    Group_InstantaneousValues(items);
    Group_ClockObjects(items);
    Group_SystemParameters(items);
}

// === Групи об'єктів ===
void ObisObjectModel::Group_Identification(CGXDLMSObjectCollection& items, unsigned long serialNumber) {
    AddLogicalDeviceName(items, serialNumber);
}

void ObisObjectModel::Group_EnergyRegisters(CGXDLMSObjectCollection& items) {
    AddActiveEnergyL1(items);
    AddActiveEnergyL2(items);
    AddActiveEnergyL3(items);
    AddActiveEnergyTotal(items);
}

void ObisObjectModel::Group_TariffRegisters(CGXDLMSObjectCollection& items) {
    // TODO: Додати 1.8.1, 1.8.2, 1.8.3, 1.8.4...
}

void ObisObjectModel::Group_InstantaneousValues(CGXDLMSObjectCollection& items) {
    // TODO: Додати поточні значення: напруга, струм, потужність...
}

void ObisObjectModel::Group_ClockObjects(CGXDLMSObjectCollection& items) {
    AddClockObject(items);
}

void ObisObjectModel::Group_SystemParameters(CGXDLMSObjectCollection& items) {
    // TODO: Додати об'єкти системних параметрів...
}

// === Окремі об'єкти ===
CGXDLMSAssociationLogicalName* ObisObjectModel::AddAssociationLN(CGXDLMSObjectCollection& items, int clientSAP, int serverSAP) {
    auto* association = new CGXDLMSAssociationLogicalName("0.0.40.0.0.255");
    association->SetClientSAP(clientSAP);
    association->SetServerSAP(serverSAP);
    items.push_back(association);
    return association;
}

CGXDLMSClock* ObisObjectModel::AddClockObject(CGXDLMSObjectCollection& items) {
    m_pClock = new CGXDLMSClock("0.0.1.0.0.255");
    items.push_back(m_pClock);
    return m_pClock;
}


CGXDLMSData* ObisObjectModel::AddLogicalDeviceName(CGXDLMSObjectCollection& items, unsigned long sn) {
    char buff[17];
    sprintf(buff, "NIK%.13lu", sn);
    CGXDLMSVariant id(buff);
    auto* ldn = new CGXDLMSData("0.0.42.0.0.255");
    ldn->SetValue(id);
    items.push_back(ldn);
    return ldn;
}

CGXDLMSRegister* ObisObjectModel::AddActiveEnergyL1(CGXDLMSObjectCollection& items) {
    auto* reg = new CGXDLMSRegister("1.0.21.8.0.255");
    reg->SetScaler(-3);
    reg->SetUnit(DLMS_UNIT_ACTIVE_ENERGY);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    reg->GetAttributes().push_back(CGXDLMSAttribute(3, DLMS_DATA_TYPE_STRUCTURE));
    reg->SetAccess(3, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
    return reg;
}

CGXDLMSRegister* ObisObjectModel::AddActiveEnergyL2(CGXDLMSObjectCollection& items) {
    auto* reg = new CGXDLMSRegister("1.0.41.8.0.255");
    reg->SetScaler(-3);
    reg->SetUnit(DLMS_UNIT_ACTIVE_ENERGY);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    reg->GetAttributes().push_back(CGXDLMSAttribute(3, DLMS_DATA_TYPE_STRUCTURE));
    reg->SetAccess(3, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
    return reg;
}

CGXDLMSRegister* ObisObjectModel::AddActiveEnergyL3(CGXDLMSObjectCollection& items) {
    auto* reg = new CGXDLMSRegister("1.0.61.8.0.255");
    reg->SetScaler(-3);
    reg->SetUnit(DLMS_UNIT_ACTIVE_ENERGY);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    reg->GetAttributes().push_back(CGXDLMSAttribute(3, DLMS_DATA_TYPE_STRUCTURE));
    reg->SetAccess(3, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
    return reg;
}

CGXDLMSRegister* ObisObjectModel::AddActiveEnergyTotal(CGXDLMSObjectCollection& items) {
    m_pEnergyRegister = new CGXDLMSRegister("1.0.1.8.0.255");
    m_pEnergyRegister->SetScaler(-3);
    m_pEnergyRegister->SetUnit(DLMS_UNIT_ACTIVE_ENERGY);
    m_pEnergyRegister->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    m_pEnergyRegister->SetAccess(2, DLMS_ACCESS_MODE_READ);
    m_pEnergyRegister->GetAttributes().push_back(CGXDLMSAttribute(3, DLMS_DATA_TYPE_STRUCTURE));
    m_pEnergyRegister->SetAccess(3, DLMS_ACCESS_MODE_READ);
    items.push_back(m_pEnergyRegister);
    return m_pEnergyRegister;
}


// 3. Індикація унікального серійного номера лічильника (96.1.0)
CGXDLMSData* ObisObjectModel::SetSerialNumber(CGXDLMSObjectCollection& items)
{
    char buff[17];
    unsigned long sn = 1234567890123;
    sprintf(buff, "NIK%.13lu", sn);
    CGXDLMSVariant id;
    id.Add((const char*)buff, 16);
    CGXDLMSData* serial = new CGXDLMSData("0.0.96.1.0.255");
    serial->SetValue(id);
    serial->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_STRING));
    serial->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(serial);
}

// 4. Індикація типу лічильника (96.1.1)
CGXDLMSData* ObisObjectModel::SetMeterType(CGXDLMSObjectCollection& items)
{
    CGXDLMSData* meterType = new CGXDLMSData("0.0.96.1.1.255");
    CGXDLMSVariant meterTypeValue;
    meterTypeValue.Add("NIK 2303 AP");
    meterType->SetValue(meterTypeValue); // або інший тип
    meterType->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_STRING));
    meterType->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(meterType);    
}

// 5. Індикація версії програмного забезпечення лічильника (96.1.10)
CGXDLMSData* ObisObjectModel::SetFirmwareVersion(CGXDLMSObjectCollection& items)
{
    CGXDLMSData* fwVersion = new CGXDLMSData("0.0.96.1.10.255");
    CGXDLMSVariant fwValue;
    fwValue.Add("FW 0.0.1"); // Або будь-яке інше значення прошивки
    fwVersion->SetValue(fwValue);
    fwVersion->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_STRING));
    fwVersion->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(fwVersion);
}

// 6. Індикація контрольної суми ПЗ (96.1.11)
CGXDLMSData* ObisObjectModel::SetChecksum(CGXDLMSObjectCollection& items)
{
    CGXDLMSData* fwChecksum = new CGXDLMSData("0.0.96.1.11.255");
    CGXDLMSVariant checksumValue;
    checksumValue.Add("A1B2C3D4"); // Або інша контрольна сума
    fwChecksum->SetValue(checksumValue);
    fwChecksum->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_STRING));
    fwChecksum->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(fwChecksum); 
}

// 7. Коєфіцієнт трансформації струму (0.4.2)
CGXDLMSData* ObisObjectModel::SetCurrentTransformerRatio(CGXDLMSObjectCollection& items)
{
    CGXDLMSData* currentTransformerRatio = new CGXDLMSData("0.0.4.2.0.255");
    CGXDLMSVariant ratioValue(300); // Наприклад, трансформація 300:5 → коефіцієнт 60
    currentTransformerRatio->SetValue(ratioValue);
    currentTransformerRatio->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_UINT16));
    currentTransformerRatio->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(currentTransformerRatio);
}

// 8. Коєфіцієнт трансформації напруги (0.4.3)
CGXDLMSData* ObisObjectModel::SetVoltageTransformCoeff(CGXDLMSObjectCollection& items)
{
    CGXDLMSData* voltageTransformCoeff = new CGXDLMSData("0.0.4.3.0.255");
    CGXDLMSVariant vtValue(1); // наприклад, 1 або інший сталий коефіцієнт
    voltageTransformCoeff->SetValue(vtValue);
    voltageTransformCoeff->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_UINT16));
    voltageTransformCoeff->SetAccess(2, DLMS_ACCESS_MODE_READ); // тільки для читання
    items.push_back(voltageTransformCoeff);    
}


// 13. Активна енергія по тарифу, кВт ∙ год (n – від 1 по 4) 1.8.n (1.8.1)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyImportTariff1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.1.8.1.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

//(1.8.2)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyImportTariff2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.1.8.2.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

//(1.8.3)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyImportTariff3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.1.8.3.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

//(1.8.4)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyImportTariff4(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.1.8.4.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 14. Активна енергія (2 + 3 квадрант), кВт ∙ год (2.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyExportTotal(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.2.8.0.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);    
}

// 15. Активна енергія (2 + 3 квадрант) фази L1, кВт ∙ год (22.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyExportL1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.22.8.0.255");
    CGXDLMSVariant value = (0.1); // генерація значення
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 16. Активна енергія (2 + 3 квадрант) фази L2, кВт ∙ год (42.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyExportL2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.42.8.0.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 17. Активна енергія (2 + 3 квадрант) фази L3, кВт ∙ год (62.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyExportL3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.62.8.0.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 18. Активна енергія по n-му тарифу, кВт ∙ год (n – від 1 по 4) 2.8.n (2.8.1 - 2.8.4)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyExportTariff1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.2.8.1.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
//(2.8.2)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyExportTariff2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.2.8.2.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
//(2.8.3)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyExportTariff3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.2.8.3.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
//(2.8.4)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyExportTariff4(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.2.8.3.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 19. Реактивна енергія (1 + 2 квадрант), квар ∙ год (3.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImport_3_8_0(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.3.8.0.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 20. Реактивна енергія (1 + 2 квадрант) фази L1, квар ∙ год (23.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImport_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.23.8.0.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 21. Реактивна енергія (1 + 2 квадрант) фази L2, квар ∙ год (43.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImport_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.43.8.0.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 22. Реактивна енергія (1 + 2 квадрант) фази L3, квар ∙ год (63.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImport_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.63.8.0.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 23. Реактивна енергія по n-му тарифу, квар ∙ год (n – від 1 по 4) 3.8.n (3.8.1 - 3.8.4)
// 3.8.1
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImportTariff1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.3.8.1.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 3.8.2
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImportTariff2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.3.8.2.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 3.8.3
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImportTariff3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.3.8.3.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 3.8.4
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImportTariff4(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.3.8.4.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 24. Реактивна енергія (2 + 3 квадрант), квар ∙ год (4.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyExport_4_8_0(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.4.8.0.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 25. Реактивна енергія (2 + 3 квадрант) фази L1, квар ∙ год (24.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyExport_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.24.8.0.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 26. Реактивна енергія (2 + 3 квадрант) фази L2, квар ∙ год (44.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyExport_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.44.8.0.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 27. Реактивна енергія (2 + 3 квадрант) фази L3, квар ∙ год (64.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyExport_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.64.8.0.255");
    CGXDLMSVariant value = (0.1);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 28. Реактивна енергія по n-му тарифу, квар ∙ год (n – від 1 по 4) 4.8.n (4.8.1 - 4.8.4)
// 4.8.1
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyTariff1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.4.8.1.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 4.8.2
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyTariff2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.4.8.2.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 4.8.3
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyTariff3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.4.8.3.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 4.8.4
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyTariff4(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.4.8.4.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 29 Реактивна енергія (1 квадрант) для А +, квар ∙ год (5.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImportQ1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.5.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 30. Реактивна енергія (1 квадрант) для А + фази L1, квар ∙ год (25.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImportQ1_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.25.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 31. Реактивна енергія (1 квадрант) для А + фази L2, квар ∙ год (45.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImportQ1_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.45.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 32. Реактивна енергія (1 квадрант) для А + фази L3, квар ∙ год (65.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImportQ1_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.65.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 33. Реактивна енергія (1 квадрант) для А + по n-му тарифу, квар ∙ год (n – від 1 по 4) 5.8.n (5.8.1 - 5.8.4)
// 5.8.1
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyQ1_Tariff1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.3.8.1.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 5.8.2
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyQ1_Tariff2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.3.8.2.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 5.8.3
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyQ1_Tariff3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.3.8.3.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 5.8.4
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyQ1_Tariff4(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.3.8.4.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 34. Реактивна енергія (2 квадрант) для А -, квар ∙ год (6.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyExport_Q2_Total(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.6.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 35. Реактивна енергія (2 квадрант) для А -, квар ∙ год (26.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyExport_Q2_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.26.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 36. Реактивна енергія (2 квадрант) для А -, квар ∙ год (46.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyExport_Q2_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.46.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 37. Реактивна енергія (2 квадрант) для А -, квар ∙ год (66.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyExport_Q2_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.66.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 38. Реактивна енергія (2 квадрант) для А - по n-му тарифу, квар ∙ год (n – від 1 по 4) 6.8.n (6.8.1 - 6.8.4)
// 6.8.1
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyExport_Q2_Tariff1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.6.8.1.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 6.8.2
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyExport_Q2_Tariff2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.6.8.2.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 6.8.3
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyExport_Q2_Tariff3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.6.8.3.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 6.8.4
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyExport_Q2_Tariff4(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.6.8.4.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 39. Реактивна енергія (3 квадрант) для А -, квар ∙ год (7.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImportQ3_Aminus(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.7.8.0.255");
    CGXDLMSVariant value = (0); 
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 40. Реактивна енергія (3 квадрант) для А - фази L1, квар ∙ год (27.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyQ3_Aminus_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.27.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 41. Реактивна енергія (3 квадрант) для А - фази L2, квар ∙ год (47.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyQ3_Aminus_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.47.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 42. Реактивна енергія (3 квадрант) для А - фази L3, квар ∙ год (67.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyQ3_Aminus_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.67.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 43. Реактивна енергія (3 квадрант) для А - по n-му тарифу, квар ∙ год (n – від 1 по 4) 7.8.n (7.8.1 - 7.8.4)
// 7.8.1
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImport_Q3_Tariff1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.7.8.1.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 7.8.2
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImport_Q3_Tariff2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.7.8.2.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 7.8.3
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImport_Q3_Tariff3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.7.8.3.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 7.8.4
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImport_Q3_Tariff4(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.7.8.4.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 44. Реактивна енергія (4 квадрант) для А +, квар ∙ год (8.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImportQ4_Aplus(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.8.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 45. Реактивна енергія (4 квадрант) для А + фази L1, квар ∙ год (28.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImportQ4_Aplus_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.28.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 46. Реактивна енергія (4 квадрант) для А + фази L2, квар ∙ год (48.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImportQ4_Aplus_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.48.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 47. Реактивна енергія (4 квадрант) для А + фази L3, квар ∙ год (68.8.0)
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImportQ4_Aplus_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.68.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 48. Реактивна енергія (4 квадрант) для А + по n-му тарифу, квар ∙ год (n – від 1 по 4) 8.8.n (8.8.1 - 8.8.4)
// 8.8.1
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImport_Q4_Tariff1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.8.8.1.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 8.8.2
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImport_Q4_Tariff2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.8.8.2.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 8.8.3
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImport_Q4_Tariff3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.8.8.3.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 8.8.4
CGXDLMSRegister* ObisObjectModel::SetReactiveEnergyImport_Q4_Tariff4(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.8.8.4.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 49. Повна енергія S+ (1 +4 квадранти), кВА ∙ год (9.8.0)
CGXDLMSRegister* ObisObjectModel::SetApparentEnergySplus(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.9.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 50. Повна енергія S+ (1 +4 квадранти) фази L1, кВА ∙ год (29.8.0)
CGXDLMSRegister* ObisObjectModel::SetApparentEnergySplus_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.29.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 51. Повна енергія S+ (1 +4 квадранти) фази L2, кВА ∙ год (49.8.0)
CGXDLMSRegister* ObisObjectModel::SetApparentEnergySplus_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.49.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 52. Повна енергія S+ (1 +4 квадранти) фази L3, кВА ∙ год (69.8.0)
CGXDLMSRegister* ObisObjectModel::SetApparentEnergySplus_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.69.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 53. Повна енергія S- (2 +3 квадранти), кВА ∙ год (10.8.0)
CGXDLMSRegister* ObisObjectModel::SetApparentEnergySminus(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.10.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 54. Повна енергія S- (2 +3 квадранти) фази L1, кВА ∙ год (30.8.0)
CGXDLMSRegister* ObisObjectModel::SetApparentEnergySminus_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.30.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 55. Повна енергія S- (2 +3 квадранти) фази L2, кВА ∙ год (50.8.0)
CGXDLMSRegister* ObisObjectModel::SetApparentEnergySminus_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.50.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 56. Повна енергія S- (2 +3 квадранти) фази L3, кВА ∙ год (70.8.0)
CGXDLMSRegister* ObisObjectModel::SetApparentEnergySminus_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.70.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 57. Активна енергія |A+|+|A-|, кВт ∙ год (15.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyAbsSum(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.15.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 58. Активна енергія |A+|+|A-| фази L1, кВт ∙ год (35.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyAbs_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.35.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 59. Активна енергія |A+|+|A-| фази L2, кВт ∙ год (55.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyAbs_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.55.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 60. Активна енергія |A+|+|A-| фази L3, кВт ∙ год (75.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyAbs_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.75.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 61. Активна енергія |A+|+|A-| по n-му тарифу, кВт ∙ год (n – від 1 до 4) 15.8.n (15.8.1-15.8.4)
// 15.8.1
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyAbs_Tariff1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.15.8.1.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 15.8.2
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyAbs_Tariff2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.15.8.2.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 15.8.3
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyAbs_Tariff3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.15.8.3.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 15.8.4
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyAbs_Tariff4(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.15.8.4.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 62. Активна енергія |A+|-|A-|, кВт ∙ год (16.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyAbsDiff(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.16.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 63. Активна енергія |A+|-|A-| фази L1, кВт ∙ год (36.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyAbsDiff_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.36.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 64. Активна енергія |A+|-|A-| фази L2, кВт ∙ год (56.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyAbsDiff_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.56.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 65. Активна енергія |A+|-|A-| фази L3, кВт ∙ год (76.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyAbsDiff_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.76.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 66. Активна енергія |A+|-|A-| по n-му тарифу, кВт ∙ год (n – від 1 до 4) 16.8.n (16.8.1-16.8.4)
// 16.8.1
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyAbsDiff_Tariff1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.16.8.1.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 16.8.2
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyAbsDiff_Tariff2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.16.8.2.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 16.8.3
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyAbsDiff_Tariff3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.16.8.3.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 16.8.4
CGXDLMSRegister* ObisObjectModel::SetActiveEnergyAbsDiff_Tariff4(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.16.8.4.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 67. Активна енергія (1 квадрант), кВт ∙ год (17.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergy_Q1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.17.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 68. Активна енергія (1 квадрант) фази L1, кВт ∙ год (37.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergy_Q1_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.37.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 69. Активна енергія (1 квадрант) фази L2, кВт ∙ год (57.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergy_Q1_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.57.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 70. Активна енергія (1 квадрант) фази L3, кВт ∙ год (77.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergy_Q1_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.77.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 71. Активна енергія (2 квадрант), кВт ∙ год (18.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergy_Q2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.18.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 72. Активна енергія (2 квадрант) фази L1, кВт ∙ год (38.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergy_Q2_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.38.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 73. Активна енергія (2 квадрант) фази L2, кВт ∙ год (58.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergy_Q2_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.58.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 74. Активна енергія (2 квадрант) фази L3, кВт ∙ год (78.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergy_Q2_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.78.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 75. Активна енергія (3 квадрант), кВт ∙ год (19.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergy_Q3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.19.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 76. Активна енергія (3 квадрант) фази L1, кВт ∙ год (39.8.0)
CGXDLMSRegister* ObisObjectModel:: SetActiveEnergy_Q3_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.39.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 77. Активна енергія (3 квадрант) фази L2, кВт ∙ год (59.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergy_Q3_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.59.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 78. Активна енергія (3 квадрант) фази L3, кВт ∙ год (79.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergy_Q3_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.79.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 79. Активна енергія (4 квадрант), кВт ∙ год (20.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergy_Q4(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.20.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 80. Активна енергія (4 квадрант) фази L1, кВт ∙ год (40.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergy_Q4_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.40.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 81. Активна енергія (4 квадрант) фази L2, кВт ∙ год (60.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergy_Q4_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.60.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 82. Активна енергія (4 квадрант) фази L3, кВт ∙ год (80.8.0)
CGXDLMSRegister* ObisObjectModel::SetActiveEnergy_Q4_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.80.8.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 83. Миттєва активна потужність (1+4 квадранти), кВт (1.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantActivePowerImport(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.1.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 84. Миттєва активна потужність (1+4 квадранти) фази L1, кВт (21.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantActivePowerImport_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.21.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 85. Миттєва активна потужність (1+4 квадранти) фази L2, кВт (41.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantActivePowerImport_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.41.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 86. Миттєва активна потужність (1+4 квадранти) фази L3, кВт (61.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantActivePowerImport_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.61.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 87. Миттєва активна потужність (2+3 квадранти), кВт (2.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantActivePowerExport(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.2.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 88. Миттєва активна потужність (2+3 квадранти) фази L1, кВт (22.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantActivePowerExport_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.22.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 89. Миттєва активна потужність (2+3 квадранти) фази L2, кВт (42.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantActivePowerExport_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.42.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 90. Миттєва активна потужність (2+3 квадранти) фази L3, кВт (62.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantActivePowerExport_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.62.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 91. Миттєва реактивна потужність (1+2 квадранти), квар (3.7.0)
CGXDLMSRegister* ObisObjectModel:: SetInstantReactivePowerImport(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.3.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 92. Миттєва реактивна потужність (1+2 квадранти) фази L1, квар (23.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantReactivePowerImport_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.23.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 93. Миттєва реактивна потужність (1+2 квадранти) фази L2, квар (43.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantReactivePowerImport_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.43.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 94. Миттєва реактивна потужність (1+2 квадранти) фази L3, квар (63.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantReactivePowerImport_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.63.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 95. Миттєва реактивна потужність (3+4 квадранти), квар (4.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantReactivePowerExport(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.4.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 96. Миттєва реактивна потужність (3+4 квадранти) фази L1, квар (24.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantReactivePowerExport_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.24.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 97. Миттєва реактивна потужність (3+4 квадранти) фази L2, квар (44.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantReactivePowerExport_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.44.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 98. Миттєва реактивна потужність (3+4 квадранти) фази L3, квар (64.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantReactivePowerExport_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.64.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 99. Миттєва реактивна потужність по n-му квадранту, квар n.7.0 (5.7.0-8.7.0)
// 5.7.0
CGXDLMSRegister* ObisObjectModel:: SetInstantReactivePower_Q1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.5.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 6.7.0
CGXDLMSRegister* ObisObjectModel::SetInstantReactivePower_Q2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.6.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 7.7.0
CGXDLMSRegister* ObisObjectModel::SetInstantReactivePower_Q3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.7.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 8.7.0
CGXDLMSRegister* ObisObjectModel::SetInstantReactivePower_Q4(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.8.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 100. Миттєва реактивна потужність по n-му квадранту фази L1, квар n.7.0 (25.7.0-28.7.0)
// 25.7.0
CGXDLMSRegister* ObisObjectModel:: SetInstantReactivePower_Q1_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.25.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 26.7.0
CGXDLMSRegister* ObisObjectModel:: SetInstantReactivePower_Q2_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.26.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 27.7.0
CGXDLMSRegister* ObisObjectModel::SetInstantReactivePower_Q3_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.27.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 28.7.0
CGXDLMSRegister* ObisObjectModel::SetInstantReactivePower_Q4_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.28.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 101. Миттєва реактивна потужність по n-му квадранту фази L2, квар n.7.0 (45.7.0-48.7.0)
// 45.7.0
CGXDLMSRegister* ObisObjectModel::SetInstantReactivePower_Q1_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.45.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 46.7.0
CGXDLMSRegister* ObisObjectModel::SetInstantReactivePower_Q2_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.46.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 47.7.0
CGXDLMSRegister* ObisObjectModel:: SetInstantReactivePower_Q3_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.47.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 48.7.0
CGXDLMSRegister* ObisObjectModel:: SetInstantReactivePower_Q4_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.48.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 102. Миттєва реактивна потужність по n-му квадранту фази L3, квар n.7.0 (65.7.0-68.7.0)
// 65.7.0
CGXDLMSRegister* ObisObjectModel::SetInstantReactivePower_Q1_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.65.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 66.7.0
CGXDLMSRegister* ObisObjectModel::SetInstantReactivePower_Q2_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.66.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 67.7.0
CGXDLMSRegister* ObisObjectModel:: SetInstantReactivePower_Q3_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.67.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 68.7.0
CGXDLMSRegister* ObisObjectModel:: SetInstantReactivePower_Q4_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.68.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 103. Миттєва повна потужність S+ (1+4 квадранти), кВА (9.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantApparentPowerSplus(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.9.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 104. Миттєва повна потужність S+ (1+4 квадранти) фази L1, кВА (29.7.0)
CGXDLMSRegister* ObisObjectModel:: SetInstantApparentPowerSplus_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.29.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 105. Миттєва повна потужність S+ (1+4 квадранти) фази L2, кВА (49.7.0)
CGXDLMSRegister* ObisObjectModel:: SetInstantApparentPowerSplus_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.49.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 106. Миттєва повна потужність S+ (1+4 квадранти) фази L3, кВА (69.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantApparentPowerSplus_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.69.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 107. Миттєва повна потужність S- (2+3 квадранти), кВА (10.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantApparentPowerSminus(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.10.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 108. Миттєва повна потужність S- (2+3 квадранти) фази L1, кВА (30.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantApparentPowerSminus_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.30.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 109. Миттєва повна потужність S- (2+3 квадранти) фази L2, кВА (50.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantApparentPowerSminus_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.50.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 110. Миттєва повна потужність S- (2+3 квадранти) фази L3, кВА (70.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantApparentPowerSminus_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.70.7.0.255");
    CGXDLMSVariant value = (0);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 111. Миттєва активна потужність |A+|+|A-|), кВт (15.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantActivePowerAbsSum(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.15.7.0.255");
    CGXDLMSVariant value =(0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 112. Миттєва активна потужність |A+|+|A-|) фази L1, кВт (35.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantActivePowerAbsSum_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.35.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 113. Миттєва активна потужність |A+|+|A-|) фази L2, кВт (55.7.0)
CGXDLMSRegister* ObisObjectModel:: SetInstantActivePowerAbsSum_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.55.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 114. Миттєва активна потужність |A+|+|A-|) фази L3, кВт (75.7.0)
CGXDLMSRegister* ObisObjectModel:: SetInstantActivePowerAbsSum_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.75.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 115. Миттєва активна потужність |A+|-|A-|), кВт (16.7.0)
CGXDLMSRegister* ObisObjectModel:: SetInstantActivePowerAbsDiff(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.16.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 116. Миттєва активна потужність |A+|-|A-|) фази L1, кВт (36.7.0)
CGXDLMSRegister* ObisObjectModel:: SetInstantActivePowerAbsDiff_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.36.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 117. Миттєва активна потужність |A+|-|A-|) фази L2, кВт (56.7.0)
CGXDLMSRegister* ObisObjectModel:: SetInstantActivePowerAbsDiff_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.56.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 118. Миттєва активна потужність |A+|-|A-|) фази L3, кВт (76.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantActivePowerAbsDiff_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.76.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 119. Миттєва активна потужність по n-му квадранту, кВт n.7.0 (17.7.0-20.7.0)
// 17.7.0
CGXDLMSRegister* ObisObjectModel:: SetInstantActivePower_Q1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.17.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 18.7.0
CGXDLMSRegister* ObisObjectModel::SetInstantActivePower_Q2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.18.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 19.7.0
CGXDLMSRegister* ObisObjectModel::SetInstantActivePower_Q3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.19.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 20.7.0
CGXDLMSRegister* ObisObjectModel:: SetInstantActivePower_Q4(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.20.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 120. Миттєва активна потужність по n-му квадранту фази L1, кВт n.7.0 (37.7.0-40.7.0)
// 37.7.0
CGXDLMSRegister* ObisObjectModel::SetInstantActivePower_Q1_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.37.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 38.7.0
CGXDLMSRegister* ObisObjectModel:: SetInstantActivePower_Q2_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.38.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 39.7.0
CGXDLMSRegister* ObisObjectModel:: SetInstantActivePower_Q3_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.39.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 40.7.0
CGXDLMSRegister* ObisObjectModel::SetInstantActivePower_Q4_L1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.40.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 121. Миттєва активна потужність по n-му квадранту фази L2, кВт n.7.0 (57.7.0-60.7.0)
// 57.7.0
CGXDLMSRegister* ObisObjectModel:: SetInstantActivePower_Q1_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.57.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 58.7.0
CGXDLMSRegister* ObisObjectModel:: SetInstantActivePower_Q2_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.58.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 59.7.0
CGXDLMSRegister* ObisObjectModel:: SetInstantActivePower_Q3_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.59.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 60.7.0
CGXDLMSRegister* ObisObjectModel::SetInstantActivePower_Q4_L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.60.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 122. Миттєва активна потужність по n-му квадранту фази L3, кВт n.7.0 (77.7.0-80.7.0)
// 77.7.0
CGXDLMSRegister* ObisObjectModel:: SetInstantActivePower_Q1_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.77.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 78.7.0
CGXDLMSRegister* ObisObjectModel:: SetInstantActivePower_Q2_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.78.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 79.7.0
CGXDLMSRegister* ObisObjectModel:: SetInstantActivePower_Q3_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.79.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
// 80.7.0
CGXDLMSRegister* ObisObjectModel::SetInstantActivePower_Q4_L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.80.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 123. Сила струму фази L1, А (31.7.0)
CGXDLMSRegister* ObisObjectModel:: SetInstantCurrentL1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.31.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 124. Сила струму фази L2, А (51.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantCurrentL2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.51.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 125. Сила струму фази L3, А (71.7.0)
CGXDLMSRegister* ObisObjectModel:: SetInstantCurrentL3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.71.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 126. Напруга фази L1, В (32.7.0)
CGXDLMSRegister* ObisObjectModel:: SetInstantVoltageL1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.32.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 127. Напруга фази L2, В (52.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantVoltageL2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.52.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 128. Напруга фази L3, В (72.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantVoltageL3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.72.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 129. Коефіцієнт потужності фази L1 (33.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantPowerFactorL1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.33.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 130. Коефіцієнт потужності фази L2 (53.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantPowerFactorL2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.53.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 131. Коефіцієнт потужності фази L3 (73.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantPowerFactorL3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.73.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 132. Значення частоти основної гармоніки фази L1, Гц (34.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantFrequencyL1(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.34.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 133. Значення частоти основної гармоніки фази L2, Гц (54.7.0)
CGXDLMSRegister* ObisObjectModel::SetInstantFrequencyL2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.54.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 134. Значення частоти основної гармоніки фази L3, Гц (74.7.0)
CGXDLMSRegister* ObisObjectModel:: SetInstantFrequencyL3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.74.7.0.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 135. Кут між фазами L1 та L2 (81.7.10)
CGXDLMSRegister* ObisObjectModel:: SetPhaseAngleL1L2(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.81.7.10.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}

// 136. Кут між фазами L1 та L3 (81.7.20)
CGXDLMSRegister* ObisObjectModel:: SetPhaseAngleL1L3(CGXDLMSObjectCollection& items)
{
    CGXDLMSRegister* reg = new CGXDLMSRegister("1.0.81.7.20.255");
    CGXDLMSVariant value = (0);
    reg->SetValue(value);
    reg->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_FLOAT64));
    reg->SetAccess(2, DLMS_ACCESS_MODE_READ);
    items.push_back(reg);
}
