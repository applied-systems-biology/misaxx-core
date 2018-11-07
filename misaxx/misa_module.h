//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <pattxx/dispatcher.h>
#include "misaxx/workers/misa_worker.h"
#include "misa_module_declaration_base.h"
#include "misa_module_base.h"
#include "misa_submodule.h"
#include "misaxx/parameters/algorithm_node_path.h"
#include "misaxx/parameters/object_node_path.h"
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
        template<class Instance> using dispatched = misa_future_dispatch<misa_module<ModuleDeclaration>, Instance>;

        using dispatched_base = misa_future_dispatch_base<misa_module<ModuleDeclaration>>;

        static_assert(std::is_base_of<misa_module_declaration_base, ModuleDeclaration>::value, "misa_module only accepts module definitions as template parameter!");

        explicit misa_module(pattxx::nodes::node *t_node, ModuleDeclaration definition) :
                pattxx::dispatcher(t_node),
                ModuleDeclaration(std::move(definition)) {
        }

    protected:

        template<class Instance>
        dispatched<Instance> future_dispatch(const std::string &t_name) {
            dispatched<Instance> result;
            result.name = t_name;
            result.function = [t_name](misa_module<ModuleDeclaration> &t_worker) -> Instance & {
                return t_worker.template dispatch<Instance>(t_name, &t_worker);
            };
            m_future_dispatched.push_back(result);
            return result;
        }

        template<class Submodule, class Module = typename Submodule::module_type>
        dispatched<Module> future_dispatch(Submodule &t_submodule) {
            dispatched<Module> result;
            result.name = t_submodule.name;
            result.function = [&t_submodule](misa_module<ModuleDeclaration> &t_worker) -> Module & {
                if (t_submodule.has_instance())
                    throw std::runtime_error("The submodule already has been instantiated!");
                // Dispatch the module and tell the submodule holder
                auto &instance = t_worker.template dispatch<Module>(t_submodule.name, std::move(t_submodule.definition()));
                t_submodule.m_module = &instance;
                return instance;
            };
            m_future_dispatched.push_back(result);
            return result;
        }

        template<class InstanceBase>
        dispatched<InstanceBase> select(const std::string &t_name, const std::vector<dispatched<InstanceBase>> &t_values) {
            for (const auto &v : t_values) {
                if (v.name == t_name) {
                    return v;
                }
            }
            throw std::runtime_error("Unknown instance with name " + t_name);
        }

        template<class InstanceBase>
        dispatched<InstanceBase>
        select_from_algorithm_json(const std::string &t_param_name, const std::vector<dispatched<InstanceBase>> &t_values) {
            metadata <std::string> m;
            for(const auto &v : t_values) {
                m.allowed_values.push_back(v.name);
            }
            std::string n = from_algorithm_json<std::string>(t_param_name, std::move(m));
            return select<InstanceBase>(n, t_values);
        }

        template<class InstanceBase>
        dispatched<InstanceBase>
        select_from_algorithm_json_or(const std::string &t_param_name, const std::string &t_default, const std::vector<dispatched<InstanceBase>> &t_values) {
            metadata <std::string> m;
            for(const auto &v : t_values) {
                m.allowed_values.push_back(v.name);
            }
            std::string n = from_algorithm_json_or<std::string>(t_param_name, t_default, std::move(m));
            return select<InstanceBase>(n, t_values);
        }

        template<class FutureDispatch, class Instance = typename FutureDispatch::result_type>
        Instance &misa_dispatch(const FutureDispatch &t_dispatch) {
            return t_dispatch.dispatch_specific(*this);
        }

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
        auto from_object_json(const std::string &t_name, const metadata <T> &t_metadata = metadata<T>()) {
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
        auto from_object_json_or(const std::string &t_name, T t_default = T(), const metadata <T> &t_metadata = metadata<T>()) {
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
        auto from_algorithm_json(const std::string &t_name, const metadata <T> &t_metadata = metadata<T>()) {
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
        auto from_algorithm_json_or(const std::string &t_name, T t_default = T(), const metadata <T> &t_metadata = metadata<T>()) {
            return from_json_or<T, algorithm_node_path, InputCheckTag>(t_name, std::move(t_default), t_metadata);
        }

        /**
         * Loads a parameter from an auto-parameter
         * @tparam Parameter
         * @return
         */
        template<class Parameter, class InputCheckTag = pattxx::parameters::default_check>
        auto from_parameter() {
            return from_json<Parameter, typename Parameter::configuration_namespace_type, InputCheckTag>(Parameter().get_serialization_id());
        }

        /**
         * Loads a parameter from an auto-parameter
         * @tparam Parameter
         * @return
         */
        template<class Parameter, class InputCheckTag = pattxx::parameters::default_check>
        auto from_parameter_or(Parameter t_default = Parameter()) {
            return from_json_or<Parameter, typename Parameter::configuration_namespace_type, InputCheckTag>(Parameter().get_serialization_id(), std::move(t_default));
        }

    public:

        virtual void misa_init() = 0;

        virtual void misa_simulate() {
            for (const auto &f : m_future_dispatched) {
                misa_dispatch(f);
            }
        }

        void init() override {
            if (pattxx::runtime::simulation_mode)
                misa_simulate();
            else
                misa_init();
        }

    private:

        /**
         * Lists all future dispatched entries for later use in misa_simulate()
         */
        std::vector<dispatched<pattxx::worker>> m_future_dispatched;

    };
}
