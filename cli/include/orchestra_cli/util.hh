#pragma once

#include <format>
#include <memory>
#include <optional>
#include <print>
#include <ranges>
#include <stacktrace>
#include <string_view>
#include <vector>

#ifdef __GNUG__
#include <cxxabi.h>
#endif

namespace orchestra {

template <typename T>
concept good_exception =
    std::derived_from<std::remove_cvref_t<T>, std::exception>;

template <good_exception T> struct program_error {
  const std::remove_cvref_t<T> exception;
  const std::stacktrace stack;
};

using program_arguments = const std::vector<std::string_view>;
using general_error = const program_error<std::runtime_error>;

namespace util {

inline auto build_program_arguments(const int argc, const char *const *argv)
    -> program_arguments {
  return program_arguments{argv,
                           std::next(argv, static_cast<std::ptrdiff_t>(argc))};
}

inline auto environment_variable(const std::string variable)
    -> std::optional<const std::string_view> {
  if (const char *result = std::getenv(variable.data())) {
    return {result};
  }

  return std::nullopt;
}

inline auto cxx_demangle(const char *input) -> std::string {
#ifdef __GNUG__
  int cxa_demangle_result{-1};
  std::unique_ptr<char, decltype(&std::free)> demangled_name{
      abi::__cxa_demangle(input, nullptr, nullptr, &cxa_demangle_result),
      std::free};

  return cxa_demangle_result == 0 ? demangled_name.get() : input;
#else
  return input;
#endif
}

inline auto print_stacktrace(const std::stacktrace &&stacktrace) -> void {
  std::println("  stack:");
  for (const auto &[i, entry] : stacktrace | std::views::enumerate) {
    std::println("    #{} -> @ {}:{} ({})", (stacktrace.size() - i),
                 entry.description(), entry.source_line(), entry.source_file());
  }
}

} // namespace util

template <good_exception T>
[[noreturn]] constexpr inline auto
panic(const program_error<T> &&error) -> void {
  std::println("panic: called with exception of type {}",
               util::cxx_demangle(typeid(error.exception).name()));

  std::println("  what(): {}", error.exception.what());
  util::print_stacktrace(std::move(error.stack));

  __builtin_trap();
}

constexpr inline auto error_from_std_exception(good_exception auto &&exception)
    -> const program_error<std::remove_cvref_t<decltype(exception)>> {
  return {std::move(exception), std::move(std::stacktrace::current())};
}

constexpr inline auto panic(good_exception auto &&exception) -> void {
  auto error = error_from_std_exception(exception);
  panic(std::move(error));
}

constexpr inline auto assert(const std::string_view &name,
                             const bool cond) -> void {
  if (!(cond)) {
    panic(
        std::runtime_error(std::format("Assertion '{}' doesn't hold!", name)));
  }
}

} // namespace orchestra
