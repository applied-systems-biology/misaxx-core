//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <pattxx/dispatcher.h>
#include "misaxx/misa_module_declaration_base.h"
#include "misa_worker.h"
#include "misaxx/misa_module_base.h"
#include "misaxx/parameters/algorithm_node_path.h"
#include "misaxx/parameters/object_node_path.h"
#include "misaxx/misa_future_dispatch.h"

namespace misaxx {

    /**
     * MISA++ dispatcher base class.
     * @tparam ModuleDeclaration
     */
    template<class ModuleDeclaration> struct misa_dispatcher : public pattxx::dispatcher, public misa_worker<ModuleDeclaration> {
        static_assert(std::is_base_of<misa_module_declaration_base, ModuleDeclaration>::value, "Template argument must be a module definition!");

        /**
         * A future dispatch instance
         */
        template<class Instance> using dispatched = misa_future_dispatch<misa_dispatcher<ModuleDeclaration>, Instance>;

        explicit misa_dispatcher(pattxx::nodes::node *t_node, ModuleDeclaration *t_module) : pattxx::dispatcher(t_node), m_module(t_module) {

        }

        /**
         * Returns the module definition
         * @return
         */
        const ModuleDeclaration &module() const override {
            return *m_module;
        }

    protected:

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

        template<class FutureDispatch, class Instance = typename FutureDispatch::result_type>
        Instance &misa_dispatch(FutureDispatch &t_dispatch) {
            return t_dispatch.dispatch_specific(*this);
        }

        template<class Instance>
        dispatched<Instance> future_dispatch(const std::string &t_name) {
            dispatched<Instance> result;
            result.name = t_name;
            result.function = [t_name](misa_dispatcher<ModuleDeclaration> &t_worker) -> Instance & {
                return t_worker.template dispatch<Instance>(t_name, &t_worker);
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

    public:

        virtual void misa_init() = 0;

        virtual void misa_simulate() {
            for(const auto &f : m_future_dispatched) {
                misa_dispatch(f);
            }
        }

        void init() override {
            if(get_node().get_runtime().is_building_schema())
                misa_simulate();
            else
                misa_init();
        }

    private:

        ModuleDeclaration *m_module;

        /**
         * Lists all future dispatched entries for later use in misa_simulate()
         */
        std::vector<dispatched<pattxx::worker>> m_future_dispatched;
    };
}
