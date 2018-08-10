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
#include <libs/pattxx/pattxx/default_runtime.h>
#include <misaxx/misa_runtime.h>

using namespace misaxx;
using namespace pattxx;

struct my_module_definition : public misa_module_definition {
    misa_file_stack my_stack = declare_data<misa_file_stack>();
};

struct my_task1 : public misa_task<my_module_definition> {
    using misa_task::misa_task;

    void work() {
    }
};

struct my_module : public misa_module<my_module_definition> {

    using misa_module::misa_module;

    void init() {
        // Dispatcher part
        misa_dispatch<my_task1>("abc");
    }
};

int main(int argc, const char** argv) {

    misa_runtime<my_module> rt("my_module");
    rt.run();

    return 0;
}