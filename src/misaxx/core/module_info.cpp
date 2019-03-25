#include <misaxx/core/misa_mutable_module_info.h>
#include <misaxx/core/module_info.h>

misaxx::misa_module_info misaxx::module_info() {
    misaxx::misa_mutable_module_info info;
    info.set_id("misaxx");
    info.set_version("1.0.0");
    info.set_name("MISA++ core module");
    info.set_description("Base for all MISA++ modules");
    info.add_author("Ruman Gerst");
    info.set_license("BSD-2-Clause");
    info.set_organization("Leibniz Institute for Natural Product Research and Infection Biology - Hans Knöll Institute (HKI), Jena, Germany");
    info.set_url("https://asb-git.hki-jena.de/RGerst/misaxx-core/");

    // External dependency: Boost
    misaxx::misa_mutable_module_info boost_info;
    boost_info.set_id("boost");
    boost_info.set_version("1.67");
    boost_info.set_name("Boost Libraries");
    boost_info.set_url("https://www.boost.org/");
    boost_info.set_organization("Boost Community");
    boost_info.set_authors({"Beman Dawes", "David Abrahams", "Rene Rivera"});
    boost_info.set_license("BSL-1.0");
    boost_info.set_is_external(true);
    info.add_dependency(std::move(boost_info));

    // External dependency: OpenMP
    misaxx::misa_mutable_module_info openmp_info;
    openmp_info.set_id("openmp");
    openmp_info.set_version("4.5");
    openmp_info.set_name("OpenMP");
    openmp_info.set_url("https://www.openmp.org/");
    openmp_info.set_organization("OpenMP ARB");
    openmp_info.set_citation("Dagum, Leonardo, and Ramesh Menon. \"OpenMP: An industry-standard API for shared-memory programming.\" Computing in Science & Engineering 1 (1998): 46-55.");
    openmp_info.set_is_external(true);
    info.add_dependency(std::move(openmp_info));

    // External dependency: JSON for modern C++
    misaxx::misa_mutable_module_info nlohmann_json_info;
    nlohmann_json_info.set_id("nlohmann-json");
    nlohmann_json_info.set_name("JSON for Modern C++");
    nlohmann_json_info.set_version("3.5.0");
    nlohmann_json_info.set_url("https://github.com/nlohmann/json");
    nlohmann_json_info.add_author("Niels Lohmann");
    nlohmann_json_info.set_is_external(true);
    info.add_dependency(std::move(nlohmann_json_info));

    return info;
}