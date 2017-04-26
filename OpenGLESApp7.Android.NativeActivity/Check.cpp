#include "pch.h"
#include "Check.h"


Check::Check(AInputEvent* event, int w, int h)
{
	if (AMotionEvent_getY(event, 0) < 14 * h / 20) {
		up = true;
		button1 = false;
		button2 = false;
		button3 = false;
		button4 = false;
	}

	if ((AMotionEvent_getY(event, 0) > 14 * h / 20) && (AMotionEvent_getY(event, 0) < 18 * h / 20) && (AMotionEvent_getX(event, 0) < 3 * w / 20)) {
		up = false;
		button1 = true;
		button2 = false;
		button3 = false;
		button4 = false;
	}

	if ((AMotionEvent_getY(event, 0) > 18 * h / 20) && (AMotionEvent_getX(event, 0) < 3 * w / 20)) {
		up = false;
		button1 = false;
		button2 = true;
		button3 = false;
		button4 = false;
	}

	if ((AMotionEvent_getY(event, 0) > 14 * h / 20) && (AMotionEvent_getY(event, 0) < 18 * h / 20) && (AMotionEvent_getX(event, 0) > 17 * w / 20)) {
		up = false;
		button1 = false;
		button2 = false;
		button3 = true;
		button4 = false;
	}

	if ((AMotionEvent_getY(event, 0) > 18 * h / 20) && (AMotionEvent_getX(event, 0) > 17 * w / 20)) {
		up = false;
		button1 = false;
		button2 = false;
		button3 = false;
		button4 = true;
	}
}


Check::~Check()
{
}
