#include "TextureItem.h"

void TextureItem::setBackgroundTexture(Image img) {
    _backgroundTT.loadFromImage(img);
}

void TextureItem::setUndoTexture(Image img) {
    _undoTT.loadFromImage(img);
}

void TextureItem::setRedoTexture(Image img) {
    _redoTT.loadFromImage(img);
}

void TextureItem::setReplayTexture(Image img) {
    _replayTT.loadFromImage(img);
}

void TextureItem::setExitTexture(Image img) {
    _exitTT.loadFromImage(img);
}

Texture TextureItem::getBackgroundTexture() {
    return _backgroundTT;
}

Texture TextureItem::getUndoTexture() {
    return _undoTT;
}

Texture TextureItem::getRedoTexture() {
    return _redoTT;
}

Texture TextureItem::getReplayTexture() {
    return _replayTT;
}

Texture TextureItem::getExitTexture() {
    return _exitTT;
}