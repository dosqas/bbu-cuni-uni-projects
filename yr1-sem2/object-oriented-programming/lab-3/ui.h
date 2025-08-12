#pragma once
#include "services.h"
#include "domain.h"

typedef struct {
	Service* service;
}UI;

UI* createUI(Service* service);
void destroyUI(UI* ui);
int menu(UI* ui);