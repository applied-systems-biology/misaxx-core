//
// Created by rgerst on 30.07.18.
//


#pragma once

#include <vector>
#include <string>
#include <boost/algorithm/string/join.hpp>
#include <misaxx/json/misa_json_property.h>
#include <nlohmann/json.hpp>
#include <misaxx/json/misa_json_schema_builder.h>
#include <misaxx/misa_serializeable.h>
#include <misaxx/misa_primitive.h>

namespace misaxx {

    /**
     * The runtime is responsible for executing the work represented by the worker tree and providing external parameters such as
     * JSON parameters.
     * This is the interface containing common functions for different runtime implementations.
     */
    struct misa_runtime_base {

        /**
         * Returns the JSON that contains the parameters
         * @return
         */
        virtual nlohmann::json &get_parameter_json() = 0;

        /**
         * Gets the raw JSON value of a path
         * @param t_path
         * @return
         */
        virtual nlohmann::json get_json_raw(const std::vector<std::string> &t_path) = 0;

        /**
         * Returns an optional instance to a schema builder.
         * If it returns nullptr, the schema builder will be ignored.
         * @return
         */
        virtual misa_json_schema_builder &get_schema_builder() = 0;

        /**
         * Gets the converted JSON value of given path. If the value is not defined in JSON, an exception is thrown.
         * @tparam T
         * @tparam Converter Converter that should be used
         * @param t_path
         * @return
         */
        template<typename T> T get_json(const std::vector<std::string> &t_path, const misa_json_property<T> &t_json_metadata) {

            if(is_simulating()) {
                get_schema_builder().insert<T>(t_path, t_json_metadata);
                if constexpr (std::is_base_of<misa_serializeable, T>::value) {
                    T().to_json_schema(misa_json_schema(get_schema_builder(), t_path));
                }
                else {
                    misa_primitive<T> v;
                    v.metadata = t_json_metadata;
                    v.to_json_schema(misa_json_schema(get_schema_builder(), t_path));
                }
            }

            auto json = get_json_raw(t_path);
            if(json.empty()) {
                if(t_json_metadata.default_value) {
                    return t_json_metadata.default_value.value();
                }
                else if(is_simulating()) {
                    return T();
                }
                else {
                    throw std::runtime_error(boost::algorithm::join(t_path, " / ") + " does not exist!");
                }
            }
            else {
                return json.get<T>();
            }
        }

        /**
         * Returns true if the runtime is in simulation mode
         * @return
         */
        bool is_simulating() const {
            return m_is_simulating;
        }

    protected:

        bool m_is_simulating = false;

    protected:

        /**
         * Protected constructor for singleton
         */
        misa_runtime_base() = default;

        virtual ~misa_runtime_base() = default;

    private:

        /**
         * Static singleton instance of the runtime
         */
        static inline misa_runtime_base *singleton_instance = nullptr;

    public:

        static misa_runtime_base &instance() {
            if(singleton_instance) {
                return *singleton_instance;
            }
            else {
                throw std::runtime_error("Runtime is not initialized!");
            }
        }

        /**
         * Instantiates the runtime
         * @tparam Runtime
         */
        template<class Runtime> struct remote_ptr {
            template<class... Args> explicit remote_ptr(Args&&... args) {
                singleton_instance = new Runtime(std::forward<Args>(args)...);
            }

            ~remote_ptr() {
                singleton_instance = nullptr;
            }

            Runtime &operator*() const {
                return *dynamic_cast<Runtime*>(singleton_instance);
            }

            Runtime *operator->() const {
                return dynamic_cast<Runtime*>(singleton_instance);
            }
        };

    };

}