//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <pattxx/dispatcher.h>
#include "misa_module_declaration_base.h"
#include "misa_worker.h"
#include "misa_module_base.h"
#include "algorithm_node_path.h"
#include "object_node_path.h"

namespace misaxx {

    /**
     * MISA++ dispatcher base class.
     * @tparam ModuleDeclaration
     */
    template<class ModuleDeclaration> struct misa_dispatcher : public pattxx::dispatcher, public misa_worker<ModuleDeclaration> {
        static_assert(std::is_base_of<misa_module_declaration_base, ModuleDeclaration>::value, "Template argument must be a module definition!");

        template<class Data> using misa_data = std::shared_ptr<Data>;

        /**
         * A future dispatch instance
         */
        template<class Instance> using dispatched = std::function<Instance &(misa_dispatcher<ModuleDeclaration> &)>;

        explicit misa_dispatcher(pattxx::nodes::node *t_node, ModuleDeclaration *t_module) : pattxx::dispatcher(t_node), m_module(t_module) {

        }

        /**
         * Returns the module definition
         * @return
         */
        const ModuleDeclaration &module() const override {
            return *m_module;
        }

        /**
        * pattxx::dispatcher::dispatch with the additional function of setting the module accordingly.
        * @tparam Instance
        * @param t_name
        * @return
        */
        template<class FutureDispatch, class Instance = typename FutureDispatch::result_type>
        Instance &misa_dispatch(FutureDispatch &t_dispatch) {
            return t_dispatch(*this);
        }

    protected:

        /**
         * Declares a future dispatch.
         * @tparam Instance
         * @param t_name
         * @return
         */
        template<class Instance> dispatched <Instance> future_dispatch(const std::string &t_name) {
            return [t_name](misa_dispatcher<ModuleDeclaration> &t_worker) -> Instance& {
                return t_worker.template dispatch<Instance>(t_name, &t_worker);
            };
        }

        /**
         * Loads a parameter from the object namespace. Global in the whole subtree of the object.
         * @tparam T
         * @tparam InputCheckTag
         * @param t_name
         * @param t_metadata
         * @return
         */
        template<typename T, class InputCheckTag = pattxx::parameters::default_check> auto from_object_json(const std::string &t_name, const pattxx::metadata &t_metadata = pattxx::metadata()) {
            return from_json<T, object_node_path, InputCheckTag>(t_name, t_metadata);
        }

        /**
         * Loads a parameter from the object namespace. Global in the whole subtree of the object.
         * @tparam T
         * @tparam InputCheckTag
         * @param t_name
         * @param t_default
         * @param t_metadata
         * @return
         */
        template<typename T, class InputCheckTag = pattxx::parameters::default_check> auto from_object_json_or(const std::string &t_name, T t_default = T(), const pattxx::metadata &t_metadata = pattxx::metadata()) {
            return from_json_or<T, object_node_path, InputCheckTag>(t_name, std::move(t_default), t_metadata);
        }

        /**
         * Loads a parameter from the algorithm namespace. The path ignores the object.
         * @tparam T
         * @tparam InputCheckTag
         * @param t_name
         * @param t_metadata
         * @return
         */
        template<typename T, class InputCheckTag = pattxx::parameters::default_check> auto from_algorithm_json(const std::string &t_name, const pattxx::metadata &t_metadata = pattxx::metadata()) {
            return from_json<T, algorithm_node_path, InputCheckTag>(t_name, t_metadata);
        }

        /**
         * Loads a parameter from the algorithm namespace. The path ignores the object.
         * @tparam T
         * @tparam InputCheckTag
         * @param t_name
         * @param t_default
         * @param t_metadata
         * @return
         */
        template<typename T, class InputCheckTag = pattxx::parameters::default_check> auto from_algorithm_json_or(const std::string &t_name, T t_default = T(), const pattxx::metadata &t_metadata = pattxx::metadata()) {
            return from_json_or<T, algorithm_node_path, InputCheckTag>(t_name, std::move(t_default), t_metadata);
        }

        /**
         * Loads a parameter from an auto-parameter
         * @tparam Parameter
         * @return
         */
        template<class Parameter, class InputCheckTag = pattxx::parameters::default_check> auto from_parameter() {
            return from_json<Parameter, typename Parameter::configuration_namespace_type, InputCheckTag> (Parameter::name, Parameter::metadata);
        }

        /**
         * Loads a parameter from an auto-parameter
         * @tparam Parameter
         * @return
         */
        template<class Parameter, class InputCheckTag = pattxx::parameters::default_check> auto from_parameter_or(Parameter t_default = Parameter()) {
            return from_json_or<Parameter, typename Parameter::configuration_namespace_type, InputCheckTag> (Parameter::name, std::move(t_default), Parameter::metadata);
        }

        /**
         * Creates an enumeration of future dispatchers.
         * @tparam InstanceBase
         * @tparam FutureDispatchers
         * @param name Base name of the dispatchers
         * @param dispatchers future_dispatch instances
         * @return
         */
        template<class InstanceBase, class... FutureDispatchers> misa_future_dispatch_enum<InstanceBase> future_dispatch_enum(std::string name, const FutureDispatchers&... dispatchers) {
            misa_future_dispatch_enum<InstanceBase> result(std::move(name));
            result.add_dispatchers(*this, dispatchers...);
            return result;
        }

    private:

        ModuleDeclaration *m_module;
    };
}
