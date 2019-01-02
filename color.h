//
// Created by Ionut Costica on 2019-01-02.
//

#ifndef LEDSERVERUTILITIES_COLOR_H
#define LEDSERVERUTILITIES_COLOR_H

#include <node.h>
#include <napi.h>
#include <sstream>

struct Color {
	double r;
	double g;
	double b;
	double n;
};

template<class T>
inline T getKey(Napi::Env env, Napi::Object object, std::string key, T defaultValue) {
	const auto keyValue = object.Get(Napi::String::New(env, key));
	if (keyValue.IsNull() || keyValue.IsUndefined()) return defaultValue;
	return keyValue.ToNumber().DoubleValue();
}

inline v8::Maybe<Color> objectToColor(Napi::Env env, v8::Maybe<Napi::Object> object) {
	if (object.IsNothing()) return v8::Nothing<Color>();
	const auto obj = object.FromJust();
	const auto r = getKey(env, obj, "r", 0.0);
	const auto g = getKey(env, obj, "g", 0.0);
	const auto b = getKey(env, obj, "b", 0.0);
	const auto n = getKey(env, obj, "n", 1.0);
	return v8::Just(Color {r, g, b, n});
}
inline v8::Maybe<Color> objectToColor(Napi::Env env, Napi::Value value) {
	if (value.IsUndefined() || value.IsNull()) return v8::Nothing<Color>();
	const auto object = value.ToObject();
	const auto r = getKey(env, object, "r", 0.0);
	const auto g = getKey(env, object, "g", 0.0);
	const auto b = getKey(env, object, "b", 0.0);
	const auto n = getKey(env, object, "n", 1.0);
	return v8::Just(Color {r, g, b, n});
}
inline Color objectToColor(Napi::Env env, Napi::Object object) {
	const auto r = getKey(env, object, "r", 0.0);
	const auto g = getKey(env, object, "g", 0.0);
	const auto b = getKey(env, object, "b", 0.0);
	const auto n = getKey(env, object, "n", 1.0);
	return Color {r, g, b, n};
}

inline Napi::Value colorToObject(Napi::Env env, v8::Maybe<Color> color) {
	if (color.IsNothing()) return env.Null();
	auto resColor = color.FromMaybe(Color {0, 0, 0, 1});
	auto object = Napi::Object::New(env);
	object.Set(Napi::String::New(env, "r"), resColor.r);
	object.Set(Napi::String::New(env, "g"), resColor.g);
	object.Set(Napi::String::New(env, "b"), resColor.b);
	object.Set(Napi::String::New(env, "n"), resColor.n);
	return object;
}
inline Napi::Value colorToObject(Napi::Env env, Color color) {
	auto object = Napi::Object::New(env);
	object.Set(Napi::String::New(env, "r"), color.r);
	object.Set(Napi::String::New(env, "g"), color.g);
	object.Set(Napi::String::New(env, "b"), color.b);
	object.Set(Napi::String::New(env, "n"), color.n);
	return object;
}

inline std::string colorToString(Color c) {
	std::ostringstream out;
	out << "{r: " << c.r << ", g: " << c.g << ", b: " << c.b << ", n: " << c.n << "}";
	return out.str();
}
inline std::string colorToString(v8::Maybe<Color> c) {
	return c.IsNothing() ? "null" : colorToString(c.FromJust());
}

#endif //LEDSERVERUTILITIES_COLOR_H
