//
// Created by rgerst on 28.02.19.
//

#include <fstream>
#include <misaxx/core/misa_module_info.h>
#include <misaxx/core/runtime/misa_runtime_properties.h>
#include <sstream>
#include <iomanip>
#include "misa_readme_builder.h"
#include "src/misaxx/core/utils/markdown.h"

using namespace misaxx;

namespace {

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

    inline void write_filesystem_readme(markdown::document &doc, const nlohmann::json &schema) {
        using namespace markdown;
        doc += heading2("Input files");
        doc += paragraph(text("The application expects that the input folder follows a specific structure. "),
                text("Please put the input data into their respective folders or create symbolic links to avoid copying."));
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
