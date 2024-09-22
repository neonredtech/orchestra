project('Orchestra', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++23']
)

add_project_link_arguments('-lstdc++exp', '-lstdc++exp', language: 'cpp')

cli_deps = [
]

cli_includes = [
  include_directories('cli/include')
]

cli_sources = [
  'cli/source/app/app.cc',
]

cli = static_library('fuzzorchestraclient',
  cli_sources,
  install: true,
  include_directories: [cli_includes],
  dependencies: [cli_deps]
)

cli_exe = executable(
  'fuzz-orchestra',
  'cli/source/main.cc',
  include_directories: [cli_includes],
  dependencies: [cli_deps],
  install: true,
  link_with: [cli]
)

