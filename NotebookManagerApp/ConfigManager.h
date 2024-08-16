#pragma once
class ConfigManager
{
public:
    ConfigManager(const CString& configFile);

    CStringA GetIP() const;
    int GetPort() const;
    CStringA GetUID() const;
    CStringA GetPWD() const;
    CStringA GetDBNAME() const;

private:
    CString configFile;
    CStringA IP;
    int Port;
    CStringA UID;
    CStringA PWD;
    CStringA DBNAME;
};

