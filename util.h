//
// Created by Ionut Costica on 2019-01-04.
//

#ifndef LEDSERVERUTILITIES_UTIL_H
#define LEDSERVERUTILITIES_UTIL_H

#include <napi.h>

template<class T>
inline T getNumberAtIndex(Napi::Array array, size_t index, T defaultValue) {
	const Napi::Value keyValue = array[index];
	if (keyValue.IsNull() || keyValue.IsUndefined()) return defaultValue;
	try {
		return (T) keyValue.ToNumber();
	} catch (Napi::Error &err) {
		return defaultValue;
	}
}

// don't forget to delete[] the result of this function or memory leaks are in your future
template<typename T>
inline T* toNumberArray(Napi::Array array, T defaultValue, size_t additionalElementsNumber, T additionalElementValue) {
	const auto arrayLength = array.Length();
	const auto result = new T[arrayLength + additionalElementsNumber];
	for (size_t i = 0; i < arrayLength; ++i) {
		result[i] = getNumberAtIndex(array, i, defaultValue);
	}
	for (size_t i = arrayLength; i < arrayLength + additionalElementsNumber; ++i) {
		result[i] = additionalElementValue;
	}
	return result;
}

#endif //LEDSERVERUTILITIES_UTIL_H
