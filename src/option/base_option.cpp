#include "option/base_option.h"
#include "util/filesystem.h"
#include "util/algorithm.hpp"
#include <iostream>
namespace cppm::option {
    void base_option::config_load() {
        auto path = cppm::util::reverse_find_file(fs::current_path(), "cppm.toml");
        if(!path) { std::cerr << "can't find cppm.toml" << std::endl; exit(1); }
        config_ = cppm::Config::load(path->parent_path().string());
    }
}
