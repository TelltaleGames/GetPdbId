// Copyright (c) 2021 Telltale Games. All rights reserved.
// See LICENSE for usage, modification, and distribution terms.
#pragma once

#include <string>

class RegistryTools
{
public:
    static std::wstring GetStringValueFromHKLM(const std::wstring& regSubKey, const std::wstring& regValue);
};

