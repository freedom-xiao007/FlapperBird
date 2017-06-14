#include "StdAfx.h"
#include "Bird.h"


Bird::Bird(void)
	:pos(225)
{
}


Bird::~Bird(void)
{
}

void Bird::up() {
	pos -= 25;
}

void Bird::down() {
	pos += 5;
}

void Bird::reset() {
	pos = 225;
}

int Bird::getPos() {
	return pos;
}
