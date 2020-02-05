#include"config/workspace.h"
#include<fmt/format.h>
#include<cpptoml.h>
#include"config/config.h"

namespace cppm
{
    void WorkSpace::parse(table_ptr table)
    {
        auto workspace = toml::get_table(table, "workspace");
        toml::get_array(workspace, "member", member);
        if(!member.empty()) {
            auto dep = toml::get_table(table, "dependencies");
            std::for_each(member.begin(), member.end()
            , [&](auto path) {
                  Config sub = Config().load(path);
                  auto mem = cpptoml::make_table();
                  if(!sub.libs.list.empty()) {
                    auto lib = sub.libs.list.front();
                    mem->insert("version", sub.package.version);
                    mem->insert("module", lib.name);
                    mem->insert("load-path", path);
                    dep->insert(lib.name, mem);
                  }
                  else {
                    mem->insert("version", sub.package.version);
                    mem->insert("load-path", path);
                    mem->insert("type", "bin");
                    dep->insert(sub.package.name, mem);
                  }
            });
            table->insert("dependencies", dep);
        }
    }

    std::string WorkSpace::generate() {
        using namespace fmt::literals;
        std::string gen = "\n";
        //for(auto mem : member) gen += "add_subdirectory({0})\n"_format(mem);
        return gen; 
    }
}