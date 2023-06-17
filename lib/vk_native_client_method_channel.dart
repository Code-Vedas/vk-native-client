import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'vk_native_client_platform_interface.dart';

/// An implementation of [VkNativeClientPlatform] that uses method channels.
class MethodChannelVkNativeClient extends VkNativeClientPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('vk_native_client');

  @override
  Future<String?> getPlatformVersion() async {
    final version = await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }

  @override
  Future<String?> getClipboardText() async {
    final text = await methodChannel.invokeMethod<String>('getClipboardText');
    return text;
  }

  @override
  Future<void> setClipboardText(String text) async {
    await methodChannel.invokeMethod<void>('setClipboardText', text);
  }
}