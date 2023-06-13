#pragma once

#include "Settings.h"

class TextureItem {
private:
    Texture _backgroundTT;
    Texture _undoTT;
    Texture _redoTT;
    Texture _replayTT;
    Texture _exitTT;
public:
    TextureItem() {
        // do nothing
    }

    ~TextureItem() {
        // do nothing
    }

    void setBackgroundTexture(Image img);

    void setUndoTexture(Image img);

    void setRedoTexture(Image img);

    void setReplayTexture(Image img);

    void setExitTexture(Image img);

    Texture getBackgroundTexture();

    Texture getUndoTexture();

    Texture getRedoTexture();

    Texture getReplayTexture();

    Texture getExitTexture();
};
