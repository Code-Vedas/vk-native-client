#pragma once
#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>

// class ClipboardLinux
class ClipboardLinux
{
public:
    static char kMimeTextPlain[];
    static char kMimeTextHtml[];

    static char kPlainText[];
    static char kHtmlText[];
    static GdkAtom kGdkAtomTextPlain;
    static GdkAtom kGdkAtomTextHtml;

    static guint kUserInfoTextPlain;
    static guint kUserInfoTextHtml;
    static FlValue *getClipboardData();
    static bool canCopyFromClipboard();
    static bool setClipboardData(FlMethodCall *method_call);
};