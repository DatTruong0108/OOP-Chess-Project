#pragma once

#include "Settings.h"
#include <string>

using namespace std;

class ImageItem {
private:
    Image _backgroundImg;
    Image _undoImg;
    Image _redoImg;
    Image _replayImg;
    Image _exitImg;
public:
    ImageItem() {
        // do nohting
    }

    ~ImageItem() {
        // do nothing
    }

    void setBackgroundImg(string path);

    void setUndoImg(string path);

    void setRedoImg(string path);

    void setReplayImg(string path);

    void setExitImg(string path);

    Image getBackgroundImg();

    Image getUndoImg();

    Image getRedoImg();

    Image getReplayImg();

    Image getExitImg();
};

