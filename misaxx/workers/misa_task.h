//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <pattxx/task.h>
#include "misa_worker.h"
#include "misaxx/misa_module_declaration_base.h"
#include "misaxx/parameters/object_node_path.h"
#include "misaxx/parameters/algorithm_node_path.h"

namespace misaxx {

    /**
     * Base class for a MISA++ task
     * @tparam ModuleDeclaration
     */
    template<class ModuleDeclaration> struct misa_task : public pattxx::task, public misa_worker<ModuleDeclaration> {

        static_assert(std::is_base_of<misa_module_declaration_base, ModuleDeclaration>::value, "Template argument must be a module definition!");

        explicit misa_task(pattxx::nodes::node *t_node, ModuleDeclaration *t_module) : pattxx::task(t_node), m_module(t_module) {

        }

        /**
         * Returns the module definition
         * @return
         */
        ModuleDeclaration &module() override {
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
        template<typename T, class InputCheckTag = pattxx::parameters::default_check> auto from_object_json(const std::string &t_name, const metadata<T> &t_metadata = metadata<T>()) {
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
        template<typename T, class InputCheckTag = pattxx::parameters::default_check> auto from_object_json_or(const std::string &t_name, T t_default = T(), const metadata<T> &t_metadata = metadata<T>()) {
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
        template<typename T, class InputCheckTag = pattxx::parameters::default_check> auto from_algorithm_json(const std::string &t_name, const metadata<T> &t_metadata = metadata<T>()) {
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
        template<typename T, class InputCheckTag = pattxx::parameters::default_check> auto from_algorithm_json_or(const std::string &t_name, T t_default = T(), const metadata<T> &t_metadata = metadata<T>()) {
            return from_json_or<T, algorithm_node_path, InputCheckTag>(t_name, std::move(t_default), t_metadata);
        }

        /**
         * Loads a parameter from an auto-parameter
         * @tparam Parameter
         * @return
         */
        template<class Parameter, class InputCheckTag = pattxx::parameters::default_check> auto from_parameter() {
            return from_json<Parameter, typename Parameter::configuration_namespace_type, InputCheckTag> (Parameter().get_serialization_id());
        }

        /**
         * Loads a parameter from an auto-parameter
         * @tparam Parameter
         * @return
         */
        template<class Parameter, class InputCheckTag = pattxx::parameters::default_check> auto from_parameter_or(Parameter t_default = Parameter()) {
            return from_json_or<Parameter, typename Parameter::configuration_namespace_type, InputCheckTag> (Parameter().get_serialization_id(), std::move(t_default));
        }

    public:

        virtual void misa_work() = 0;

        virtual void misa_simulate() {
        }

        void work() override {
            if(get_node().get_runtime().is_building_schema())
                misa_simulate();
            else
                misa_work();
        }

    private:

        ModuleDeclaration *m_module;

    };
}
