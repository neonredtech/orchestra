#pragma once

#include <orchestra_cli/util.hh>

namespace orchestra {
class application final {
  using consolidated_program_arguments = const std::vector<std::string_view>;

private:
  const consolidated_program_arguments launch_arguments;

public:
  application(program_arguments &&args);
  auto run() -> void;
};
} // namespace orchestra
