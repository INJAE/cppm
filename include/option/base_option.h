#ifndef __CPPM_OPTION_BASE_OPTION_H__
#define __CPPM_OPTION_BASE_OPTION_H__

#include <nlpo/app.h>
#include "config/config.h"

namespace cppm::option
{
    class base_option
    {
    public:
        nlpo::App& app() { return app_; }
        virtual ~base_option() {}
        void config_load();
    protected:
        nlpo::App app_;
        Config config_;
    };
}

#endif
