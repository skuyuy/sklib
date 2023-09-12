#pragma once
#include <type_traits>

namespace sklib::type_traits
{
  template<class T, class... Ts>
  using is_any_of = std::disjunction<std::is_same<T, Ts>...>;
  template<class T, class... Ts>
  constexpr bool is_any_of_v = is_any_of<T, Ts>::value;

  template<class T, class... Ts>
  using is_all_of = std::conjunction<std::is_same<T, Ts>...>;
  template<class T, class... Ts>
  constexpr bool is_all_of_v = is_all_of<T, Ts>::value;
}