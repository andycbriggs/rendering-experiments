#pragma once

// String helpers
// https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
inline std::wstring toStdWString(const std::string& str)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

inline std::string toStdString(const std::wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}

// Windows error handling
static std::wstring GetErrorString(DWORD errorCode)
{
    LPWSTR messageBuffer = nullptr;

    size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorCode, NULL, (LPWSTR)&messageBuffer, 0, NULL);

    std::wstring message(messageBuffer, size);

    LocalFree(messageBuffer);

    return message;
}

// Fatal error handlers
#define ThrowIfFailed(hr, functionName) \
    if (FAILED(hr)) \
    { \
        std::wstringstream ss; \
        ss << L#functionName << L" failed (result: " << (long long)hr << L", error: " << GetErrorString(GetLastError()) << L", location: " << TEXT(__FUNCTION__) << L":" << __LINE__ << L")"; \
        throw std::runtime_error(toStdString(ss.str())); \
    }

#define ThrowIfNull(hr, functionName) \
    if (hr == NULL) \
    { \
        std::wstringstream ss; \
        ss << L#functionName << L" failed (result: " << (long long)hr << L", error: " << GetErrorString(GetLastError()) << L", location: " << TEXT(__FUNCTION__) << L":" << __LINE__ << L")"; \
        throw std::runtime_error(toStdString(ss.str())); \
    }

#define ThrowIfFalse ThrowIfNull
static_assert(NULL == FALSE, "NULL does not equal FALSE, macro alias is invalid.");

#define ThrowIfNullPointer(ptr, functionName) \
    if (ptr == nullptr) \
    { \
        std::wstring msg = std::wstring(L#functionName) + L" failed (error: Is nullptr, location: " + TEXT(__FUNCTION__) + L":" + std::to_wstring(__LINE__) + L")"; \
        throw std::runtime_error(toStdString(msg)); \
    }

#define ThrowFunctionError(description, functionName) \
    { \
        std::wstring msg = std::wstring(L#functionName) + L" failed (error: " + description + L", location: " + TEXT(__FUNCTION__) + L":" + std::to_wstring(__LINE__) + L")"; \
        throw std::runtime_error(toStdString(msg)); \
    }