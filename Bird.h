#pragma once
class Bird
{
public:
	Bird(void);
	~Bird(void);

	void up();
	void down();
	void reset();
	int getPos();

private:
	int pos;
};

