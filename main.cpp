//
// Created by rgerst on 30.07.18.
//


#include <iostream>
#include <fstream>
#include <misaxx/filesystem/importers/json_importer.h>
#include <misaxx/filesystem/importers/folder_importer.h>
#include <misaxx/misa_file_stack.h>
#include <misaxx/misa_module.h>
#include <misaxx/misa_module_definition.h>
#include <misaxx/misa_task.h>
#include <pattxx/default_runtime.h>
#include <misaxx/misa_runtime.h>

using namespace misaxx;
using namespace pattxx;

struct other_module_def : public misa_module_definition {

};

struct other_task1 : public misa_task<other_module_def> {
    using misa_task::misa_task;

    void work() {
        std::cout << "test2" << std::endl;
    }
};

struct other_module : public misa_module<other_module_def> {
    using misa_module::misa_module;

    void init() {
        misa_dispatch<other_task1>("other task 1");
    }
};


struct my_module_definition : public misa_module_definition {
    misa_file_stack my_stack = data<misa_file_stack>(metadata("my stack"));
    submodule <other_module> other = imported<other_module>("other", metadata("Other module"));
};

struct my_task1 : public misa_task<my_module_definition> {
    using misa_task::misa_task;

    void work() {
        std::cout << "test" << std::endl;
    }
};

struct my_module : public misa_module<my_module_definition> {

    using misa_module::misa_module;

    void init() {
        // Dispatcher part
        chain c;
        c >> misa_dispatch<my_task1>("abc") >> misa_dispatch(other);
    }
};

int main(int argc, const char** argv) {

    misa_runtime<my_module> rt("my_module");
    rt.run();

    return 0;
}