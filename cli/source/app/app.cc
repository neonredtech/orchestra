#include <orchestra_cli/app.hh>
#include <orchestra_cli/util.hh>

#include <format>
#include <print>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>

namespace orchestra {

application::application(const program_arguments &&args)
    : launch_arguments{std::move(args)} {
  // program constructor
}

auto application::run() -> void {
  for (const auto &path_arg : this->launch_arguments | std::views::drop(1)) {
    std::println("{}", path_arg);
  }
}

} // namespace orchestra
