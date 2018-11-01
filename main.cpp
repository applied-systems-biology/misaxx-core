//
// Created by rgerst on 30.07.18.
//


#include <iostream>
#include <fstream>
#include <misaxx/misa_module.h>
#include <misaxx/misa_module_declaration.h>
#include <misaxx/misa_task.h>
#include <pattxx/default_runtime.h>
#include <misaxx/misa_runtime.h>
#include <misaxx/filesystem/misa_filesystem_directories_importer.h>
#include <misaxx/misa_cli.h>
#include <misaxx/algorithm_node_path.h>
#include <misaxx/object_node_path.h>
#include <misaxx/ome_data/object3d_voxel_size.h>
#include <misaxx/ome_data/object_name.h>
#include <misaxx/cached_data/misa_cache.h>
#include <misaxx/cached_data/misa_cached_data.h>
#include <misaxx/cached_data/misa_unsafe_image_stack.h>

using namespace misaxx;
using namespace pattxx;

struct other_module_def : public misa_module_declaration {
    void init_data() override {

    }
};

struct other_task1 : public misa_task<other_module_def> {
    using misa_task::misa_task;

    int abcde = from_object_json<int>("abcde");

    void misa_work() override {
        auto x = get_node().get_custom_path<algorithm_node_path>();
        auto y = get_node().get_custom_path<object_node_path>();
        std::cout << "test2" << std::endl;
    }
};

struct other_module : public misa_module<other_module_def> {
    using misa_module::misa_module;

    dispatched <other_task1> dispatch_other_task1 = future_dispatch<other_task1>("other task 1");

    void misa_init() override {
        auto x = get_node().get_custom_path<algorithm_node_path>();
        auto y = get_node().get_custom_path<object_node_path>();
        misa_dispatch(dispatch_other_task1);
    }
};


struct my_module_declaration : public misa_module_declaration {
//    misa_cached_data<misa_unsafe_image_stack> my_stack;
    misa_cached_data<misa_unsafe_file> test_file1;
    misa_cached_data<misa_unsafe_image_stack<coixx::images::grayscale_float >> example_stack;
    submodule <other_module> other;

    void init_data() override {
        test_file1.suggest_import_location(filesystem, "test_file1");
        example_stack.suggest_import_location(filesystem, "example_stack");
//        import_from_filesystem(my_stack, "/");
//        process(processed, my_stack, "processed");
//        init_submodule(other, "other");
    }
};

struct my_task1 : public misa_task<my_module_declaration> {
    using misa_task::misa_task;

    void misa_work() override {
        std::cout << module().test_file1.access_readonly().get().string() << std::endl;
        std::cout << "test###abc" << std::endl;
    }
};

struct my_task2 : public misa_task<my_module_declaration> {
    using misa_task::misa_task;

    void misa_work() override {
        std::cout << "test###def" << std::endl;
    }
};

struct my_module : public misa_module<my_module_declaration> {

    using misa_module::misa_module;

    object_name md = from_parameter<object_name>();
    object3d_voxel_size vs = from_parameter<object3d_voxel_size>();

    dispatched <my_task1> dispatch_my_task1 = future_dispatch<my_task1>("abc");
    dispatched<other_module> dispatcher_other = future_dispatch(other);

    dispatched < misa_task<my_module_declaration>> test = select< misa_task<my_module_declaration>>("def", { future_dispatch<my_task1>("abc"), future_dispatch<my_task2>("def") });
    dispatched < misa_task<my_module_declaration>> test2 = select_from_algorithm_json_or< misa_task<my_module_declaration>>("test2", "x.a", { future_dispatch<my_task1>("x.a"), future_dispatch<my_task2>("x.b") });

    void misa_init() override {
        auto x = get_node().get_custom_path<algorithm_node_path>();
        auto y = get_node().get_custom_path<object_node_path>();

        // Dispatcher part
        chain c;
        c >> misa_dispatch(dispatch_my_task1) >> misa_dispatch(dispatcher_other);
        c >> misa_dispatch(test);
    }
};

int main(int argc, const char** argv) {
    misa_cli<misa_multiobject_root<my_module>> cli("my_module");
    return cli.prepare_and_run(argc, argv);
}