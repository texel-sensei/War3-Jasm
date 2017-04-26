#ifndef OPTIONALT_H_INCLUDED
#define OPTIONALT_H_INCLUDED

#include <cassert>
#include <utility>
#include <type_traits>


	struct inplace_construct_t{};
	constexpr inplace_construct_t inplace_construct{};
	struct nullopt_t{};
	constexpr nullopt_t nullopt{};

	template<typename T>
	class optional
	{
	public:
		constexpr optional() : data{},  valid { false } {}

		 constexpr optional(nullopt_t) : valid{false}{}

		 optional(T obj) : valid{true} {
			new (data) T (std::move(obj));
		}

		template<typename... args>
		 explicit optional(inplace_construct_t, args... params) : valid{true}
		{
			new(data) T (std::forward<args>(params)...);
		}

		 optional(const optional& other) : valid(other.valid) {
			if(valid)
				new(data) T (*other.tptr());
		}

		 optional(optional&& other) noexcept(std::is_nothrow_move_constructible_v<T>) : valid(std::move(other.valid))
		{
			if(valid)
			{
				new(data) T (std::move(*other.tptr()));
				other.tptr()->~T();
			}
			other.valid = false;
		}

		 ~optional()
		{
			if(valid)
				tptr()->~T();
		}

		 explicit constexpr operator bool() const
		{
			return valid;
		}

		 T& value()
		{
			if(valid)
				return *tptr();
			assert(!"There is no valid data.");
			return *tptr();
		}

		 const T& value() const
		{
			if (valid)
				return *tptr();
			assert(!"There is no valid data.");
			return *tptr();
		}

		template<typename U>
		 T value_or(U&& alt) const {
			if(valid)
				return value();
			else
				return alt;
		}

		 T& operator*()
		{
			if(valid)
				return *tptr();
			assert(!"There is no valid data.");
			return *tptr();
		}

		 const T& operator*() const
		{
			if (valid)
				return *tptr();
			assert(!"There is no valid data.");
			return *tptr();
		}

		 T* operator->()
		{
			if(valid)
				return tptr();
			assert(!"There is no valid data.");
			return nullptr;
		}

		 const T* operator->() const
		{
			if(valid)
				return tptr();
			assert(!"There is no valid data.");
			return nullptr;
		}

		 optional& operator=(nullopt_t) {
			if(valid){
				tptr()->~T();
			}
			valid = false;
			return *this;
		}

		 optional& operator=(T const& t) {
			if(valid) {
				tptr()->~T();
			}
			new(data) T (t);
			valid = true;
			return *this;
		}

		 optional& operator=(const optional& other)
		{
			if(this == &other){
				return *this;
			}
			if(valid){
				tptr()->~T();
			}
			valid = other.valid;
			if(valid)
				new(data) T (*other.tptr());
			return *this;
		}

		 optional& operator=(optional&& other) noexcept(std::is_nothrow_move_assignable_v<T>)
		{
			if(this == &other){
				return *this;
			}
			if(valid){
				tptr()->~T();
			}
			valid = std::move(other.valid);
			if(valid){
				new(data) T (std::move(*other.tptr()));
				other.valid = false;
				other.tptr()->~T();
			}
			return *this;
		}

	private:
		char data[sizeof(T)];
		bool valid;

		 T* tptr() {return reinterpret_cast<T*>(data);}
		 T const* tptr() const {return reinterpret_cast<const T*>(data);}
	};

	template<typename T, typename... Args>
	 optional<T> make_optional(Args&&... par) {
		return optional<T>(inplace_construct, std::forward<Args>(par)...);
	}

#endif // OPTIONALT_H_INCLUDED
