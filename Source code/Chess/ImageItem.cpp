#include "ImageItem.h"

void ImageItem::setBackgroundImg(string path) {
	_backgroundImg.loadFromFile(path);
}

void ImageItem::setUndoImg(string path) {
    _undoImg.loadFromFile(path);
}

void ImageItem::setRedoImg(string path) {
    _redoImg.loadFromFile(path);
}

void ImageItem::setReplayImg(string path) {
    _replayImg.loadFromFile(path);
}

void ImageItem::setExitImg(string path) {
    _exitImg.loadFromFile(path);
}

Image ImageItem::getBackgroundImg() {
    return _backgroundImg;
}

Image ImageItem::getUndoImg() {
    return _undoImg;
}

Image ImageItem::getRedoImg() {
    return _redoImg;
}

Image ImageItem::getReplayImg() {
    return _replayImg;
}

Image ImageItem::getExitImg() {
    return _exitImg;
}