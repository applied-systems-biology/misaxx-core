//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <pattxx/dispatcher.h>
#include "misa_worker.h"
#include "misa_module_declaration_base.h"
#include "misa_module_base.h"
#include "misa_submodule.h"
#include "algorithm_node_path.h"
#include "object_node_path.h"
#include "misa_future_dispatch.h"
#include "misa_future_dispatch_enum.h"

namespace misaxx {

    /**
     * Second part of a MISA++ module. This module dispatcher is responsible dispatching the module-internal workers,
     * setting up the data and submodules.
     *
     * A MISA module must be always instantiated with a module definition (the root module is instantiated with a default definition)
     * @tparam ModuleDeclaration
     */
    template<class ModuleDeclaration>
    struct misa_module : public misa_module_base, public misa_worker<ModuleDeclaration>, public pattxx::dispatcher, public ModuleDeclaration {

    public:

        using module_declaration_type = ModuleDeclaration;

        /**
         * A future dispatch instance
         */
        template<class Instance> using dispatched = misa_future_dispatch<Instance>;

        /**
         * An enumeration of future dispatch instances
         */
        template<class InstanceBase> using dispatched_enum = misa_future_dispatch_enum<InstanceBase>;

        static_assert(std::is_base_of<misa_module_declaration_base, ModuleDeclaration>::value, "misa_module only accepts module definitions as template parameter!");

        explicit misa_module(pattxx::nodes::node *t_node, ModuleDeclaration definition) :
                pattxx::dispatcher(t_node),
                ModuleDeclaration(std::move(definition)) {
        }

        /**
         * pattxx::dispatcher::dispatch with the additional function of setting the module accordingly.
         * @tparam Instance
         * @param t_name
         * @return
         */
        template<class FutureDispatch, class Instance = typename FutureDispatch::instance_type, typename... Args>
        Instance &misa_dispatch(FutureDispatch &t_dispatch, Args &&... args) {
            auto &inst = dispatch<Instance>(t_dispatch.name, static_cast<ModuleDeclaration *>(this), std::forward<Args>(args)...);
            return inst;
        }

    protected:

        /**
         * Declares a future dispatch.
         * @tparam Instance
         * @param t_name
         * @return
         */
        template<class Instance> misa_future_dispatch<Instance> future_dispatch(std::string t_name) {
            return misa_future_dispatch<Instance>(std::move(t_name));
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
            // Dispatch the module and tell the submodule holder
            auto &instance = dispatch<Module>(t_submodule.name, std::move(t_submodule.definition()));
            t_submodule.m_module = &instance;
            return instance;
        }

        /**
         * Returns the module definition
         * @return
         */
        ModuleDeclaration &module() override {
            return *this;
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
    };
}
