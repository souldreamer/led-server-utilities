//
// Created by Ionut Costica on 2019-01-02.
//

#ifndef LEDSERVERUTILITIES_INTERPOLATION_H
#define LEDSERVERUTILITIES_INTERPOLATION_H

#include <node.h>

#include <math.h>
#include <map>
#include <string>
#include "color.h"

#include <iostream>

using scalarInterpolationFunction = std::function<double(double, double, double)>;
using colorInterpolationFunction = std::function<Color(Color, Color, double, bool)>;

const std::map<std::string, scalarInterpolationFunction> scalarInterpolationFunctions = {
	{"linear",    [](double a, double b, double x) -> double { return a * (1 - x) + b * x; }},
	{"easeIn",    [](double a, double b, double x) -> double { return (b - a) * pow(x, 3) + a; }},
	{"easeOut",   [](double a, double b, double x) -> double { return (b - a) * (pow(x - 1, 3) + 1) + a; }},
	{"easeInOut", [](double a, double b, double x) -> double {
		return x < 0.5 ? ((b - a) * 4 * x * x * x + a) : ((b - a) * (4 * pow(x - 1, 3) + 1) + a);
	}},
};

std::function<colorInterpolationFunction(scalarInterpolationFunction)> scalarInterpolationToColor =
	[](const scalarInterpolationFunction &interpolation) -> colorInterpolationFunction {
		return [interpolation](Color a, Color b, double x, bool skipIntensity = false) -> Color {
			const auto cr = interpolation(a.r, b.r, x);
			const auto cg = interpolation(a.g, b.g, x);
			const auto cb = interpolation(a.b, b.b, x);
			const auto cn = interpolation(a.n, b.n, x);
			return Color{cr, cg, cb, skipIntensity ? a.n : cn};
		};
	};

const std::map<std::string, colorInterpolationFunction> INTERPOLATION_FUNCTIONS = {
	{"linear",    scalarInterpolationToColor(scalarInterpolationFunctions.at("linear"))},
	{"Linear",    scalarInterpolationToColor(scalarInterpolationFunctions.at("linear"))},
	{"easeIn",    scalarInterpolationToColor(scalarInterpolationFunctions.at("easeIn"))},
	{"EaseIn",    scalarInterpolationToColor(scalarInterpolationFunctions.at("easeIn"))},
	{"easeOut",   scalarInterpolationToColor(scalarInterpolationFunctions.at("easeOut"))},
	{"EaseOut",   scalarInterpolationToColor(scalarInterpolationFunctions.at("easeOut"))},
	{"easeInOut", scalarInterpolationToColor(scalarInterpolationFunctions.at("easeInOut"))},
	{"EaseInOut", scalarInterpolationToColor(scalarInterpolationFunctions.at("easeInOut"))},
};

inline v8::Maybe<Color> interpolatePixels(
	v8::Maybe<Color> color1,
	v8::Maybe<Color> color2,
	v8::Maybe<double> x,
	v8::Maybe<std::string> interpolationName,
	v8::Maybe<bool> skipIntensity
) {
	if (color1.IsNothing() && color2.IsNothing()) return v8::Nothing<Color>();
	
	const auto resolvedX = x.FromMaybe(0);
	auto function = INTERPOLATION_FUNCTIONS.at(interpolationName.FromMaybe("Linear"));
	return resolvedX < 0
		? color1
		: (resolvedX > 1
			? color2
			: v8::Just(function(
				color1.FromMaybe(Color{0, 0, 0, 1}),
				color2.FromMaybe(Color{0, 0, 0, 1}),
				resolvedX,
				skipIntensity.FromMaybe(false)
			))
		);
}

inline Napi::Value interpolateTwoLayers(
	Napi::Env env,
	Napi::Value layer1,
	Napi::Value layer2,
	Napi::Value x,
	Napi::Value interpolationName,
	Napi::Value skipIntensity
) {
	if ((layer1.IsNull() || layer1.IsUndefined()) && (layer2.IsNull() || layer2.IsUndefined())) return env.Null();
	
	const auto resolvedX = x.IsNull() || x.IsUndefined() ? 0 : x.ToNumber().DoubleValue();
	if (resolvedX <= 0) return layer1;
	if (resolvedX >= 1) return layer2;
	const auto resolvedSkipIntensity = skipIntensity.IsNull() || skipIntensity.IsUndefined() ? false : (bool) skipIntensity.ToBoolean();
	
	auto function = INTERPOLATION_FUNCTIONS.at(
		interpolationName.IsNull() || interpolationName.IsUndefined() ? "Linear" : (std::string) interpolationName.ToString()
	);
	
	Napi::Array actualLayer1 = layer1.IsNull() || layer1.IsUndefined() ? Napi::Array::New(env, 0) : Napi::Array(env, layer1.ToObject());
	Napi::Array actualLayer2 = layer2.IsNull() || layer2.IsUndefined() ? Napi::Array::New(env, 0) : Napi::Array(env, layer2.ToObject());
	size_t layer1Length = actualLayer1.Length();
	size_t layer2Length = actualLayer2.Length();
	size_t maxLength = layer1Length > layer2Length ? layer1Length : layer2Length;
	auto result = Napi::Array::New(env, maxLength);
	
	for (int i = 0; i < maxLength; ++i) {
		const Napi::Value value1 = actualLayer1[i];
		const Napi::Value value2 = actualLayer2[i];
		auto color1 = value1.IsNull() || value1.IsUndefined() ? Color{0, 0, 0, 1} : objectToColor(env, value1.ToObject());
		auto color2 = value2.IsNull() || value2.IsUndefined() ? Color{0, 0, 0, 1} : objectToColor(env, value2.ToObject());
		result[i] = colorToObject(env, function(color1, color2, resolvedX, resolvedSkipIntensity));
	}
	return result;
}

#endif //LEDSERVERUTILITIES_INTERPOLATION_H
