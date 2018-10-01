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
        template<class Instance> using dispatched = std::function<Instance &(misa_module<ModuleDeclaration> &)>;

        static_assert(std::is_base_of<misa_module_declaration_base, ModuleDeclaration>::value, "misa_module only accepts module definitions as template parameter!");

        explicit misa_module(pattxx::nodes::node *t_node, ModuleDeclaration definition) :
                pattxx::dispatcher(t_node),
                ModuleDeclaration(std::move(definition)) {
        }

    protected:

        /**
         * Declares a future dispatch.
         * @tparam Instance
         * @param t_name
         * @return
         */
        template<class Instance>
        dispatched<Instance> future_dispatch(const std::string &t_name) {
            // TODO: Log which dispatchers should be called
            return [t_name](misa_module<ModuleDeclaration> &t_worker) -> Instance & {
                return t_worker.template dispatch<Instance>(t_name, &t_worker);
            };
        }

        template<class Instance> misa_future_dispatch<Instance> option(std::string name) {
            return misa_future_dispatch<Instance>(std::move(name));
        }

        template<class InstanceBase, class FutureDispatcher, class... FutureDispatchers> dispatched <InstanceBase>
            future_dispatch_any_from_name(const std::string &t_name,const FutureDispatcher& disp, const FutureDispatchers&... args) {
            static_assert(std::is_base_of<InstanceBase, typename FutureDispatcher::instance_type>::value, "Future dispatchers must instantiate the base type!");
            // TODO: Log which dispatchers should be called
            if(disp.name == t_name) {
                return [t_name](misa_module<ModuleDeclaration> &t_worker) -> InstanceBase & {
                    return t_worker.template dispatch<typename FutureDispatcher::instance_type>(t_name, &t_worker);
                };
            }
            else {
                if constexpr (sizeof...(args) > 0) {
                    return future_dispatch_any_from_name<InstanceBase>(t_name, args...);
                }
                else {
                    throw std::runtime_error("Cannot find future dispatcher with name " + t_name);
                }
            }
        }

        template<class Submodule, class Module = typename Submodule::module_type>
        dispatched<Module> future_dispatch(Submodule &t_submodule) {
            // TODO: Log which dispatchers should be called
            return [&t_submodule](misa_module<ModuleDeclaration> &t_worker) -> Module & {
                if (t_submodule.has_instance())
                    throw std::runtime_error("The submodule already has been instantiated!");
                // Dispatch the module and tell the submodule holder
                auto &instance = t_worker.template dispatch<Module>(t_submodule.name, std::move(t_submodule.definition()));
                t_submodule.m_module = &instance;
                return instance;
            };
        }

        /**
         * pattxx::dispatcher::dispatch with the additional function of setting the module accordingly.
         * @tparam Instance
         * @param t_name
         * @return
         */
        template<class FutureDispatch, class Instance = typename FutureDispatch::result_type>
        Instance &misa_dispatch(const FutureDispatch &t_dispatch) {
            return t_dispatch(*this);
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
        template<typename T, class InputCheckTag = pattxx::parameters::default_check>
        auto from_object_json(const std::string &t_name, const metadata<T> &t_metadata = metadata<T>()) {
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
        template<typename T, class InputCheckTag = pattxx::parameters::default_check>
        auto from_object_json_or(const std::string &t_name, T t_default = T(), const metadata<T> &t_metadata = metadata<T>()) {
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
        template<typename T, class InputCheckTag = pattxx::parameters::default_check>
        auto from_algorithm_json(const std::string &t_name, const metadata<T> &t_metadata = metadata<T>()) {
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
        template<typename T, class InputCheckTag = pattxx::parameters::default_check>
        auto from_algorithm_json_or(const std::string &t_name, T t_default = T(), const metadata<T> &t_metadata = metadata<T>()) {
            return from_json_or<T, algorithm_node_path, InputCheckTag>(t_name, std::move(t_default), t_metadata);
        }

        /**
         * Loads a parameter from an auto-parameter
         * @tparam Parameter
         * @return
         */
        template<class Parameter, class InputCheckTag = pattxx::parameters::default_check>
        auto from_parameter() {
            return from_json<Parameter, typename Parameter::configuration_namespace_type, InputCheckTag>(Parameter().get_name());
        }

        /**
         * Loads a parameter from an auto-parameter
         * @tparam Parameter
         * @return
         */
        template<class Parameter, class InputCheckTag = pattxx::parameters::default_check>
        auto from_parameter_or(Parameter t_default = Parameter()) {
            return from_json_or<Parameter, typename Parameter::configuration_namespace_type, InputCheckTag>(Parameter().get_name(), std::move(t_default));
        }

    };
}
