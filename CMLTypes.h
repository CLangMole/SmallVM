//
// Created by probn on 03.10.2024.
//

#ifndef SMALLVM_CMLTYPES_H
#define SMALLVM_CMLTYPES_H

#include <cstdint>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>
#include <type_traits>

#define Type typename

typedef std::string string;

typedef signed char sint8;
typedef signed short sint16;
typedef signed int sint32;
typedef signed long long sint64;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef float dec32;
typedef double dec64;

typedef bool boolean;

template<Type KEY, Type VALUE>
using Dictionary = std::unordered_map<KEY, VALUE>;

template<Type ...T>
using Variant = std::variant<T...>;

template<Type T>
using List = std::vector<T>;


template<Type T>
boolean TryParse(const string &str, T &value) {
    std::istringstream iStrStream(str);
    iStrStream >> value;
    return !iStrStream.fail();
}

template<Type KEY, Type VALUE>
boolean TryGetValue(const Dictionary<KEY, VALUE> &dictionary, const KEY &key, VALUE &value) {
    auto it = dictionary.find(key);
    if (it != dictionary.end()) {
        value = it->second;
        return true;
    }
    return false;
}

template<Type T> requires(std::is_floating_point<T>::value)
boolean IsDecimal(const string &str, T &value) {
    return TryParse(str, value) && str.find('.') != std::string::npos;
}

#endif //SMALLVM_CMLTYPES_H
