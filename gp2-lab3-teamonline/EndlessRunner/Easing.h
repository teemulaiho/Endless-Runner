#pragma once

/*

Code found here: https://github.com/jesusgollonet/ofpennereasing/blob/master/PennerEasing/Linear.cpp

@t is the current time (or position) of the tween. This can be seconds or frames, steps, seconds, ms, whatever – as long as the unit is the same as is used for the total time [3].
@b is the beginning value of the property.
@c is the change between the beginning and destination value of the property.
@d is the total time of the tween.

*/


inline static float Cubic_EaseIn(float t, float b, float c, float d) {
	return c * (t /= d) * t * t + b;
}
inline static float Cubic_EaseOut(float t, float b, float c, float d) {
	return c * ((t = t / d - 1) * t * t + 1) + b;
}

inline static float Cubic_EaseInOut(float t, float b, float c, float d) {
	if ((t /= d / 2) < 1) return c / 2 * t * t * t + b;
	return c / 2 * ((t -= 2) * t * t + 2) + b;
}

inline static float Linear_EaseNone(float t, float b, float c, float d) {
	return c * t / d + b;
}

inline static int Cubic_EaseOut_Clamped(int t, int b, int c, int d) {
	if (t > d) t = d;
	return (int)Cubic_EaseOut((float)t, (float)b, (float)c, (float)d);
}