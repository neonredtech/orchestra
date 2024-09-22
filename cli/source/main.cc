#include <orchestra_cli/app.hh>
#include <orchestra_cli/util.hh>

#include <exception>
#include <print>
#include <stacktrace>

#include <cxxabi.h>

namespace {

auto global_exception_handler() -> void {
  const auto stacktrace = std::stacktrace::current(1);
  const auto current_exception = std::current_exception();

#ifdef __GNUG__
  const auto current_exception_typename =
      abi::__cxa_current_exception_type()->name();
  std::println("uncaught exception of type {}",
               orchestra::util::cxx_demangle(current_exception_typename));
#else
  std::println("uncaught exception (type is not known)");
#endif

  try {
    if (current_exception) {
      std::rethrow_exception(current_exception);
    }
  } catch (const std::exception &err) {
    std::println("  what: {}", err.what());
  } catch (...) {
    std::println("  exception message is not known.");
  }

  orchestra::util::print_stacktrace(std::move(stacktrace));
  __builtin_trap();
}

} // namespace

auto main(int argc, char **argv) -> int {
  std::set_terminate(global_exception_handler);

  orchestra::application app{
      orchestra::util::build_program_arguments(argc, argv)};
  app.run();
}
