import 'package:flutter_web_plugins/flutter_web_plugins.dart';
import '../vk_native_client_platform_interface.dart';
import 'clipboard_web.dart';
import 'default_clipboard_web.dart';

/// A web implementation of the VkNativeClientPlatform of the VkNativeClient plugin.
class VkNativeClientWeb extends VkNativeClientPlatform {
  /// Constructs a VkNativeClientWeb
  VkNativeClientWeb();

  static void registerWith(Registrar registrar) {
    if (!ClipboardWeb.detectClipboardApi()) {
      return DefaultClipboardWeb.registerWith(registrar);
    }
    VkNativeClientPlatform.instance = VkNativeClientWeb();
  }

  /// get clipboard data from the clipboard asynchronously.
  ///
  /// Returns:
  /// - Future<Map<String, String>?>: a [Map] containing the clipboard data.
  ///   - 'plainText': [String] containing the plain text from the clipboard.
  ///   - 'htmlText': [String] containing the html text from the clipboard.
  @override
  Future<Map<String, String>?> getClipboardData() async {
    /// Read raw clipboard text from the DOM.
    return ClipboardWeb.getClipboardData();
  }

  /// Writes the provided [text] to the clipboard asynchronously.
  ///
  /// Parameters:
  /// - params: [Map<String, String>] containing the clipboard data.
  ///   - 'plainText': [String] containing the plain text to write to the clipboard.
  ///   - 'htmlText': [String] containing the html text to write to the clipboard.
  ///
  /// Returns:
  /// - Future<bool>: [bool] indicating whether the clipboard write was successful.
  @override
  Future<bool> setClipboardData(Map<String, String> params) async {
    /// Write raw clipboard text to the DOM.
    return ClipboardWeb.setClipboardData(params);
  }

  /// Retrieves the mime types of the content currently available in the clipboard asynchronously.
  ///
  /// Returns:
  /// - Future<List<String>>: [List] of [String]s containing the mime types of the content currently available in the clipboard.
  ///   - 'plainText': [String] containing the plain text from the clipboard.
  ///   - 'htmlText': [String] containing the html text from the clipboard.
  @override
  Future<List<String>> getClipboardDataMimeTypes() async {
    /// Read raw clipboard text mime types from the DOM.
    return ClipboardWeb.getClipboardDataMimeTypes();
  }
}
