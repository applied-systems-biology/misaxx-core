//
// Created by rgerst on 01.11.18.
//


#pragma once

#include <misaxx/core/misa_data_pattern.h>
#include <misaxx/core/descriptions/misa_file_description.h>

namespace misaxx {

    /**
     * A pattern that can set the filename of misaxx::misa_file_description description types
     */
    struct misa_file_pattern : public misa_data_pattern {

        /**
         * An optional preset filename. If it is present, the pattern will always use this filename
         */
        boost::filesystem::path filename;

        /**
         * Extensions that the file can have
         * If empty, the pattern accepts any file
         * This has no effect if the filename is not empty
         */
        std::vector<boost::filesystem::path> extensions;

        misa_file_pattern() = default;

        explicit misa_file_pattern(std::vector<boost::filesystem::path> t_extensions);

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misa_json_schema_property &t_schema) const override;

        /**
         * Returns true if the pattern has already a filename
         * @return
         */
        bool has_filename() const;

        /**
         * Returns true if the pattern has a set of file extensions to look for
         * @return
         */
        bool has_extensions() const;

        /**
         * Returns true if the path matches the pattern
         * @param t_path
         * @return
         */
        bool matches(const boost::filesystem::path &t_path) const;

        /**
         * Sets the filename of the description
         * Requires that the pattern already has a filename
         * @param target
         */
        void apply(misa_file_description &target) const;

        /**
         * Applies the pattern and
         * sets the filename of the description
         * @param target
         * @param t_directory
         */
        void apply(misa_file_description &target, const boost::filesystem::path &t_directory) const;

    protected:
        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
    };

    inline void to_json(nlohmann::json& j, const misa_file_pattern& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_file_pattern& p) {
        p.from_json(j);
    }
}
