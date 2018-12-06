//
// Created by rgerst on 30.07.18.
//


#pragma once

#include <misaxx/parameters/parameter_checks.h>
#include <misaxx/parameters/default_parameter_check.h>
#include <misaxx/parameters/parameter_base.h>
#include <misaxx/workers/task_tree/misa_work_node.h>

namespace misaxx::parameters {

    /**
     * Acts as a wrapper around an input that might not exist, yet.
     *
     * InputCheckTag is a tag type that contains a type InputCheckTag::type<T>.
     * This sub-type should contain an operator (const T& value) const that returns a bool indicating if the value
     * exists.
     *
     * The current value of the input can only be obtained if it exists (which means that it was set and it exists according to the InputCheckTag).
     *
     * @tparam T Wrapped type
     * @tparam InputCheckTag Type that contains logic to determine if the input exists
     */
    template<typename T, class InputCheckTag = default_check > class input_parameter : public wrapped_input_parameter_base<T> {

    public:

        using value_type = T;
        using input_check_type = typename InputCheckTag::template type<value_type >;

        std::shared_ptr<T> m_value;
        std::weak_ptr<nodes::misa_work_node> m_node;

        static_assert(std::is_base_of<check_base, input_check_type >::value, "InputCheckTag points to incompatible class!");

        template<class OtherInputCheckTag> input_parameter(const input_parameter<T, OtherInputCheckTag> &t_src) : m_value(t_src.m_value), m_node(t_src.m_node) {

        }

        input_parameter(T value, const std::shared_ptr<nodes::misa_work_node> &t_node) : m_value(new T(std::move(value))), m_node(t_node) {
        }

        const T &value() const override {
            if(!input_check_type::check(*m_value))
                throw std::runtime_error("Value does not exist!");
            return *m_value;
        }

        operator T() {
            if(!input_check_type::check(*m_value))
                throw std::runtime_error("Value does not exist!");
            return *m_value;
        }

        input_parameter<T, InputCheckTag> & operator =(const T &value) {
            m_value = value;
            return *this;
        }

        input_parameter<T, InputCheckTag> & operator =(T &&value) {
            m_value = std::move(value);
            return *this;
        }

        bool exists() const {
            return m_value && input_check_type::check(*m_value);
        }

        /**
         * Returns the pointer to the node that created this input_parameter.
         * The node can be nullptr
         * @return
         */
        std::shared_ptr<nodes::misa_work_node> get_node() const override {
            return m_node.lock();
        }

    };

}