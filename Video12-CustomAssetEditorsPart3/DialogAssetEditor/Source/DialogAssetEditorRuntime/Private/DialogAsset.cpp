#include "DialogAsset.h"
#include "UObject/ObjectSaveContext.h"

void UDialogAsset::PreSave(FObjectPreSaveContext saveContext) { 
    if (_onPreSaveListener) {
        _onPreSaveListener();
    }
}
