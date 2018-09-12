//
// Created by rgerst on 30.07.18.
//


#include <iostream>
#include <fstream>
#include <misaxx/module_data/misa_file_stack.h>
#include <misaxx/misa_module.h>
#include <misaxx/misa_module_declaration.h>
#include <misaxx/misa_task.h>
#include <pattxx/default_runtime.h>
#include <misaxx/misa_runtime.h>
#include <misaxx/filesystem/importers/directories_importer.h>
#include <misaxx/misa_cli.h>
#include <misaxx/module_data/misa_generic_file_stack.h>
#include <misaxx/algorithm_node_path.h>
#include <misaxx/object_node_path.h>
#include <misaxx/parameters/voxel_size.h>
#include <misaxx/parameters/object_metadata.h>

using namespace misaxx;
using namespace pattxx;

struct other_module_def : public misa_module_declaration {

};

struct other_task1 : public misa_task<other_module_def> {
    using misa_task::misa_task;

    int abcde = from_object_json<int>("abcde");

    void work() {
        auto x = get_node().get_custom_path<algorithm_node_path>();
        auto y = get_node().get_custom_path<object_node_path>();
        std::cout << "test2" << std::endl;
    }
};

struct other_module : public misa_module<other_module_def> {
    using misa_module::misa_module;

    void init() {
        auto x = get_node().get_custom_path<algorithm_node_path>();
        auto y = get_node().get_custom_path<object_node_path>();
        misa_dispatch<other_task1>("other task 1");
    }
};


struct my_module_definition : public misa_module_declaration {
    data<misa_generic_file_stack> my_stack = declare_data<misa_generic_file_stack>("my_stack", metadata("my stack"));
    data<misa_generic_file_stack> processed = declare_data<misa_generic_file_stack>("processed");
    submodule <other_module> other = declare_submodule<other_module>("other", metadata("Other module"));
};

struct my_task1 : public misa_task<my_module_definition> {
    using misa_task::misa_task;

    void work() {
        std::cout << "test" << std::endl;
    }
};

struct my_module : public misa_module<my_module_definition> {

    using misa_module::misa_module;

    object_metadata md = from_parameter<object_metadata>();
    voxel_size vs = from_parameter<voxel_size>();

    void init() {
        // Data part
        init_submodule(other);
        init_data(my_stack).from_filesystem(filesystem.imported);
        init_data(processed).from_reference_stack(my_stack);

        auto x = get_node().get_custom_path<algorithm_node_path>();
        auto y = get_node().get_custom_path<object_node_path>();

        // Dispatcher part
        chain c;
        c >> misa_dispatch<my_task1>("abc") >> misa_dispatch(other);
    }
};

int main(int argc, const char** argv) {
    misa_cli<misa_multiobject_root<my_module>> cli("my_module");
    return cli.prepare_and_run(argc, argv);
}