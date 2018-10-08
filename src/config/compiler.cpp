#include "config/compiler.h"
#include <fmt/format.h>


namespace cppm
{
    void Compiler::parse(table_ptr table) {
        auto compilers = table->get_table("compiler");
        for(auto& compiler_table : *compilers) {
            CompilerType compiler_t; 
            compiler_t.name = compiler_table.first;
            auto compiler = compilers->get_table(compiler_t.name);
            compiler_t.option     = *compiler->get_as<std::string>("option");
            compiler_t.version    = *compiler->get_as<std::string>("version");
            compiler_t.ccache     = *compiler->get_as<bool>("ccache");
            list[compiler_t.name] = compiler_t;
        } 
    }

    std::string Compiler::generate() {
        using namespace fmt::literals;
        std::string gen
        = "    if (\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"Clang\")\n"_format()
        + "set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} {0}\")"_format(list["clang++"].option)
        + "elseif (\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"GNU\")  \n"_format()
        + "set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} {0}\")"_format(list["g++"].option)
        + "elseif (\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"Intel\")\n"_format()
            //        + "set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} {0}\")"_format();
        + "elseif (\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"MSVC\") \n"_format()
            //        + "set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} {0}\")"_format();
        + "endif()"_format();
        


        return gen;
    }
}