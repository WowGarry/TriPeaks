#include "UndoManager.h"

USING_NS_CC;

UndoManager::UndoManager() {
}

UndoManager::~UndoManager() {
    clear();
}

void UndoManager::recordAction(UndoModel* undoData) {
    if (undoData) {
        _undoStack.push_back(undoData);
    }
}

UndoModel* UndoManager::undo() {
    if (_undoStack.empty()) {
        return nullptr;
    }

    UndoModel* undoData = _undoStack.back();
    _undoStack.pop_back();
    return undoData;
}

bool UndoManager::canUndo() const {
    return !_undoStack.empty();
}

void UndoManager::clear() {
    for (auto undoData : _undoStack) {
        delete undoData;
    }
    _undoStack.clear();
}