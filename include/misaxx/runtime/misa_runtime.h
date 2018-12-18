//
// Created by rgerst on 15.08.18.
//


#pragma once

#include <misaxx/filesystem/misa_filesystem.h>
#include <misaxx/runtime/misa_runtime_base.h>
#include <misaxx/json/misa_json_schema_builder.h>
#include <misaxx-helpers/measurement/manual_stopwatch.h>
#include <misaxx/workers/task_tree/misa_work_node.h>
#include <misaxx/misa_root_module_base.h>
#include <misaxx/json/misa_json_helper.h>

namespace misaxx {

    template<class RootModule>
    class misa_runtime : public misa_runtime_base {

        static_assert(std::is_base_of<misa_root_module_base, RootModule>::value, "Only root modules can be instantiated!");

    protected:

        /**
        * Instantiates the default runtime and provides the root instantiator with necessary parameters
        * @tparam Args
        * @param args
        */
        explicit misa_runtime(std::string t_name) : m_root_dispatcher_name(std::move(t_name)) {

        }

    public:

        /**
         * Module declaration type of the root node
         */
        using module_declaration_type = typename RootModule::module_declaration_type;

        /**
         * Returns the instance of the root dispatcher
         * @return
         */
        RootModule &module_instance() {
            return dynamic_cast<RootModule&>(*get_root_node()->get_instance());
        }

        /**
        * Returns the filesystem of the current module instance
        * @return
        */
        misa_filesystem get_filesystem() override {
            return module_instance().filesystem;
        }

        /**
         * Sets the filesystem
         * @param t_filesystem
         */
        void set_filesystem(misa_filesystem t_filesystem) override {
            if(is_running())
                throw std::runtime_error("Cannot change the filesystem while the runtime is running!");
            m_root_declaration.filesystem = std::move(t_filesystem);
        }

    protected:

        std::shared_ptr<nodes::misa_work_node> create_root_node() override {
            return std::make_shared<nodes::misa_work_node>(m_root_dispatcher_name,
                        std::shared_ptr<nodes::misa_work_node>(), [this](const std::shared_ptr<nodes::misa_work_node> &t_root) {
                        auto root_module = std::make_shared<RootModule>(t_root, std::move(m_root_declaration));
                        return root_module;
                    });
        }

    private:

        /**
        * Root module declaration
         * This will be later moved into the actual root module
        */
        module_declaration_type m_root_declaration;


        /**
         * Name of the root module
         */
        std::string m_root_dispatcher_name;

    public:

        /**
         * Creates an instance of the runtime and assigns the singleton
         * @param name
         * @return
         */
        static std::shared_ptr<misa_runtime<RootModule>> create_instance(std::string name) {
            if(singleton_instance.expired()) {
                auto x = misa_runtime<RootModule> { std::move(name) };
                auto ptr = std::make_shared<misa_runtime<RootModule>>(std::move(x));
                misa_runtime_base::singleton_instance = ptr;
                return ptr;
            }
            else {
                throw std::runtime_error("Only one runtime can be active!");
            }
        }

    };
}