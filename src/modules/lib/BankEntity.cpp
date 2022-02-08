#include "BankEntity.h"

BankEntity::BankEntity(
	RuntimeInformationType *rit
) : Entity(
	rit,
	_N("banks"),
	_N("bankId")
) {
	Set(_N("bankId"), 0U);
	Set(_N("bank"));
	Set(_N("bankCode"));
	Set(_N("bic"));
}
