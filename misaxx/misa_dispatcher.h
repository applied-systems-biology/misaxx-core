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
     * @tparam ModuleDefinition
     */
    template<class ModuleDefinition> struct misa_dispatcher : public pattxx::dispatcher, public misa_worker<ModuleDefinition> {
        static_assert(std::is_base_of<misa_module_declaration_base, ModuleDefinition>::value, "Template argument must be a module definition!");

        template<class Data> using misa_data = std::shared_ptr<Data>;

        explicit misa_dispatcher(pattxx::nodes::node *t_node, ModuleDefinition *t_module) : pattxx::dispatcher(t_node), m_module(t_module) {

        }

        /**
         * Returns the module definition
         * @return
         */
        const ModuleDefinition &module() const override {
            return *m_module;
        }

    protected:

        /**
         * pattxx::dispatcher::dispatch with the additional function of setting the module accordingly.
         * @tparam Instance
         * @param t_name
         * @return
         */
        template<class Instance, typename... Args> Instance &misa_dispatch(const std::string &t_name, Args&&... args) {
            auto &inst = dispatch<Instance>(t_name, static_cast<ModuleDefinition*>(this), std::forward<Args>(args)...);
            return inst;
        }

        /**
         * Dispatches a submodule as described in the module definition
         * @tparam Submodule
         * @tparam Module
         * @param m_submodule
         * @return
         */
        template<class Submodule, class Module = typename Submodule::module_type>
        Module &misa_dispatch(Submodule &t_submodule) {
            if(t_submodule.has_instance())
                throw std::runtime_error("The submodule already has been instantiated!");
            // Initialize filesystem and more
            t_submodule.init(module());
            // Dispatch the module and tell the submodule holder
            auto &instance = dispatch<Module>(t_submodule.name, std::move(t_submodule.definition()));
            t_submodule.m_module = &instance;
            return instance;
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
