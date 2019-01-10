#include "option/build.h"
#include "util/algorithm.hpp"
#include <memory>
#include <fmt/format.h>
#include "util/command.h"
#include "util/filesystem.h"
#include "package/package.h"
#include <iostream>
#include <thread>

namespace cppm::option
{
    std::string CommandBuilder::build(Config& config)
    {
        using namespace fmt::literals;
        auto has_toolchains = [&config]() -> std::string {
           return config.cppm_config.package.toolchains() == "" ?
           "" : " -DCMAKE_TOOLCHAIN_FILE=\"{0}\""_format(config.cppm_config.package.toolchains());
        };

        return "  cd {0} "_format(config.path.build)
            +  "&& cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1"
            +  has_toolchains()
            +  " {0} {1}"_format(config.cmake.option, cmake_option) + " .. "
            +  "&& cmake --build . -- {0}"_format(build_option);
    }
    
    Build::Build() {
        using namespace fmt::literals;
        app_.add_option("help")
            .abbr("h")
            .desc("show cppm commands and options")
            .call_back([&](){ app_.show_help(); });
        app_.add_option("dep")
            .abbr("D")
            .desc("build cppm package file")
            .args("{cppkg name}")
            .call_back([&](){ dep(); });
        app_.add_option("ninja")
            .abbr("n")
            .desc("ninja use to build this option remove build directory")
            .call_back([&](){
                clean = true;
                cmd.cmake_option += " -G Ninja ";
                app_.call_default();
            });
        app_.add_option("make")
            .abbr("m")
            .desc("Unix make use to build this option remove build directory")
            .call_back([&](){
                clean = true;
                cmd.cmake_option += " -G \"Unix Makefiles\" ";
                app_.call_default();
            });
        app_.add_option("gcc")
            .abbr("g")
            .desc("g++ use to compile ")
            .call_back([&](){
                cmd.cmake_option += " -DCMAKE_CXX_COMPILER={0}"_format("g++");
                app_.call_default();
            });
        app_.add_option("clang")
            .abbr("c")
            .desc("clang++ use to compile ")
            .call_back([&](){
                cmd.cmake_option += " -DCMAKE_CXX_COMPILER={0}"_format("clang++");
                app_.call_default();
            });
        app_.add_option("release")
            .abbr("r")
            .desc("compile release mode")
            .call_back([&](){
                cmd.cmake_option += " -DCMAKE_BUILD_TYPE={0}"_format("RELEASE");
                app_.call_default();
            });
        app_.add_option("debug")
            .abbr("d")
            .desc("compile debug mode")
            .call_back([&](){
                cmd.cmake_option += " -DCMAKE_BUILD_TYPE={0}"_format("DEBUG");
                app_.call_default();
            });
        app_.add_option("ntc")
            .desc("not change CMakeLists.txt test options")
            .call_back([this]() { none_tc = true; app_.call_default();});
        app_.add_command()
            .desc("Build command")
            .args("{cppm options} {builder options}")
            .call_back([&](){
                config_load();
                fs::create_directories(config_.path.build);
                cmd.build_option += "-j{}"_format(std::thread::hardware_concurrency());
                if(!none_tc) {
                    dependency_check();
                    cmakelist_build();
                    fs::copy_file((std::string(std::getenv("HOME")))+"/.cppm/cmake/cppm_tool.cmake"
                                  ,config_.path.cmake +"/cppm_tool.cmake"
                                  ,fs::copy_option::overwrite_if_exists);
                }
                if(clean) {
                    fs::remove_all(config_.path.build);
                    fs::create_directory(config_.path.build);
                }
                if(!app_.args().empty()) {
                    cmd.build_option += util::accumulate(app_.args(), " ");
                    app_.args().clear();
                }
                //fmt::print(cmd_->build(config).c_str());
                system(cmd.build(config_).c_str());
            });
    }

    void Build::dependency_check() {
        using namespace fmt::literals;
        using namespace package;
        std::vector<Dependency> not_installed_dep;

        for(auto dep : config_.dependencies.list) {
            if(!fs::exists("{0}/{1}.cmake.in"_format(config_.path.thirdparty,dep.name))) {
                not_installed_dep.push_back(dep);
            }
        }
        for(auto dep : not_installed_dep){
            auto path = cppkg::search(dep.name, dep.version);
            cppkg::install(config_,path);
        }
    }

    void Build::dep() {
        if(app_.args().empty())    { fmt::print(stderr,"need argument");     exit(1);}
        if(app_.args().size() > 1) { fmt::print(stderr,"too many argument"); exit(1);}
        package::cppkg::build(app_.get_arg());
    }

    void Build::cmakelist_build()
    {
        std::ofstream CmakeLists(config_.path.root + "/CMakeLists.txt"); CmakeLists.is_open();
        CmakeLists << config_.generate();
        CmakeLists.close();
    }
}
