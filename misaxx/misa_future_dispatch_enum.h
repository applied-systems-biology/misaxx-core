//
// Created by rgerst on 21.09.18.
//


#pragma once

#include <tuple>

namespace misaxx {
    template<class InstanceBase, class... FutureDispatchers> struct misa_future_dispatch_enum {
        using instance_base_type = InstanceBase;

        /**
         * Checks if the dispatchers are all dispatching InstanceBase
         * @tparam Disp
         * @tparam Args
         * @return
         */
        template<class Disp, class... Args> constexpr void check_dispatchers() {
            static_assert(std::is_base_of<InstanceBase, typename Disp::instance_type>::value,
                    "Future dispatchers must dispatch into base type!");
            if constexpr (sizeof...(Args) > 0) {
                check_dispatchers<Args...>();
            }
        }

        explicit misa_future_dispatch_enum(FutureDispatchers&& ...dispatchers) :
        m_future_dispatchers(std::make_tuple(std::forward<FutureDispatchers>(dispatchers)...)){
            check_dispatchers<FutureDispatchers...>();
        }

        template<class Worker, int Index> InstanceBase &dispatch_(Worker &t_worker, const std::string &t_name) {
            if constexpr (Index == sizeof...(FutureDispatchers) - 1) {
                throw std::runtime_error("Cannot dispatch future_dispatcher with name " + t_name);
            }
            else {
                if(m_future_dispatchers.template get<Index>().name == t_name) {
                    return t_worker.misa_dispatch(m_future_dispatchers.template get<Index>());
                }
                else {
                    return dispatch_<Worker, Index + 1>(t_worker, t_name);
                }
            }
        }

        template<class Worker> InstanceBase &dispatch(Worker &t_worker, const std::string &t_name) {
            return dispatch_<0>(t_name);
        }

        std::tuple<FutureDispatchers...> m_future_dispatchers;
    };
}
