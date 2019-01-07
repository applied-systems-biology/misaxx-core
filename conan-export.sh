#!/bin/bash

conan export-pkg . misaxx-core/0.1.0@hki/stable -s build_type=Release --build-folder=conan-build-release --force

