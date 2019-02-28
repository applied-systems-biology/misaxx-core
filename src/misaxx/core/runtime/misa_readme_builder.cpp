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
            std::optional<nlohmann::json> fs_opt = extract_property(entry, { "metadata", "properties", "description" });
            if(fs_opt.has_value()) {
                cache_info info;
                info.description = fs_opt.value();
                info.path = path;
            }
        }
        for(auto it = entry.begin(); it != entry.end(); ++it) {
            extract_caches(it.value(), path + "/" + it.key(), result);
        }
    }

    inline void write_filesystem_readme(markdown::document &doc, const nlohmann::json &schema) {
        using namespace markdown;

        std::optional<nlohmann::json> fs_opt = extract_property(schema, { "properties", "filesystem", "properties", "json-data", "properties",
                                                                          "exported", "properties", "children", "additionalProperties" });
        if(fs_opt.has_value()) {
            doc += heading2("Input files");
            doc += paragraph(text("The application expects that the input folder follows a specific structure. "),
                             text("Please put the input data into their respective folders or create symbolic links if you want to avoid copying already existing data."));
            doc += paragraph(text("The input folder should contain sub-folders that match the sample names provided in the parameter file. "),
                             text("Specific folders are used as input data. See the following table for the full filesystem structure and which kind of data is expected within those folders:"));

            std::vector<cache_info> caches;
            extract_caches(fs_opt.value(), "", caches);

            table<2> tbl;
            tbl += row(text("Path"), text("Data type"));

            doc += cut(tbl);
        }
        else {
            doc += heading2("Input files");
            doc += paragraph(text("This module has no input files. Please provide an input directory that only contains empty directories corresponding to the samples."));
        }
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

    // Write the README
    {
        std::ofstream w;
        w.open(t_output_path.string());
        w << doc.to_string();
    }
}
