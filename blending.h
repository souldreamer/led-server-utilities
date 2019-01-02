//
// Created by Ionut Costica on 2019-01-02.
//

#ifndef LEDSERVERUTILITIES_BLEND_H
#define LEDSERVERUTILITIES_BLEND_H

#include <map>
#include <vector>
#include <string>
#include <random>
#include "color.h"
#include <iostream>

#define MAX(a, b) (a)>(b)?(a):(b)
#define MIN(a, b) (a)<(b)?(a):(b)
#define CLAMP(a) MIN(1,MAX(0,(a)))

using operationFunction = std::function<double(double, double)>;
using blendingFunction = std::function<v8::Maybe<Color>(v8::Maybe<Color>, v8::Maybe<Color>, bool)>;

std::function<blendingFunction(operationFunction)> applyOperation =
	[](const operationFunction &operation) -> blendingFunction {
		return [operation](v8::Maybe<Color> color1, v8::Maybe<Color> color2, bool applyToIntensity) -> v8::Maybe<Color> {
			if (color1.IsNothing()) return color2;
			if (color2.IsNothing()) return color1;
			const Color a = color1.FromJust();
			const Color b = color2.FromJust();
			const auto cr = operation(a.r, b.r);
			const auto cg = operation(a.g, b.g);
			const auto cb = operation(a.b, b.b);
			const auto cn = applyToIntensity ? operation(a.n, b.n) : b.n;
			return v8::Just(Color{CLAMP(cr), CLAMP(cg), CLAMP(cb), CLAMP(cn)});
		};
	};

std::random_device randomDevice;
std::mt19937 dissolveRandom(randomDevice());
std::uniform_real_distribution<double> distribution(0, 1);

v8::Maybe<Color> dissolveOperation(v8::Maybe<Color> color1, v8::Maybe<Color> color2, bool applyToIntensity) {
	if (color1.IsNothing() && color2.IsNothing()) return v8::Nothing<Color>();
	if (color1.IsNothing() && !color2.IsNothing()) {
		const Color color = color2.FromJust();
		return distribution(dissolveRandom) <= color.n ? color2 : v8::Nothing<Color>();
	}
	if (!color1.IsNothing() && color2.IsNothing()) {
		const Color color = color1.FromJust();
		return distribution(dissolveRandom) <= color.n ? color1 : v8::Nothing<Color>();
	}
	if (color1.IsNothing() || color2.IsNothing()) return v8::Nothing<Color>();

	const Color col1 = color1.FromJust();
	const Color col2 = color2.FromJust();
	const Color c1 = col1.n < col2.n ? col1 : col2;
	const Color c2 = col1.n < col2.n ? col2 : col1;
	if (c2.n == 0) return color2;
	return distribution(dissolveRandom) >= (c1.n / c2.n - 0.2) ? color2 : color1;
};

const std::map<std::string, blendingFunction> BLEND_MODES = {
	{"Replace", applyOperation([](double a, double b) {return b;})},
	{"Dissolve", dissolveOperation},
	{"Multiply", applyOperation([](double a, double b) {return a * b;})},
	{"Screen", applyOperation([](double a, double b) {return 1 - (1 - a) * (1 - b);})},
	{"Overlay", applyOperation([](double a, double b) {return a < 0.5 ? 2 * a * b : (1 - 2 * (1 - a) * (1 - b));})},
	{"HardLight", applyOperation([](double a, double b) {return b < 0.5 ? 2 * a * b : (1 - 2 * (1 - a) * (1 - b));})},
	{"SoftLight", applyOperation([](double a, double b) {return (1 - 2 * b) * a * a + 2 * b * a;})},
	{"Divide", applyOperation([](double a, double b) {return a / b;})},
	{"Add", applyOperation([](double a, double b) {return a + b;})},
	{"Subtract", applyOperation([](double a, double b) {return a - b;})},
	{"Difference", applyOperation([](double a, double b) {return MAX(a - b, b - a);})},
	{"Darken", applyOperation([](double a, double b) {return MIN(a, b);})},
	{"Lighten", applyOperation([](double a, double b) {return MAX(a, b);})},
};

inline Napi::Value blendPixels(Napi::Env env, Napi::Value inColors, Napi::Value inBlendMode, Napi::Value inApplyToIntensity) {
	if (inColors.IsNull() || inColors.IsUndefined() || !inColors.IsArray()) return env.Null();
	const Napi::Array colorArray = Napi::Array(env, inColors.ToObject());
	v8::Maybe<Color> color = v8::Nothing<Color>();
	const auto blendFunction = BLEND_MODES.at(inBlendMode.IsNull() || inBlendMode.IsUndefined() ? "Replace" : (std::string) inBlendMode.ToString());
	const bool applyToIntensity = inApplyToIntensity.IsNull() || inApplyToIntensity.IsUndefined() ? false : (bool) inApplyToIntensity.ToBoolean();
	for (int i = 0; i < colorArray.Length(); ++i) {
		color = blendFunction(color, objectToColor(env, colorArray[i]), applyToIntensity);
	}
	return colorToObject(env, color);
}

inline Napi::Value blendTwoPixels(Napi::Env env, Napi::Value inColor1, Napi::Value inColor2, Napi::Value inBlendMode, Napi::Value inApplyToIntensity) {
	const auto color1 = objectToColor(env, inColor1);
	const auto color2 = objectToColor(env, inColor2);
	const auto blendFunction = BLEND_MODES.at(inBlendMode.IsNull() || inBlendMode.IsUndefined() ? "Replace" : (std::string) inBlendMode.ToString());
	const bool applyToIntensity = inApplyToIntensity.IsNull() || inApplyToIntensity.IsUndefined() ? false : (bool) inApplyToIntensity.ToBoolean();
	return colorToObject(env, blendFunction(color1, color2, applyToIntensity));
}

inline Napi::Value blendLayers(Napi::Env env, Napi::Value inLayers, Napi::Value inBlendMode, Napi::Value inApplyToIntensity) {
	if (inLayers.IsNull() || inLayers.IsUndefined() || !inLayers.IsArray()) return env.Null();
	const Napi::Array layers = Napi::Array(env, inLayers.ToObject());
	size_t maxLength = 0;
	for (int i = 0; i < layers.Length(); ++i) {
		if (layers[i].IsNull() || layers[i].IsUndefined() || !layers[i].IsArray()) continue;
		const auto layer = Napi::Array(env, layers[i].ToObject());
		maxLength = MAX(maxLength, layer.Length());
	}
	
	const auto blendFunction = BLEND_MODES.at(inBlendMode.IsNull() || inBlendMode.IsUndefined() ? "Replace" : (std::string) inBlendMode.ToString());
	const bool applyToIntensity = inApplyToIntensity.IsNull() || inApplyToIntensity.IsUndefined() ? false : (bool) inApplyToIntensity.ToBoolean();
	
	Napi::Array result = Napi::Array::New(env, maxLength);
	
	Napi::Array layerArrays[layers.Length()];
	for (int i = 0; i < layers.Length(); ++i) {
		if (layers[i].IsNull() || layers[i].IsUndefined() || !layers[i].IsArray()) layerArrays[i] = Napi::Array::New(env);
		else layerArrays[i] = Napi::Array(env, layers[i].ToObject());
	}
	
	for (int i = 0; i < maxLength; ++i) {
		v8::Maybe<Color> computedColor = v8::Nothing<Color>();
		for (int j = 0; j < layers.Length(); ++j) {
			const auto color = objectToColor(env, layerArrays[j][i]);
			computedColor = j == 0
				? color
				: blendFunction(computedColor, color, applyToIntensity);
		}
		result[i] = colorToObject(env, computedColor);
	}
	return result;
}

#endif //LEDSERVERUTILITIES_BLEND_H
