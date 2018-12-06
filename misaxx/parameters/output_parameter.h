//
// Created by rgerst on 30.07.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include <misaxx/parameters/parameter_checks.h>
#include <misaxx/parameters/default_parameter_check.h>
#include <misaxx/parameters/parameter_prepare.h>
#include <misaxx/parameters/default_parameter_prepare.h>
#include <misaxx/parameters/parameter_base.h>
#include <misaxx/workers/task_tree/misa_work_node.h>

namespace misaxx::parameters {

    /**
     * Variable that is an output of a node. The library can use it to determine if a task can be skipped and
     * check if the output exists after a specific node finished its work.
     *
     * OutputCheckTag is a tag type that contains a type OutputCheckTag::type<T>.
     * This sub-type should contain an operator (const T& value) const that returns a bool indicating if the value
     * already exists.
     *
     * OutputPrepareTag is a tag type that contains a type OutputPrepareTag::type<T>.
     * This sub-type should contain an operator (const T& value) const that applies some action to prepare the output.
     * For example default_output_prepare creates the necessary parent directories.
     *
     * @tparam T
     * @tparam OutputCheckTag Type that contains logic to determine if the output already exists
     * @tparam OutputPrepareTag Type that contains logic indicating what to do when the output is initialized
     */
    template<typename T, class OutputCheckTag = default_check, class OutputPrepareTag = default_prepare> class output_parameter : public output_parameter_base {

    public:

        using value_type = T;
        using output_check_type = typename OutputCheckTag::template type<value_type >;
        using output_prepare_type = typename OutputPrepareTag::template type<value_type >;

        static_assert(std::is_base_of<check_base, output_check_type >::value, "OutputCheckTag points to incompatible class!");
        static_assert(std::is_base_of<prepare_base, output_prepare_type>::value, "OutputPrepareTag points to incompatible class!");

        explicit output_parameter(T value, const std::shared_ptr<nodes::misa_work_node> &t_node) : m_value(new T(std::move(value))), m_node(t_node) {
            output_prepare_type ::prepare(*m_value);
        }

        output_parameter(const wrapped_input_parameter_base<T> &t_source) : m_value(new T(t_source.value())), m_node(t_source.get_node()) {
            t_source.get_node()->register_output_parameter(std::make_unique<output_parameter<T, OutputCheckTag, OutputPrepareTag>>(*this));
        }

        const T &value() const {
            return *m_value;
        }

        operator T() {
            return *m_value;
        }

        bool exists() const override {
            return output_check_type::check(*m_value);
        }

        /**
         * Returns the pointer to the node that created this output_parameter.
         * The node can be nullptr
         * @return
         */
        std::shared_ptr<nodes::misa_work_node> get_node() const override {
            return m_node.lock();
        }

    private:

        std::shared_ptr<T> m_value;
        std::weak_ptr<nodes::misa_work_node> m_node;

    };

}