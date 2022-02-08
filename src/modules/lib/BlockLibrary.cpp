#include "BlockLibrary.h"

BlockLibrary::BlockLibrary() : DSTemplateContainer<BlockType *>() {
}

void BlockLibrary::Set(const wxString &key, const wxString &value, void *data, const bool deleteOldData) {
	BlockType *blockType = Find(key, value);
	if(blockType) {
		if(deleteOldData)
			delete static_cast<int *>(blockType->GetData()); // XXX
		blockType->SetData(data);
	} else {
		blockType = new BlockType(key, value, data);
		Add(blockType);
	}
}

void BlockLibrary::Unset(const wxString &key, const wxString &value, const bool deleteData) {
	BlockType *blockType = Find(key, value);
	if(blockType) {
		if(deleteData)
			delete static_cast<int *>(blockType->GetData()); // XXX
		Remove(blockType, true);
	}
}

BlockType *BlockLibrary::Find(const wxString &key, const wxString &value) {
	BlockType *blockType = NULL;

	const unsigned int count = GetCount();
	for(unsigned int i = 0; i < count; i++) {
		blockType = Get(i);
		if(blockType && blockType->GetKey() == key && blockType->GetValue() == value)
			break;
		blockType = NULL;
	}
	
	return blockType;
}

unsigned int BlockLibrary::GetCountByKey(const wxString &key) {
	unsigned int counter = 0;

	BlockType *blockType = NULL;
	const unsigned int count = GetCount();
	for(unsigned int i = 0; i < count; i++) {
		blockType = Get(i);
		if(blockType && blockType->GetKey() == key)
			counter++;
	}

	return counter;
}
