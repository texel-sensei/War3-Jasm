#pragma once
#include <type_traits>
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
	}

	explicit constexpr restricted_number(S r) : num(r)
	{
	}

	constexpr S value() const { return num; }
	constexpr void set_value(S v) { num = v; }

private:
	S num;
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

using base_id = restricted_number<uint32_t, Eq_i>;

using Register = restricted_number<uint8_t, Eq_i, Tag<struct reg_tag>::type>;
using Type = restricted_number<uint8_t, Eq_i, Tag<struct type_tag>::type>;
using VariableId = restricted_number<base_id, hex_displ, Tag<struct var_tag>::type>;
using FunctionId = restricted_number<base_id, hex_displ, Tag<struct fun_tag>::type>;
using StringId = restricted_number<base_id, hex_displ, Tag<struct str_tag>::type>;
using LabelId = restricted_number<base_id, hex_displ, Tag<struct lab_tag>::type>;

