#pragma once
class Check
{
public:
	bool button1 = false;
	bool button2 = false;
	bool button3 = false;
	bool button4 = false;
	bool up = false;
	Check(AInputEvent* event, int w, int h);
	~Check();
};

