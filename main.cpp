//
// Created by rgerst on 30.07.18.
//


#include <iostream>
#include <fstream>
#include <misaxx/core/misa_module.h>
#include <misaxx/core/misa_module_interface.h>
#include <misaxx/core/misa_task.h>
#include <misaxx/core/runtime/misa_runtime.h>
#include <misaxx/core/filesystem/misa_filesystem_directories_importer.h>
#include <misaxx/core/runtime/misa_cli.h>
#include <misaxx/core/workers/paths/algorithm_node_path.h>
#include <misaxx/core/workers/paths/object_node_path.h>
#include <misaxx/core/misa_cache.h>
#include <misaxx/core/misa_cached_data.h>
#include <misaxx/core/caches/misa_image_stack_cache.h>
#include <misaxx/core/attachments/misa_matrix.h>
#include <misaxx/core/attachments/misa_quantity.h>
#include <misaxx/core/attachments/misa_unit_numeric.h>

using namespace misaxx;

struct other_module_def : public misa_module_declaration {
    void init_data() override {

    }
};

struct other_task1 : public misa_task<other_module_def> {
    using misa_task::misa_task;

//    int abcde = from_object_json<int>("abcde");

    void misa_work() override {
        auto x = get_node()->get_custom_path<algorithm_node_path>();
        auto y = get_node()->get_custom_path<object_node_path>();
        std::cout << "test2" << "\n";
    }
};

struct other_module : public misa_module<other_module_def> {
    using misa_module::misa_module;

    dispatched <other_task1> dispatch_other_task1 = future_dispatch<other_task1>("other task 1");

    void misa_init() override {
//        auto x = get_node()->get_custom_path<algorithm_node_path>();
//        auto y = get_node()->get_custom_path<object_node_path>();
        misa_dispatch(dispatch_other_task1);
    }
};


struct my_module_declaration : public misa_module_declaration {
//    misa_cached_data<misa_image_stack_cache> my_stack;
//    misa_cached_data<misa_unsafe_file> test_file1;
//    misa_cached_data<misa_image_stack_cache<coixx::images::grayscale_float >> example_stack;
    submodule <other_module> other;

    void init_data() override {
//        test_file1.suggest_import_location(filesystem, "test_file1");
//        example_stack.suggest_import_location(filesystem, "example_stack");
//        import_from_filesystem(my_stack, "/");
//        process(processed, my_stack, "processed");
        init_submodule(other, "other");
    }
};

struct my_task1 : public misa_task<my_module_declaration> {
    using misa_task<my_module_declaration>::misa_task;

    void misa_work() override {
//        std::cout << module()->test_file1.access_readonly().get().string() << "\n";
        std::cout << "test###abc" << "\n";
    }
};

struct my_task2 : public misa_task<my_module_declaration> {
    using misa_task<my_module_declaration>::misa_task;

    void misa_work() override {
        std::cout << "test###def" << "\n";
    }
};

struct my_module : public misa_module<my_module_declaration> {

    using misa_module<my_module_declaration>::misa_module;

//    object_name md = from_parameter<object_name>();
//    object3d_voxel_size vs = from_parameter<object3d_voxel_size>();
//
    dispatched <my_task1> dispatch_my_task1 = future_dispatch<my_task1>("abc");
    dispatched<other_module> dispatcher_other = future_dispatch(other);
//
//    dispatched < misa_task<my_module_declaration>> test = select< misa_task<my_module_declaration>>("def", { future_dispatch<my_task1>("abc"), future_dispatch<my_task2>("def") });
//    dispatched < misa_task<my_module_declaration>> test2 = select_from_algorithm_json_or< misa_task<my_module_declaration>>("test2", "x.a", { future_dispatch<my_task1>("x.a"), future_dispatch<my_task2>("x.b") });

    void misa_init() override {
//        auto x = get_node()->get_custom_path<algorithm_node_path>();
//        auto y = get_node()->get_custom_path<object_node_path>();
        // Dispatcher part
        chain c;
        c >> misa_dispatch(dispatch_my_task1) >> misa_dispatch(dispatcher_other);
//        c >> misa_dispatch(test);
    }
};

int main(int argc, const char** argv) {

    misa_quantity<int, misa_unit_numeric> x(5);
    misa_quantity<int, misa_unit_numeric> y(5);

    auto area = x * y;

    misa_matrix<int, misa_unit_numeric, 2, 3> m1 {{ 3, 2, 1,
                                                   1, 0, 2}};
    misa_matrix<int, misa_unit_numeric, 3, 2> m2 {{1, 2,
                                                   0, 1,
                                                   4, 0}};
    auto w = m1 % m2;
    auto p = w.get_element_product();

    std::cout << nlohmann::json(w) << "\n";

    misa_cli<misa_multiobject_root<my_module>> cli("my_module");
    return cli.prepare_and_run(argc, argv);
}