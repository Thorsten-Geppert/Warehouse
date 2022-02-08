#include "LogEntity.h"

LogEntity::LogEntity(
) : DSEntity(
) {
	Set(_N("number"), 0U);
	Set(_N("module"));
	Set(_N("username"));
	Set(_N("description"));
	Set(_N("message"));
	Set(_N("date"));
}

LogEntity *LogEntity::ConvertLogType(DSLogType *logType) {
	LogEntity *logEntity = new LogEntity;

	logEntity->SetValue(_N("number"), logType->GetNumber());
	logEntity->SetValue(_N("module"), logType->GetModule());
	logEntity->SetValue(_N("username"), logType->GetUsername());
	logEntity->SetValue(_N("description"), logType->GetDescription());
	logEntity->SetValue(_N("message"), logType->GetMessage());
	logEntity->SetValue(_N("date"), logType->GetDate().ToDatabaseDateTime());

	return logEntity;
}
