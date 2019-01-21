//
// Created by rgerst on 15.01.19.
//

#pragma once

#include <misaxx/core/misa_parameter.h>

namespace misaxx {

    class misa_worker;

    /**
     * Allows creating parameters for misa_workers and read-only access to them
     */
    struct misa_parameter_builder {

        explicit misa_parameter_builder(misa_worker &t_worker);

        /**
         * Creates a parameter that is equal across all samples
         * @tparam T
         * @param t_name
         * @param t_default
         * @param t_metadata
         * @return
         */
        template<typename T> misa_parameter<T> create_algorithm_parameter(std::string t_name, T t_default, misa_json_property<T> t_metadata = misa_json_property<T>()) {
            t_metadata.default_value = std::move(t_default);
            auto path = get_algorithm_path();
            path.emplace_back(std::move(t_name));
            misaxx::parameter_registry::register_parameter(path, t_metadata);
            return misa_parameter<T>(std::move(path), std::move(t_metadata));
        }

        /**
         * Creates a parameter that is equal across all samples
         * @tparam T
         * @param t_name
         * @param t_metadata
         * @return
         */
        template<typename T> misa_parameter<T> create_algorithm_parameter(std::string t_name, misa_json_property<T> t_metadata = misa_json_property<T>()) {
            auto path = get_algorithm_path();
            path.emplace_back(std::move(t_name));
            misaxx::parameter_registry::register_parameter(path, t_metadata);
            return misa_parameter<T>(std::move(path), std::move(t_metadata));
        }

        /**
         * Creates a parameter that is global within the sample
         * @tparam T
         * @param t_name
         * @param t_default
         * @param t_metadata
         * @return
         */
        template<typename T> misa_parameter<T> create_sample_parameter(std::string t_name, T t_default, misa_json_property<T> t_metadata = misa_json_property<T>()) {
            t_metadata.default_value = std::move(t_default);
            auto path = get_sample_path();
            path.emplace_back(std::move(t_name));
            misaxx::parameter_registry::register_parameter(path, t_metadata);
            return misa_parameter<T>(std::move(path), std::move(t_metadata));
        }

        /**
         * Creates a parameter that is global within the sample
         * @tparam T
         * @param t_name
         * @param t_metadata
         * @return
         */
        template<typename T> misa_parameter<T> create_sample_parameter(std::string t_name,misa_json_property<T> t_metadata = misa_json_property<T>()) {
            auto path = get_sample_path();
            path.emplace_back(std::move(t_name));
            misaxx::parameter_registry::register_parameter(path, t_metadata);
            return misa_parameter<T>(std::move(path), std::move(t_metadata));
        }

        /**
       * Creates a global parameter
       * @tparam T
       * @param t_name
       * @param t_default
       * @param t_metadata
       * @return
       */
        template<typename T> misa_parameter<T> create_runtime_parameter(std::string t_name, T t_default, misa_json_property<T> t_metadata = misa_json_property<T>()) {
            t_metadata.default_value = std::move(t_default);
            auto path = get_runtime_path();
            path.emplace_back(std::move(t_name));
            misaxx::parameter_registry::register_parameter(path, t_metadata);
            return misa_parameter<T>(std::move(path), std::move(t_metadata));
        }

        /**
         * Creates a parameter that is global within the sample
         * @tparam T
         * @param t_name
         * @param t_metadata
         * @return
         */
        template<typename T> misa_parameter<T> create_runtime_parameter(std::string t_name,misa_json_property<T> t_metadata = misa_json_property<T>()) {
            auto path = get_runtime_path();
            path.emplace_back(std::move(t_name));
            misaxx::parameter_registry::register_parameter(path, t_metadata);
            return misa_parameter<T>(std::move(path), std::move(t_metadata));
        }

    private:

        std::weak_ptr<misa_worker> m_worker;

        std::vector<std::string> get_algorithm_path();

        std::vector<std::string> get_sample_path();

        std::vector<std::string> get_node_path();

        std::vector<std::string> get_runtime_path();
    };
}



