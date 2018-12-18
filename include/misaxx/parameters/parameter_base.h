//
// Created by rgerst on 30.07.18.
//


#pragma once

#include <memory>

namespace misaxx::nodes {
    class misa_work_node;
}

namespace misaxx::parameters {

    /**
     * Base class for output parameter
     */
    struct output_parameter_base {
        virtual bool exists() const = 0;
        virtual std::shared_ptr<nodes::misa_work_node> get_node() const = 0;
    };

    /**
     * Base class for an input parameter
     */
    struct input_parameter_base {
        virtual std::shared_ptr<nodes::misa_work_node> get_node() const = 0;
    };

    /**
     * Necessary wrapper type to interact between input_parameter -> output_parameter
     * @tparam T
     */
    template<typename T> struct wrapped_input_parameter_base : public input_parameter_base {
        virtual const T &value() const = 0;
    };

}