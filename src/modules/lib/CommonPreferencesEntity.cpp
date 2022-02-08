#include "CommonPreferencesEntity.h"

CommonPreferencesEntity::CommonPreferencesEntity(
	DSPostgreSQLDatabase *db
) : DSPostgreSQLDatabaseEntity(
	db,
	_N("common"),
	_N("id")
) {
	Set(_N("id"), 0);
	Set(_N("minorValueAddedTax"), 0.00);
	Set(_N("majorValueAddedTax"), 0.00);
	Set(_N("monitionStates"), 3);
	Set(_N("monitionPrice"), 0.00);
	Set(_N("reportUse"), false);
	Set(_N("reportPathWindows"), _N(""), true, false);
	Set(_N("reportPathUnix"));
	Set(_N("reportPathMacOS"));
	Set(_N("jreHomeWindows"), _N(""), true, false);
	Set(_N("jreHomeUnix"));
	Set(_N("jreHomeMacOS"));
}
