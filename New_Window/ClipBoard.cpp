#include "ClipBoard.h"

void ClipBoard::Copy(const char* text) {
    if (OpenClipboard(NULL)) {
        EmptyClipboard();
        size_t size = strlen(text) + 1;
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size * sizeof(char));
        if (hMem) {
            void* pMem = GlobalLock(hMem);
            if (pMem == nullptr) return;
            memcpy(pMem, text, size * sizeof(char));
            GlobalUnlock(hMem);
            SetClipboardData(CF_TEXT, hMem);
        }
        CloseClipboard();
    }
}
