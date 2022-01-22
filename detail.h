namespace detail {
template<class>
constexpr bool is_unbounded_array_v = false;
template<class T>
constexpr bool is_unbounded_array_v<T[]> = true;
 
template<class>
constexpr bool is_bounded_array_v = false;
template<class T, size_t N>
constexpr bool is_bounded_array_v<T[N]> = true;
} // namespace detail
 