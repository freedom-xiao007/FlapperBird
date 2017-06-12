#include "StdAfx.h"
#include "Bird.h"


Bird::Bird(void)
	:pos(125)
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

int Bird::getPos() {
	return pos;
}
