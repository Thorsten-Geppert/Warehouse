#include "TemplateEntity.h"

TemplateEntity::TemplateEntity(
	RuntimeInformationType *rit
) : Entity(
	rit,
	_N("templates"),
	_N("templateId")
) {
	Set(_N("templateId"), 0U);
	Set(_N("name"));
	Set(_N("content"));
}
