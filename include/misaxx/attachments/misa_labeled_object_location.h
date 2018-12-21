//
// Created by rgerst on 21.12.18.
//


#pragma once

#include <misaxx/attachments/misa_location.h>

namespace misaxx {

    /**
     * Location of an object within an image that is labeled with an integer
     */
    struct misa_labeled_object_location : public misa_location {

        /**
         * Label of the object within the image
         */
        int label = 0;

        misa_labeled_object_location() = default;

        explicit misa_labeled_object_location(int t_label);

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(const misa_json_schema &t_schema) const override;

    protected:

        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;

    };
}
