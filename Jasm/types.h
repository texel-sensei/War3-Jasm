#pragma once
#include <type_traits>
#include <cstdint>
#include <iostream>

template <typename Rep, template <typename...> class... CRTPs>
struct restricted_number;

template <typename T>
struct underlying_arithmetic_type
{
	using type = T;
};

template <typename T, template <typename...> class... CRTPs>
struct underlying_arithmetic_type<restricted_number<T, CRTPs...>>
{
	using type = std::decay_t<typename underlying_arithmetic_type<typename restricted_number<T, CRTPs...>::S>::type>;
};

template <typename Rep, template <typename...> class... CRTPs>
struct restricted_number : CRTPs<restricted_number<Rep, CRTPs...>>...
{
	using S = std::decay_t<typename underlying_arithmetic_type<Rep>::type>;
	using thistype = restricted_number;

	constexpr restricted_number()
	{
		check();
	}

	explicit constexpr restricted_number(S r) : num(r)
	{
		check();
	}

	constexpr S value() const noexcept { return num; }
#pragma warning(push)
#pragma warning(disable : 4814) // disable constexpr does not add implicit const warning
	constexpr void set_value(S v) noexcept { num = v; }
#pragma warning(pop)

private:
	S num{};
	
	static constexpr void check()
	{
		static_assert(std::is_arithmetic_v<S>, "Underlying type must be arithmetic");
		static_assert(std::is_standard_layout_v<thistype>, "This is not standard layout!");
	}
};

template<typename T>
class Eq_i
{
	friend constexpr bool operator ==(T a, T b) {
		return a.value() == b.value();
	}

	friend constexpr bool operator !=(T a, T b) {
		return !(a == b);
	}
};

template<typename T>
struct Tag
{
	template<typename U, typename TT = T>
	struct type{};
};

template<typename T>
struct hex_displ
{
	friend std::ostream& operator<<(std::ostream& os, T t) { 
		return os << std::hex << t.value();
	}
};

template<typename T>
struct hashable
{
	size_t hash() const noexcept
	{
		return static_cast<T const*>(this)->value();
	}
};

template<typename T>
struct NumHasher
{
	size_t operator()(hashable<T> h) const noexcept {
		return h.hash();
	}
};

template<template<typename...> class... CRTPs>
using base_id = restricted_number<uint32_t, Eq_i, hex_displ, hashable, CRTPs...>;

using Register = restricted_number<uint8_t, Eq_i, Tag<struct reg_tag>::type>;
using Type = restricted_number<uint8_t, Eq_i, Tag<struct type_tag>::type>;
using VariableId = base_id<Tag<struct var_tag>::type>;
using FunctionId = base_id<Tag<struct fun_tag>::type>;
using LabelId    = base_id<Tag<struct lab_tag>::type>;

