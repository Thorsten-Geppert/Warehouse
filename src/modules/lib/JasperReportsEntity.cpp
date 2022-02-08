#include "JasperReportsEntity.h"

JasperReportsEntity::JasperReportsEntity(
	RuntimeInformationType *rit
) : Entity(
	rit,
	_N("jasper_reports"),
	_N("report"),
	false
) {
	this->rit = rit;

	Set(_N("report"));
	Set(_N("name"));
	Set(_N("description"));
	Set(_N("rank"));
}
