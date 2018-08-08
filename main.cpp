//
// Created by rgerst on 30.07.18.
//


#include <iostream>
#include <fstream>
#include <misaxx/filesystem/importers/json_importer.h>
#include <misaxx/filesystem/importers/default_importer.h>

using namespace misaxx;

int main(int argc, const char** argv) {

    filesystem::importers::json_importer importer;
    importer.json_file = "/home/rgerst/tmp/misa_data_text.json";

    auto x = importer.import();

    filesystem::importers::default_importer importer2;
    importer2.input_path = "/home/rgerst/tmp/misa_folder_import";
    auto y = importer2.import();

    return 0;
}