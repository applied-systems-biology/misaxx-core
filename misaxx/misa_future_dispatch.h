//
// Created by rgerst on 26.09.18.
//


#pragma once

#include <string>
#include <pattxx/worker.h>

namespace misaxx {

    template<class Worker>
    struct misa_future_dispatch_base {
        virtual pattxx::worker &dispatch(Worker &t_parent) const = 0;
    };

    template<class Worker, class Instance>
    struct misa_future_dispatch : public misa_future_dispatch_base<Worker> {
        using worker_type = Worker;
        using result_type = Instance;
        using function_type = std::function<Instance &(Worker &)>;
        std::string name;
        function_type function;

        misa_future_dispatch() = default;

        template<class HigherOrderInstace>
        misa_future_dispatch(const misa_future_dispatch<Worker, HigherOrderInstace> &t_src)
                : name(t_src.name), function(t_src.function) {

        }

        Instance &dispatch_specific(Worker &t_parent) const {
            return function(t_parent);
        }

        pattxx::worker &dispatch(Worker &t_parent) const override {
            return dispatch_specific(t_parent);
        }
    };
}
