//
// Created by rgerst on 26.09.18.
//


#pragma once

#include <string>
#include <misaxx/workers/misa_worker_base.h>
#include <functional>
#include <typeindex>

namespace misaxx {

    class misa_dispatcher;

    /**
     * Contains a blueprint for a dispatched worker
     * @tparam Worker
     */
    struct misa_dispatch_blueprint_base {
        virtual misaxx::misa_worker_base &dispatch(misa_dispatcher &t_parent) const = 0;
        virtual std::type_index get_instantiated_type() = 0;
        virtual std::string get_name() = 0;
    };

    /**
     * Contains a blueprint for a dispatched worker
     * @tparam Worker
     * @tparam Instance
     */
    template<class Instance>
    struct misa_dispatch_blueprint : public misa_dispatch_blueprint_base {
        using result_type = Instance;
        using function_type = std::function<Instance &(misa_dispatcher &)>;
        std::string name;
        function_type function;
        bool allow_multi_instancing = true;
        bool is_instantiated = false;

        misa_dispatch_blueprint() = default;

        /**
         * This constructor is needed to allow internal dynamic casting of the function
         * @tparam HigherOrderInstace
         * @param t_src
         */
        template<class HigherOrderInstace>
        misa_dispatch_blueprint(const misa_dispatch_blueprint<HigherOrderInstace> &t_src)
                : name(t_src.name), function(t_src.function) {

        }

        Instance &dispatch_specific(misa_dispatcher &t_parent) const {
            if(!allow_multi_instancing && is_instantiated)
                throw std::runtime_error("Cannot instantiate blueprint of type " + std::string(typeid(Instance).name()) + " as it does not allow multiple instancing.");
            return function(t_parent);
        }

        misaxx::misa_worker_base &dispatch(misa_dispatcher &t_parent) const override {
            return dispatch_specific(t_parent);
        }

        std::type_index get_instantiated_type() override {
            return typeid(Instance);
        }

        std::string get_name() override {
            return name;
        }
    };
}
