#include "Moose.h"

Moose::Moose(RuntimeInformationType *rit) {
	SetRuntimeInformationType(rit);
}

void Moose::SetRuntimeInformationType(RuntimeInformationType *rit) {
	this->rit = rit;
}

RuntimeInformationType *Moose::GetRuntimeInformationType() const {
	return rit;
}
