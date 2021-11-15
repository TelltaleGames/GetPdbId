// /*
//cl /I"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\DIA SDK\include" "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\DIA SDK\lib\amd64\diaguids.lib" Ole32.lib pdbguid.cpp

#include <stdio.h>
#include <Dia2.h>
#include <assert.h>
#include <string>
#include "RegistryTools.h"

typedef HRESULT(DllGetClassObject_t)(REFCLSID rclsid, REFIID riid, LPVOID* ppv);

int wmain(int argc, const wchar_t** argv) {
    if (argc != 3) {
        return 1;
    }

    HRESULT hr = CoInitialize(NULL);
    assert(SUCCEEDED(hr));

    std::wstring vsBasePath = argv[2];
    std::wstring msdiaFile = vsBasePath + std::wstring(L"\\DIA SDK\\bin\\amd64\\msdia140.dll");

    HMODULE module = LoadLibraryW(msdiaFile.c_str());
    assert(module);

    DllGetClassObject_t* DllGetClassObject = (DllGetClassObject_t*)GetProcAddress(module, "DllGetClassObject");
    assert(DllGetClassObject);

    IClassFactory* classFactory;
    hr = DllGetClassObject(CLSID_DiaSource, IID_PPV_ARGS(&classFactory));
    assert(SUCCEEDED(hr));

    IDiaDataSource* src;
    hr = classFactory->CreateInstance(nullptr, IID_PPV_ARGS(&src));
    // The above bascially approximates this, but without requiring the dll to be registered.
    //hr = CoCreateInstance(CLSID_DiaSource, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&src));
    assert(SUCCEEDED(hr));

    hr = src->loadDataFromPdb(argv[1]);
    assert(SUCCEEDED(hr));
    IDiaSession* session;
    hr = src->openSession(&session);
    assert(SUCCEEDED(hr));
    IDiaSymbol* symbol;
    hr = session->get_globalScope(&symbol);
    assert(SUCCEEDED(hr));

    const int bufSize = 256;

    GUID guid;
    symbol->get_guid(&guid);
    wchar_t buf[bufSize];
    int len = StringFromGUID2(guid, buf, bufSize);
    assert(len > 0);

    DWORD age;
    symbol->get_age(&age);
    std::wstring age_str = std::to_wstring(age);

    // Format guid into PDB ID
    wchar_t pBuf[bufSize];
    int j = 0;
    for (int i = 0; i < bufSize - age_str.length() - 1 && buf[i] != L'\0'; ++i) {
        wchar_t c = buf[i];
        switch (c) {
        case L'{':
        case L'}':
        case L'-':
        case L' ':
            continue;
        }

        pBuf[j] = c;
        ++j;
    }

    // Append on age
    for (int i = 0; i < age_str.length(); ++i)
        pBuf[j + i] = age_str[i];

    pBuf[j + age_str.length()] = L'\0';

    wprintf(L"%ls\n", pBuf);

    symbol->Release();
    session->Release();
    src->Release();
    classFactory->Release();
    FreeLibrary(module);

    return 0;
}