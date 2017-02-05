#pragma once

template<typename T, size_t S>
inline size_t array_size(const T(&a)[S]){
	return S;
}
