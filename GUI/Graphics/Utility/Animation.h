#pragma once
float InOutSine(float t);

template <typename T>
T Lerp(const T& start, const T& end, float t)
{
	if (t < 0.0f) t = 0.0f;
	else if (t > 1.0f) t = 1.0f;

	return start + t * (end - start);
}

static inline Vector2 VecLerp(const Vector2& a, const Vector2& b, float t) { return Vector2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t); }