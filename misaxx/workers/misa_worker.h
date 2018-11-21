//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <misaxx/workers/misa_worker_base.h>
#include <misaxx/workers/task_tree/misa_work_node.h>
#include <misaxx/workers/paths/object_node_path.h>
#include <misaxx/workers/paths/algorithm_node_path.h>
#include <misaxx/parameters/input_parameter.h>
#include <misaxx/parameters/output_parameter.h>

namespace misaxx {
    /**
     * Base class for all MISA++ workers
     * @tparam ModuleDeclaration
     */
    template<class ModuleDeclaration> struct misa_worker : public misa_worker_base {
        using module_type = ModuleDeclaration;

        /**
        * Input variable that has a built-in function to check if the value is valid.
        * The value can be invalid if the underlying value is not accessed.
        * If the value is accessed and it is invalid, an exception will be thrown.
        */
        template<typename T, class InputCheckTag = parameters::default_check> using input = parameters::input_parameter<T, InputCheckTag>;

        /**
         * Output variable that has a built-in function to check if the value exists and an additional
         * function to prepare creating the output file.
         * The validation check is used by the runtime to determine if a skippable_task is skippable.
         */
        template<typename T, class OutputCheckTag = parameters::default_check, class OutputPrepareTag = parameters::default_prepare> using output = parameters::output_parameter<T, OutputCheckTag, OutputPrepareTag>;


        explicit misa_worker(const std::shared_ptr<nodes::misa_work_node> &t_node, const std::shared_ptr<ModuleDeclaration> &t_module_declaration) :
        m_node(t_node), m_module_declaration(t_module_declaration) {
        }

        /**
         * Returns a pointer to the module that this worker is working on
         * @return
         */
        virtual std::shared_ptr<ModuleDeclaration> module() {
            return m_module_declaration.lock();
        }

        /**
         * Gets the node that this worker is associated to
         * @return
         */
        std::shared_ptr<nodes::misa_work_node> get_node() const override {
            return m_node.lock();
        }

        /**
         * Rejects the workload. It will be tried again later.
         */
        void reject_work() override {
            get_node()->reject_work();
        }

        /**
        * Gets a parameter value from name. If the value is not defined in JSON, an exception will be thrown.
        * @tparam T
        * @tparam InputCheckTag
        * @param t_name
        * @return
        */
        template<typename T, class Namespace, class InputCheckTag = parameters::default_check>
        input<T, InputCheckTag> from_json(const std::string &t_name, const misa_json_property <T> &t_metadata = misa_json_property<T>()) {
            auto path = get_node()->template get_custom_path<Namespace>();
            path.push_back(t_name);
            return get_node()->template make_parameter<input<T, InputCheckTag>>(misa_runtime_base::instance().get_json<T>(path, t_metadata));
        }

        /**
         * Gets a parameter value from name. If the value is not defined in JSON, return the default value instead.
         * @tparam T
         * @tparam InputCheckTag
         * @param t_name
         * @return
         */
        template<typename T, class Namespace, class InputCheckTag = parameters::default_check>
        input<T, InputCheckTag> from_json_or(const std::string &t_name, T t_default = T(), misa_json_property <T> t_metadata = misa_json_property<T>()) {
            t_metadata.default_value = std::move(t_default);
            auto path = get_node()->template get_custom_path<Namespace>();
            path.push_back(t_name);
            return get_node()->template make_parameter<input<T, InputCheckTag>>(misa_runtime_base::instance().get_json<T>(path, t_metadata));
        }

        /**
         * Creates an input<T> instance that is not checked.
         * Use the output as input for output<T> or another input<T>
         * @tparam T
         * @tparam InputCheckTag
         * @param t_value
         * @return
         */
        template<class T, class InputCheckTag = parameters::no_check>
        input<T, InputCheckTag> define(T t_value) {
            return m_node.lock()->template make_parameter<input<T, InputCheckTag>>(std::move(t_value));
        }

        /**
        * Returns true if all input<> or output<> parameters in the list exist
        * Convenience wrapper around long if conditions.
        * @tparam Input
        * @tparam OtherInputs
        * @param t_input
        * @param t_other
        * @return
        */
        template<class Input, class... OtherInputs> bool parameters_exist(const Input &t_input, const OtherInputs&... t_other) {
            if constexpr (sizeof...(t_other) == 0) {
                return t_input.exists();
            }
            else {
                if(!t_input.exists())
                    return false;
                return parameters_exist(t_other...);
            }
        }

        /**
        * Loads a parameter from the object namespace. Global in the whole subtree of the object.
        * @tparam T
        * @tparam InputCheckTag
        * @param t_name
        * @param t_metadata
        * @return
        */
        template<typename T, class InputCheckTag = misaxx::parameters::default_check> auto from_object_json(const std::string &t_name, const misa_json_property<T> &t_metadata = misa_json_property<T>()) {
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
        template<typename T, class InputCheckTag = misaxx::parameters::default_check> auto from_object_json_or(const std::string &t_name, T t_default = T(), const misa_json_property<T> &t_metadata = misa_json_property<T>()) {
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
        template<typename T, class InputCheckTag = misaxx::parameters::default_check> auto from_algorithm_json(const std::string &t_name, const misa_json_property<T> &t_metadata = misa_json_property<T>()) {
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
        template<typename T, class InputCheckTag = misaxx::parameters::default_check> auto from_algorithm_json_or(const std::string &t_name, T t_default = T(), const misa_json_property<T> &t_metadata = misa_json_property<T>()) {
            return from_json_or<T, algorithm_node_path, InputCheckTag>(t_name, std::move(t_default), t_metadata);
        }

        /**
         * Loads a parameter from an auto-parameter
         * @tparam Parameter
         * @return
         */
        template<class Parameter, class InputCheckTag = misaxx::parameters::default_check> auto from_parameter() {
            return from_json<Parameter, typename Parameter::configuration_namespace_type, InputCheckTag> (Parameter().get_serialization_id().get_id());
        }

        /**
         * Loads a parameter from an auto-parameter
         * @tparam Parameter
         * @return
         */
        template<class Parameter, class InputCheckTag = misaxx::parameters::default_check> auto from_parameter_or(Parameter t_default = Parameter()) {
            return from_json_or<Parameter, typename Parameter::configuration_namespace_type, InputCheckTag> (Parameter().get_serialization_id().get_id(), std::move(t_default));
        }

    private:

        /**
         * The parent node the worker is connected to
         * Must be a raw pointer, as the  shared ptr is not ready during construction and we
         * get a bad_weak_ptr exception
         */
        std::weak_ptr<nodes::misa_work_node> m_node;

        /**
         * Pointer to the module declaration
         */
        std::weak_ptr<ModuleDeclaration> m_module_declaration;
    };
}
