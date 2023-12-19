#include <type_traits>

template<typename T>
struct SupportsAddAndCompare {
  template<typename U, typename = decltype(
    std::declval<U>() + std::declval<U>(), // Add
    std::declval<U>() < std::declval<U>() // compare, for performing max/min
  )>
  static std::true_type test(U*);
  
  template<typename>
  static std::false_type test(...);

  static constexpr bool value = decltype(test<T>(nullptr))::type;
};

template<typename T>
struct SupportsAddOrMultiply {
  template<typename U, typename = 
  std::enable_if_t<
    std::is_same_v<decltype(std::declval<U>() + std::declval<U>()), U> ||
    std::is_same_v<decltype(std::declval<U>() * std::declval<U>()), U>
  >
  >
  static std::true_type test(U*);
  
  template<typename>
  static std::false_type test(...);

  static constexpr bool value = decltype(test<T>(nullptr))::type;
};

struct S {
  int val;
  S(int val) : val(val) {}
  S operator+(const S& other) {
    return S(this->val + other.val);
  }
};

template<typename T, typename = SupportsAddAndCompare<T>, typename = SupportsAddOrMultiply<T>>
void f(T t) {

}

int main() {
  f(S(1));
}