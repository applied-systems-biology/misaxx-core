//
// Created by rgerst on 28.02.19.
//

#include <fstream>
#include <misaxx/core/misa_module_info.h>
#include <misaxx/core/runtime/misa_runtime_properties.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "misa_readme_builder.h"
#include "src/misaxx/core/utils/markdown.h"

using namespace misaxx;

namespace {

    struct cache_info {
        std::string path;
        nlohmann::json description;
        nlohmann::json pattern;

        std::string get_name() const {
            if(description.find("misa:documentation-title") != description.end()) {
                return description.at("misa:documentation-title").get<std::string>();
            }
            else {
                return description.at("misa:serialization-id").get<std::string>();
            }
        }

        std::string get_description() const {
            if(description.find("misa:documentation-description") != description.end()) {
                return description.at("misa:documentation-description").get<std::string>();
            }
            else {
                return "No description provided";
            }
        }
    };

    struct parameter_info {
        std::string path;
        nlohmann::json property;

        std::string get_name() const {
            if(property.find("misa:documentation-title") != property.end()) {
                return property.at("misa:documentation-title").get<std::string>();
            }
            return "";
        }

        std::string get_description() const {
            if(property.find("misa:documentation-description") != property.end()) {
                return property.at("misa:documentation-description").get<std::string>();
            }
            return "";
        }

        std::string get_type() const {
            return property.at("type").get<std::string>();
        }

        std::string get_default() const {
            if(property.find("default") != property.end()) {
                std::stringstream w;
                w << property.at("default");
                return w.str();
            }
            return "";
        }

        std::string get_allowed_values() const {
            if(property.find("enum") != property.end()) {
                std::stringstream w;
                w << property.at("enum");
                return w.str();
            }
            return "";
        }

        bool is_interesting() const {
            return !(get_name().empty() && get_description().empty() && get_default().empty() && get_allowed_values().empty());
        }
    };

    inline std::string readme_basic_parameter_file() {
        nlohmann::json json;
        json["filesystem"]["input-directory"] = "<Input directory>";
        json["filesystem"]["output-directory"] = "<Output directory>";
        json["filesystem"]["source"] = "directories";
        json["samples"]["My sample 1"] = nlohmann::json::object({ });
        json["samples"]["My sample 2"] = nlohmann::json::object({ });

        std::stringstream w;
        w << std::setw(4) << json;
        return w.str();
    }

    inline std::optional<nlohmann::json> extract_property(const nlohmann::json &json, const std::vector<std::string> &path) {
        const nlohmann::json* current = &json;
        for(const auto &entry : path) {
            auto it = current->find(entry);
            if(it == current->end()) {
                return std::nullopt;
            }
            else {
                current = &(*it);
            }
        }
        return *current;
    }


    inline void extract_caches(const nlohmann::json &entry, const std::string &path, std::vector<cache_info> &result) {
        if(!entry.is_object())
            return;
        if(entry.find("misa:serialization-id") != entry.end() && entry.at("misa:serialization-id").get<std::string>() == "misa:filesystem/entry") {
            std::optional<nlohmann::json> fs_opt = extract_property(entry, { "properties", "metadata", "properties", "description" });
            if(fs_opt.has_value()) {
                std::optional<nlohmann::json> fs_pattern = extract_property(entry, { "properties", "metadata", "properties", "pattern" });

                cache_info info;
                info.description = fs_opt.value();
                info.path = path;

                if(fs_pattern.has_value())
                    info.pattern = fs_pattern.value();

                result.emplace_back(std::move(info));
            }
        }
        for(auto it = entry.begin(); it != entry.end(); ++it) {
            if(it.value().is_object()) {

                std::string sub_path = path;
                if(it.value().find("misa:serialization-id") != it.value().end() &&
                        it.value().at("misa:serialization-id").get<std::string>() == "misa:filesystem/entry") {
                    sub_path += it.key() + "/";
                }

                extract_caches(it.value(), sub_path, result);
            }
        }
    }

    inline void write_filesystem_readme(markdown::document &doc, const nlohmann::json &schema) {
        using namespace markdown;

        std::optional<nlohmann::json> fs_opt = extract_property(schema, { "properties", "filesystem", "properties", "json-data", "properties",
                                                                          "imported", "properties", "children", "additionalProperties" });
        if(fs_opt.has_value()) {
            doc += heading2("Input files");
            doc += paragraph(text("The application expects that the input folder follows a specific structure. "),
                             text("Please put the input data into their respective folders or create symbolic links if you want to avoid copying already existing data."));
            doc += paragraph(text("The input folder should contain sub-folders that match the sample names provided in the parameter file. "),
                             text("Specific folders are used as input data. See the following table for the full filesystem structure and which kind of data is expected within those folders:"));

            std::vector<cache_info> caches;
            extract_caches(fs_opt.value(), "<Input folder>/<Sample>/", caches);

            table<2> tbl;
            tbl += row(text("Path"), text("Data type"));

            for(const auto &cache : caches) {
                tbl += row(inline_code(cache.path), text(cache.get_name()));
            }

            doc += cut(tbl);
        }
        else {
            doc += heading2("Input files");
            doc += paragraph(text("This module has no input files. Please provide an input directory that only contains empty directories corresponding to the samples."));
        }
    }

    inline void write_output_filesystem_readme(markdown::document &doc, const nlohmann::json &schema) {
        using namespace markdown;

        std::optional<nlohmann::json> fs_opt = extract_property(schema, { "properties", "filesystem", "properties", "json-data", "properties",
                                                                          "exported", "properties", "children", "additionalProperties" });
        if(fs_opt.has_value()) {
            doc += heading2("Output files");
            doc += paragraph(text("Following files and directories are generated into the output folder:"));

            std::vector<cache_info> caches;
            extract_caches(fs_opt.value(), "<Output folder>/<Sample>/", caches);

            table<2> cache_tbl;
            cache_tbl += row(text("Path"), text("Data type"));

            for(const auto &cache : caches) {
                cache_tbl += row(inline_code(cache.path), text(cache.get_name()));
            }

            table<2> tbl;
            tbl += row(text("Path"), text("Description"));
            tbl += row(inline_code("<Output folder>/parameters.json"), text("A copy of the parameter file"));
            tbl += row(inline_code("<Output folder>/parameter-schema.json"), text("Human- and machine-readable schema that describes the parameter file, input and output files"));
            tbl += row(inline_code("<Output folder>/runtime-log.json"), text("Contains information about the performance"));
            tbl += row(inline_code("<Output folder>/attachments/"), text("Additional info attached to data during calculation. The folder structure follows the structure of input and output files"));
            tbl += row(inline_code("<Output folder>/attachments/serialization-schemas.json"), text("Description of attached object types"));

            doc += cut(cache_tbl);
            doc += paragraph(text("The application will also generate following output files:"));
            doc += cut(tbl);
        }
        else {
            doc += heading2("Output files");
            doc += paragraph(text("This module has no input files. Please provide an input directory that only contains empty directories corresponding to the samples."));
        }
    }

    inline void write_cache_readme(markdown::document &doc, const nlohmann::json &schema) {
        std::vector<cache_info> caches;
        std::optional<nlohmann::json> fs_opt_imported = extract_property(schema, { "properties", "filesystem", "properties", "json-data", "properties",
                                                                          "imported", "properties", "children", "additionalProperties" });
        std::optional<nlohmann::json> fs_opt_exported = extract_property(schema, { "properties", "filesystem", "properties", "json-data", "properties",
                                                                          "exported", "properties", "children", "additionalProperties" });
        if(fs_opt_imported.has_value()) {
            extract_caches(fs_opt_imported.value(), "/", caches);
        }
        if(fs_opt_imported.has_value()) {
            extract_caches(fs_opt_exported.value(), "/", caches);
        }
        if(!caches.empty()) {
            using namespace markdown;
            doc += heading2("Data types");
            doc += paragraph(text("Input and output data have a "), bold("data type"), text(" that determines which kind of files should be put into the folder or "),
                    text("which files are generated as output."));

            std::unordered_map<std::string, cache_info> infos;
            for(const auto &info : caches) {
                infos[info.get_name()] = info;
            }

            table<2> tbl;
            tbl += row(text("Data type"), text("Description"));

            for(const auto &kv : infos) {
                tbl += row(text(kv.first), text(kv.second.get_description()));
            }

            doc += cut(tbl);
        }
    }

    inline nlohmann::json schema_to_json(const nlohmann::json &schema) {
        if(schema.at("type") == "object") {
            nlohmann::json result = nlohmann::json::object({ });
            if(schema.find("properties") != schema.end()) {
                for(auto it = schema.at("properties").begin(); it != schema.at("properties").end(); ++it) {
                    result[it.key()] = schema_to_json(it.value());
                }
            }
            return result;
        }
        else if(schema.find("default") != schema.end()) {
            return schema.at("default");
        }
        else {
            return nlohmann::json("<TODO: " + schema.at("type").get<std::string>() + " value>");
        }
    }

    inline void extract_parameters(const nlohmann::json &schema, const std::string &path, std::vector<parameter_info> &result) {
        parameter_info info;
        info.path = path;
        info.property = schema;
        result.emplace_back(std::move(info));

        if(schema.at("type") == "object") {
            if(schema.find("properties") != schema.end()) {
                for(auto it = schema.at("properties").begin(); it != schema.at("properties").end(); ++it) {
                    extract_parameters(it.value(), path + "/" + it.key(), result);
                }
            }
        }
    }

    inline std::string full_example_json(const nlohmann::json &schema) {
        nlohmann::json json {};
        json["filesystem"]["input-directory"] = "<Input directory>";
        json["filesystem"]["output-directory"] = "<Output directory>";
        json["filesystem"]["source"] = "directories";

        auto algorithm_schema = extract_property(schema, { "properties", "algorithm" });
        if(algorithm_schema.has_value()) {
            json["algorithm"] = schema_to_json(algorithm_schema.value());
        }

        auto sample_schema = extract_property(schema, {"properties", "samples", "additionalProperties" });
        if(sample_schema.has_value()) {
            json["samples"]["<Sample>"] = schema_to_json(sample_schema.value());
        }

        auto runtime_schema = extract_property(schema, {"properties", "runtime" });
        if(runtime_schema.has_value()) {
            json["runtime"] = schema_to_json(runtime_schema.value());
        }

        std::stringstream w;
        w << std::setw(4) << json;
        return w.str();
    }

    inline void write_parameters_readme(markdown::document &doc, const nlohmann::json &schema) {
        using namespace markdown;
        doc += heading2("Full parameters");
        doc += paragraph(text("The parameter file contains additional parameters that can change the behaviour of algorithms, "
                              "provide them with additional sample parameters and change internal runtime settings. "
                              "Following example parameter file contains all available settings and their default values:"));
        doc += code(full_example_json(schema), "json");


        doc += paragraph(text("Following list provides a documentation for each parameter:"));

        std::vector<parameter_info> all_parameters;
        auto algorithm_schema = extract_property(schema, { "properties", "algorithm" });
        if(algorithm_schema.has_value()) {
            extract_parameters(algorithm_schema.value(), "/algorithm", all_parameters);
        }

        auto sample_schema = extract_property(schema, {"properties", "samples", "additionalProperties" });
        if(sample_schema.has_value()) {
            extract_parameters(sample_schema.value(), "/samples/<Sample>", all_parameters);
        }

        auto runtime_schema = extract_property(schema, {"properties", "runtime" });
        if(runtime_schema.has_value()) {
            extract_parameters(runtime_schema.value(), "/runtime", all_parameters);
        }


        table<6> tbl;
        tbl += row(text("Parameter"), text("Name"), text("Description"), text("Type"), text("Default"), text("Allowed values"));
        for(const auto &info : all_parameters) {
            if(info.is_interesting())
                tbl += row(inline_code(info.path), text(info.get_name()), text(info.get_description()), text(info.get_type()),
                        inline_code(info.get_default()), inline_code(info.get_allowed_values()));
        }

        doc += cut(tbl);
    }

}

void misaxx::build_readme(const nlohmann::json &schema, const boost::filesystem::path &t_output_path) {

    misa_module_info info = runtime_properties::get_module_info();

    using namespace markdown;
    document doc;
    doc += heading(info.get_name());
    doc += paragraph(text(info.get_description()));
    doc += paragraph(italic(info.get_id() + ", version " + info.get_version()));

    // General usage
    doc += heading2("Usage");
    doc += paragraph(text("To run the module, execute following command:"));
    doc += code("./" + info.get_id() + " --parameters <parameter file>", "sh");

    // General parameter file
    doc += paragraph(text("A basic parameter file has the following format:"));
    doc += code(readme_basic_parameter_file(), "json");
    doc += paragraph(text("The parameter file must at least provide an input directory, an output directory and the list of samples. "),
            text("Additionally, there might be required parameters. See below to find a list of all optional and required parameters."));

    // Multithreading
    doc += paragraph(text("To enable multithreading, provide the number of threads as CLI parameter:"));
    doc += code("./" + info.get_id() + " --parameters <parameter file> --threads <Number of threads>", "sh");

    // Filesystem structure
    write_filesystem_readme(doc, schema);

    // Output files
    write_output_filesystem_readme(doc, schema);

    // Cache docs
    write_cache_readme(doc, schema);

    // Parameters
    write_parameters_readme(doc, schema);

    // Write the README
    {
        std::ofstream w;
        w.open(t_output_path.string());
        w << doc.to_string();
    }
}
