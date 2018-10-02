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

        template<class FutureDispatch, class Instance = typename FutureDispatch::result_type>
        Instance &misa_dispatch(FutureDispatch &t_dispatch) {
            return t_dispatch(*this);
        }

        template<class InstanceBase, class FutureDispatcher, class... FutureDispatchers> dispatched <InstanceBase>
        future_dispatch_any_from_name(const std::string &t_name,const FutureDispatcher& disp, const FutureDispatchers&... args) {
            static_assert(std::is_base_of<InstanceBase, typename FutureDispatcher::instance_type>::value, "Future dispatchers must instantiate the base type!");
            // TODO: Log which dispatchers should be called
            if(disp.name == t_name) {
                return [t_name](misa_dispatcher<ModuleDeclaration> &t_worker) -> InstanceBase & {
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

        template<class InstanceBase, class... FutureDispatchers> dispatched <InstanceBase>
        future_dispatch_any_from_algorithm_json(const std::string &t_param_name,  const FutureDispatchers&... args) {
            std::string n = from_algorithm_json<std::string>(t_param_name); // TODO: Fill metadata with enum values
            return future_dispatch_any_from_name<InstanceBase>(n, args...);
        }

        template<class InstanceBase, class... FutureDispatchers> dispatched <InstanceBase>
        future_dispatch_any_from_algorithm_json_or(const std::string &t_param_name, const std::string &t_default,  const FutureDispatchers&... args) {
            std::string n = from_algorithm_json_or<std::string>(t_param_name, t_default); // TODO: Fill metadata with enum values
            return future_dispatch_any_from_name<InstanceBase>(n, args...);
        }

    public:

        virtual void misa_init() = 0;

        virtual void misa_simulate() {
            // TODO: Dispatch from list
        }

        void init() override {
            misa_init();
        }

    private:

        ModuleDeclaration *m_module;
    };
}
