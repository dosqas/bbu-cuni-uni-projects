#pragma once

typedef struct {
	char* type;
	char* address;
	int surface;
	int price;
}Estate;

Estate* createEstate(char* type, char* address, int surface, int price);
void destroyEstate(Estate* estate);
char * getType(Estate* estate);
char * getAddress(Estate* estate);
int getSurface(Estate* estate);
int getPrice(Estate* estate);

void setType(Estate* estate, char* type);
void setAddress(Estate* estate, char* address);
void setSurface(Estate* estate, int surface);
void setPrice(Estate* estate, int price);
