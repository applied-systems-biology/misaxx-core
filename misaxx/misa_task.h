//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <pattxx/task.h>
#include "misa_worker.h"
#include "misa_module_declaration_base.h"
#include "object_node_path.h"
#include "algorithm_node_path.h"

namespace misaxx {

    /**
     * Base class for a MISA++ task
     * @tparam ModuleDefinition
     */
    template<class ModuleDefinition> struct misa_task : public pattxx::task, public misa_worker<ModuleDefinition> {

        static_assert(std::is_base_of<misa_module_declaration_base, ModuleDefinition>::value, "Template argument must be a module definition!");

        template<class Data> using misa_data = std::shared_ptr<Data>;

        explicit misa_task(pattxx::nodes::node *t_node, ModuleDefinition *t_module) : pattxx::task(t_node), m_module(t_module) {

        }

        /**
         * Returns the module definition
         * @return
         */
        ModuleDefinition &module() override {
            return *m_module;
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

    private:

        ModuleDefinition *m_module;

    };
}
