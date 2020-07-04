// nocon.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "nocon.h"

// Forward declarations of functions included in this code module:
int Start(const std::wstring& cmdLine);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(nCmdShow);

    return Start(lpCmdLine);
}

int Start(const std::wstring& cmdLine)
{
  using std::wstring;
  wstring filename;
  wstring arguments;
  bool inQuotation = false;
  for (const wchar_t* lpsz = cmdLine.c_str(); *lpsz != 0; ++lpsz)
  {
    size_t nBackslashes = 0;
    for (; *lpsz == L'\\'; ++lpsz)
    {
      ++nBackslashes;
    }
    bool quoteOrUnquote = false;
    if (lpsz[0] == L'"')
    {
      if (nBackslashes % 2 == 0)
      {
        if (lpsz[1] == L'"' && inQuotation)
        {
          ++lpsz;
        }
        else
        {
          inQuotation = !inQuotation;
          quoteOrUnquote = true;
        }
      }
      nBackslashes /= 2;
    }
    for (; nBackslashes > 0; --nBackslashes)
    {
      filename += L'\\';
    }
    if (*lpsz == 0 || ((*lpsz == L' ' || *lpsz == L'\t') && !inQuotation))
    {
      for (; *lpsz == L' ' || *lpsz == '\t'; ++lpsz)
      {
      }
      arguments = lpsz;
      break;
    }
    else if (!quoteOrUnquote)
    {
      filename += *lpsz;
    }
  }
  STARTUPINFOW siStartInfo = { 0 };
  siStartInfo.cb = sizeof(siStartInfo);
  DWORD creationFlags = 0;
  creationFlags |= CREATE_NO_WINDOW;
  PROCESS_INFORMATION processInformation;
  wstring commandLine = L"\"" + filename + L"\" " + arguments;
  if (CreateProcessW(filename.c_str(), const_cast<wchar_t*>(commandLine.c_str()), nullptr, nullptr, TRUE, creationFlags, nullptr, nullptr, &siStartInfo, &processInformation))
  {
    CloseHandle(processInformation.hProcess);
    CloseHandle(processInformation.hThread);
    return EXIT_SUCCESS;
  }
  else
  {
    return EXIT_FAILURE;
  }
}
