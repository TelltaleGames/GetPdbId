#pragma once

#include <string>

class RegistryTools
{
public:
    static std::wstring GetStringValueFromHKLM(const std::wstring& regSubKey, const std::wstring& regValue);
};

