//
// Created by rgerst on 16.01.19.
//

#pragma once

#include <string>
#include <misaxx/core/misa_serializable.h>

namespace misaxx {
    /**
     * Contains basic information about a MISA++ module
     */
    class misa_module_info : public misa_serializable {
    public:
        misa_module_info() = default;

        /**
         * A short, lowercase name without spaces or special characters
         * @return
         */
        std::string get_id() const;

        /**
         * The version of the module
         * @return
         */
        std::string get_version() const;

        /**
         * A short, descriptive name of the module
         * @return
         */
        std::string get_name() const;

        /**
         * A description of the module
         * @return
         */
        std::string get_description() const;

        /**
         * The list of dependency modules
         * @return
         */
        std::vector<misa_module_info> get_dependencies() const;

        /**
         * Website of this module
         * @return
         */
        std::string get_url() const;

        /**
         * List of authors
         * @return
         */
        std::vector<std::string> get_authors() const;

        /**
         * Organization that developed this module
         * @return
         */
        std::string get_organization() const;

        /**
         * License of the module
         * @return
         */
        std::string get_license() const;

        /**
         * Information about how to cite the module
         * @return
         */
        std::string get_citation() const;

        /**
         * If true, an external non-MISA++ dependency is described
         * @return
         */
        bool is_external() const;

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misa_json_schema_property &t_schema) const override;

    protected:

        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;

        std::string m_id;
        std::string m_version;
        std::string m_name;
        std::string m_description;
        std::vector<misa_module_info> m_dependencies;

        // Additional metadata
        std::string m_url;
        std::vector<std::string> m_authors;
        std::string m_organization;
        std::string m_license;
        std::string m_citation;

        // External (non-MISA++) dependencies
        bool m_is_external = false;
    };

    inline void to_json(nlohmann::json& j, const misa_module_info& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_module_info& p) {
        p.from_json(j);
    }
}



