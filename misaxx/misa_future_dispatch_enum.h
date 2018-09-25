//
// Created by rgerst on 21.09.18.
//


#pragma once

#include <tuple>
#include <functional>

namespace misaxx {

    template<class InstanceBase> struct misa_future_dispatch_enum {
        using instance_base_type = InstanceBase;

        /**
         * Checks if the dispatchers are all dispatching InstanceBase
         * @tparam Disp
         * @tparam Args
         * @return
         */
        template<class Worker, class Disp, class... Args> void add_dispatchers(Worker &worker, const Disp &dispatcher, const Args& ...args) {
            static_assert(std::is_base_of<InstanceBase, typename Disp::instance_type>::value,
                    "Future dispatchers must dispatch into base type!");
            // Create a new dispatcher with updated name
            Disp modified_dispatcher = dispatcher;
            modified_dispatcher.name = name + "-" + dispatcher.name;
            m_future_dispatchers[dispatcher.name] = [&worker, modified_dispatcher]() -> InstanceBase& {
               return worker.misa_dispatch(modified_dispatcher);
            };
            if constexpr (sizeof...(Args) > 0) {
                add_dispatchers(worker, args...);
            }
        }

        InstanceBase &dispatch(const std::string &t_name) {
            return m_future_dispatchers.at(t_name)();
        }

        std::string name;
        std::unordered_map<std::string, std::function<InstanceBase& ()>> m_future_dispatchers;

        explicit misa_future_dispatch_enum(std::string t_name) : name(std::move(t_name)) {

        }
    };

}
