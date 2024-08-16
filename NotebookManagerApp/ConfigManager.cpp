#include "pch.h"
#include "ConfigManager.h"
#include <Windows.h>
ConfigManager::ConfigManager(const CString& configFile)
    //: configFile(configFile), IP(L""), Port , UID(L""), PWD(L""), DBNAME(L"")// ��� ���� �ʱ�ȭ
{
    // �����ڿ��� ���� ������ �о� ��� ������ �����ϴ� �۾��� ����
    WCHAR buffer[MAX_PATH];

    GetPrivateProfileString(L"PATH", L"IP", L"", buffer, MAX_PATH, configFile);
    IP = buffer;

    GetPrivateProfileString(L"PATH", L"Port", L"", buffer, MAX_PATH, configFile);
    Port = _ttoi(buffer);

    GetPrivateProfileString(L"PATH", L"UID", L"", buffer, MAX_PATH, configFile);
    UID = buffer;

    GetPrivateProfileString(L"PATH", L"PWD", L"", buffer, MAX_PATH, configFile);
    PWD = buffer;

    GetPrivateProfileString(L"PATH", L"DB_NAME", L"", buffer, MAX_PATH, configFile);
    DBNAME = buffer;


}

CStringA ConfigManager::GetIP() const {
    return IP;
}

int ConfigManager::GetPort() const {
    return Port;
}

CStringA ConfigManager::GetUID() const {
    return UID;
}

CStringA ConfigManager::GetPWD() const {
    return PWD;
}

CStringA ConfigManager::GetDBNAME() const {
    return DBNAME;
}