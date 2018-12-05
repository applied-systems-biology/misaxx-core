//
// Created by rgerst on 14.11.18.
//

#pragma once

#include <misaxx/misa_serializeable.h>

namespace misaxx {
    /**
     * Attachment that links the attachment container to a filesystem entry.
     * This attachment is automatically generated by misa_cli when attachments are exported.
     */
    struct misa_filesystem_link : public misa_serializeable {

        boost::filesystem::path internal_path;

        void from_json(const nlohmann::json &t_json) override {
            internal_path = t_json.get<std::string>();
        }

        void to_json(nlohmann::json &t_json) const override {
            misa_serializeable::to_json(t_json);
            t_json = internal_path.string();
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            t_schema.declare_required<std::string>();
        }

        misa_serialization_id get_serialization_id() const override {
            return misa_serialization_id("misa", "filesystem-link");
        }

        std::vector<misa_serialization_id> get_serialization_id_hierarchy() const override {
            return { get_serialization_id() };
        }
    };

    void to_json(nlohmann::json& j, const misa_filesystem_link& p) {
        p.to_json(j);
    }

    void from_json(const nlohmann::json& j, misa_filesystem_link& p) {
        p.from_json(j);
    }
}




