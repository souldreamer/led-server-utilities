#include "led-server-utilities.h"
#include <map>
#include <string>
#include <functional>
#include <math.h>
#include <napi.h>

#include "color.h"
#include "interpolation.h"
#include "blending.h"

Napi::Value InterpolatePixels(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	const auto result = colorToObject(
		env,
		interpolatePixels(
			objectToColor(env, info[0].IsNull() || info[0].IsUndefined() ? v8::Nothing<Napi::Object>() : v8::Just(info[0].ToObject())),
			objectToColor(env, info[1].IsNull() || info[0].IsUndefined() ? v8::Nothing<Napi::Object>() : v8::Just(info[1].ToObject())),
			info[2].IsNull() || info[2].IsUndefined() ? v8::Nothing<double>() : v8::Just(info[2].ToNumber().DoubleValue()),
			info[3].IsNull() || info[3].IsUndefined() ? v8::Nothing<std::string>() : v8::Just(std::string(info[3].ToString())),
			info[4].IsNull() || info[4].IsUndefined() ? v8::Nothing<bool>() : v8::Just(bool(info[4].ToBoolean()))
		)
	);
	return result;
}

Napi::Value InterpolateTwoLayers(const Napi::CallbackInfo& info) {
	return interpolateTwoLayers(info.Env(), info[0], info[1], info[2], info[3], info[4]);
}

Napi::Value BlendPixels(const Napi::CallbackInfo& info) {
	return blendPixels(info.Env(), info[0], info[1], info[2]);
}

Napi::Value BlendTwoPixels(const Napi::CallbackInfo& info) {
	return blendTwoPixels(info.Env(), info[0], info[1], info[2], info[3]);
}

Napi::Value BlendLayers(const Napi::CallbackInfo& info) {
	return blendLayers(info.Env(), info[0], info[1], info[2]);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
	exports.Set(
		Napi::String::New(env, "interpolatePixels"),
		Napi::Function::New(env, InterpolatePixels)
	);
	exports.Set(
		Napi::String::New(env, "interpolateTwoLayers"),
		Napi::Function::New(env, InterpolateTwoLayers)
	);
	exports.Set(
		Napi::String::New(env, "blendPixels"),
		Napi::Function::New(env, BlendPixels)
	);
	exports.Set(
		Napi::String::New(env, "blendTwoPixels"),
		Napi::Function::New(env, BlendTwoPixels)
	);
	exports.Set(
		Napi::String::New(env, "blendLayers"),
		Napi::Function::New(env, BlendLayers)
	);
	return exports;
}

NODE_API_MODULE(LedServerUtilities, Init)

