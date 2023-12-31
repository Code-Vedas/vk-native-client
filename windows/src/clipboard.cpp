// MIT License
//
// Copyright (c) 2023 Code Vedas
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include "Clipboard.h"
#include <iostream>
#include <vector>

/// @brief Get clipboard data
/// @param format - Clipboard format
/// @return String of clipboard data
std::string *Clipboard::getClipboardData(int format)
{
  std::string str;
  if (OpenClipboard(nullptr))
  {
    HANDLE hData = GetClipboardData(format);
    if (hData != nullptr)
    {
      LPCSTR pszText = static_cast<LPCSTR>(GlobalLock(hData));
      if (pszText != nullptr)
      {
        str.assign(pszText);
        GlobalUnlock(hData);
      }
    }
    CloseClipboard();
  }
  if (str.empty())
  {
    return nullptr;
  }
  return new std::string(str);
}
void Clipboard::clearClipboard()
{
  if (OpenClipboard(nullptr))
  {
    EmptyClipboard();
    CloseClipboard();
  }
}

/// @brief Set clipboard data
/// @param format - Clipboard format
/// @param data - Clipboard data
/// @return True if successful otherwise false
void Clipboard::setClipboardData(int format, std::string data)
{
  if (OpenClipboard(nullptr))
  {
    HGLOBAL hClipboardData;
    hClipboardData = GlobalAlloc(GMEM_DDESHARE, data.size() + 1);
    char *pchData;
    pchData = (char *)GlobalLock(hClipboardData);
    strcpy_s(pchData, data.size() + 1, LPCSTR(data.c_str()));
    GlobalUnlock(hClipboardData);
    SetClipboardData(format, hClipboardData);
    CloseClipboard();
  }
}

/// @brief Get clipboard data
/// @param htmlString - HTML string
/// @return HTML string with Windows clipboard header and footer
std::string Clipboard::htmlToWindowsFragment(std::string htmlString)
{
  // Create HTML clipboard header
  std::string header =
      "Version:0.9\r\n"
      "StartHTML:00000000\r\n"
      "EndHTML:00000000\r\n"
      "StartFragment:00000000\r\n"
      "EndFragment:00000000\r\n"
      "<html><body>\r\n"
      "<!--StartFragment-->\r\n";

  // Create HTML clipboard footer
  std::string footer =
      "\r\n<!--EndFragment-->\r\n"
      "</body>\r\n"
      "</html>\r\n";

  std::string clipboardData = header + htmlString + footer;

  // Calculate offsets for the clipboard header
  size_t startHTML = header.length();
  size_t endHTML = clipboardData.length() - footer.length() - 2;
  size_t startFragment = clipboardData.find("<!--StartFragment-->");
  size_t endFragment = clipboardData.find("<!--EndFragment-->");

  // Replace the template offsets with the calculated offsets
  clipboardData.replace(clipboardData.find("00000000"), 8, std::to_string(startHTML));
  clipboardData.replace(clipboardData.find("00000000"), 8, std::to_string(endHTML));
  clipboardData.replace(clipboardData.find("00000000"), 8, std::to_string(startFragment));
  clipboardData.replace(clipboardData.find("00000000"), 8, std::to_string(endFragment));
  return clipboardData;
}

/// @brief Get clipboard data
/// @param fragmentHTML - HTML fragment
/// @return HTML fragment without Windows clipboard header and footer
std::string Clipboard::windowsFragmentToHtml(std::string fragmentHTML)
{
  // Find the start and end fragment markers in the fragment HTML
  std::string startMarker = "<!--StartFragment-->";
  std::string endMarker = "<!--EndFragment-->";

  // Find the positions of the start and end markers
  size_t startPos = fragmentHTML.find(startMarker);
  size_t endPos = fragmentHTML.find(endMarker);

  // If both markers are found
  if (startPos != std::string::npos && endPos != std::string::npos)
  {
    // Extract the normal HTML between the markers
    size_t startHTMLPos = startPos + startMarker.length();
    size_t normalHTMLLength = endPos - startHTMLPos;
    std::string normalHTML = fragmentHTML.substr(startHTMLPos, normalHTMLLength);
    return normalHTML;
  }

  // Return an empty string if markers are not found
  return "";
}