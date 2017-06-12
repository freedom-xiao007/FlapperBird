#pragma once
class Bird
{
public:
	Bird(void);
	~Bird(void);

	void up();
	void down();
	int getPos();

private:
	int pos;
};

