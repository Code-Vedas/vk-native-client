#include "clipboard_windows.h"
#include "clipboard.h"
#include <iostream>
#include <vector>

UINT ClipboardWindows::CF_HTML = RegisterClipboardFormat(TEXT("HTML Format"));

/// @brief  Get clipboard data
/// @return Map of clipboard data with mime type as key and data as value.
///   - plainText: String
///   - htmlText: String
std::map<flutter::EncodableValue, flutter::EncodableValue> ClipboardWindows::getClipboardData()
{
  std::map<flutter::EncodableValue, flutter::EncodableValue> map;
  Clipboard clipboard = Clipboard();
  std::string *text = clipboard.getClipboardData(CF_TEXT);
  if (text != nullptr)
  {
    map[flutter::EncodableValue("plainText")] = flutter::EncodableValue(*text);
  }
  else
  {
    std::string *unicode = clipboard.getClipboardData(CF_UNICODETEXT);
    if (unicode != nullptr)
    {
      map[flutter::EncodableValue("plainText")] = flutter::EncodableValue(*unicode);
    }
    else
    {
      map[flutter::EncodableValue("plainText")] = flutter::EncodableValue("");
    }
  }

  std::string *html = clipboard.getClipboardData(CF_HTML);
  if (html != nullptr)
  {
    map[flutter::EncodableValue("htmlText")] = flutter::EncodableValue(clipboard.windowsFragmentToHtml(*html));
  }
  else
  {
    map[flutter::EncodableValue("htmlText")] = flutter::EncodableValue("");
  }

  return map;
}

/// @brief  Get clipboard data mime types
/// @return Vector of clipboard data mime types.
///   - plainText: String (if plain text is available)
///   - htmlText: String (if html text is available)
std::vector<flutter::EncodableValue> ClipboardWindows::getClipboardDataMimeTypes()
{
  std::vector<flutter::EncodableValue> mimeTypes = std::vector<flutter::EncodableValue>();
  Clipboard clipboard = Clipboard();
  std::string *text = clipboard.getClipboardData(CF_TEXT);
  if (text != nullptr)
  {
    mimeTypes.push_back(flutter::EncodableValue("plainText"));
  }
  std::string *unicode = clipboard.getClipboardData(CF_UNICODETEXT);
  if (unicode != nullptr && text == nullptr)
  {
    mimeTypes.push_back(flutter::EncodableValue("plainText"));
  }
  std::string *html = clipboard.getClipboardData(CF_HTML);
  if (html != nullptr)
  {
    mimeTypes.push_back(flutter::EncodableValue("htmlText"));
  }
  return mimeTypes;
}

/// @brief  Set clipboard data
/// @param method_call - Method call from flutter.
/// @return true if clipboard data was set successfully, false otherwise.
bool ClipboardWindows::setClipboardData(const flutter::MethodCall<flutter::EncodableValue> &method_call)
{
  // Set clipboard text
  Clipboard clipboard = Clipboard();
  // get clipboard data from method_call.arguments
  // argument is passed a String from flutter
  const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());
  if (!arguments)
  {
    return false;
  }
  // get htmlText from arguments
  const auto htmlText = arguments->find(flutter::EncodableValue("htmlText"));
  // get plainText from arguments
  const auto plainText = arguments->find(flutter::EncodableValue("plainText"));

  if (htmlText == arguments->end() || plainText == arguments->end())
  {
    return false;
  }
  // clear clipboard
  clipboard.clearClipboard();
  // set clipboard data
  clipboard.setClipboardData(CF_TEXT, std::get<std::string>(plainText->second));
  clipboard.setClipboardData(CF_HTML, clipboard.htmlToWindowsFragment(std::get<std::string>(htmlText->second)));
  return true;
}