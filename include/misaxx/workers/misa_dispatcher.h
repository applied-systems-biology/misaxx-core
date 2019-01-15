//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <misaxx/workers/misa_dispatcher.h>
#include <misaxx/workers/dependency_management/misa_work_dependency_chain.h>
#include <misaxx/workers/dependency_management/misa_work_dependency_group.h>
#include <misaxx/workers/misa_worker.h>
#include <misaxx/workers/paths/algorithm_node_path.h>
#include <misaxx/workers/paths/object_node_path.h>
#include <misaxx/misa_dispatch_blueprint.h>
#include <misaxx/workers/misa_functional_task.h>
#include <misaxx/workers/task_tree/misa_work_node.h>
#include <misaxx/misa_dispatcher_builder.h>

namespace misaxx {

    /**
     * MISA++ dispatcher base class.
     * @tparam ModuleDeclaration
     */
    struct misa_dispatcher : public misa_worker {

        using chain = misaxx::dependencies::misa_work_dependency_chain;
        using group = misaxx::dependencies::misa_work_dependency_group;
        using blueprints = misa_dispatcher_blueprints;
        using builder = misa_dispatcher_builder;
        using blueprint = std::shared_ptr<misa_dispatch_blueprint_base>;

        using misa_worker::misa_worker;

    protected:

        /**
         * Creates a blueprint for dispatching a task or sub-dispatcher
         * @tparam Instance
         * @param t_name
         * @return
         */
        template<class Instance>
        blueprint create_blueprint(const std::string &t_name) {
            static_assert(!std::is_base_of<misa_module_interface, Instance>::value, "This function does not support submodules! Use create_submodule_blueprint() instead.");
            auto result = std::make_shared<misa_dispatch_blueprint<Instance>>();
            result->name = t_name;
            result->function = [t_name](misa_dispatcher &t_worker) -> Instance & {
                return t_worker.template dispatch_instance<Instance>(t_name, t_worker.get_module());
            };
            return result;
        }

        /**
         * Creates a blueprint that instantiates a submodule
         * @param t_submodule
         * @return
         */
        template<class Instance, class Interface = typename Instance::module_interface_type>
        blueprint create_submodule_blueprint(const std::string &t_name, Interface t_interface = Interface()) {
            static_assert(std::is_base_of<misa_module_interface, Instance>::value, "This function only supports submodules! Use create_blueprint() instead.");

            // Modules need to be initialized immediately
            Instance &instance = dispatch_instance<Instance>(t_name, std::move(t_interface));
            // Create a sub-filesystem within the current module
            instance.filesystem = get_module()->filesystem.create_subsystem(t_name);
            instance.setup();

            // The blueprint just works on a pointer
            Instance *instance_ptr = &instance;

            auto result = std::make_shared<misa_dispatch_blueprint<Instance>>();
            result->name = t_name;
            result->allow_multi_instancing = false;
            result->function = [instance_ptr](misa_dispatcher &t_worker) -> Instance& {
                return *instance_ptr;
            };

            return result;
        }

        /**
         * Exports a list of blueprints as algorithm JSON parameter with an optional value
         * @param t_blueprints
         * @param t_default
         * @return
         */
        std::vector<blueprint> export_to_algorithm_json(const std::string &t_param_name, std::vector<blueprint> t_blueprints, const std::optional<std::string> &t_default = std::nullopt);

    public:

        /**
         * This method should be overriden to create the blueprint of the tree created by this dispatcher
         * @param t_blueprint
         */
        virtual void create_blueprints(blueprints &t_blueprints) = 0;

        /**
         * This method creates the tree based on the blueprint
         * @param t_builder
         */
        virtual void build(const builder &t_builder) = 0;

        /**
         * Builds a simulated tree based on the blueprint
         */
        virtual void build_simulation(const builder &t_builder);

        void execute_work() override;

        bool is_parallelizeable() const override;

    private:

        /**
         * Builder that contains a blueprint of the tree that is created by this dispatcher
         */
        std::unique_ptr<misa_dispatcher_builder> m_builder;

//        /**
//         * Lists all future dispatched entries for later use in misa_simulate().
//         * We can do without a pointer because misa_dispatch_blueprint can convert internally
//         */
//        std::vector<dispatched<misa_worker_base>> m_future_dispatched;

//        /**
//        * Creates a pattxx::functional_task that runs a function
//        * @param t_function
//        * @param t_parallelized If true, the tasks might be called in a parallel context. Defaults to false.
//        * @param t_name Name of the task
//        * @return
//        */
//        misa_functional_task<ModuleDeclaration> &run_function(typename misa_functional_task<ModuleDeclaration>::function_type t_function, const std::string &t_name = "functional_task") {
//            // We can call everything by reference as we will instantiate directly afterwards anyways
//            auto nd = this->get_node()->make_child(t_name, [&](const std::shared_ptr<nodes::misa_work_node> &t_node) {
//                auto task = std::make_shared<misa_functional_task>(t_node, this->module());
//                task->m_function = std::move(t_function);
//                return task;
//            });
//
//            return dynamic_cast<misa_functional_task<ModuleDeclaration>&>(*nd->get_or_create_instance());
//        }

        /**
         * Basic dispatch function that instantiates a task or dispatcher.
         * Although already instantiated, the runtime will ensure that dependencies are satisfied before work() is called.
         * @tparam Instance
         * @param t_name
         * @return
         */
        template<class Instance, typename... Args>
        Instance &dispatch_instance(const std::string &t_name, Args &&... args) {
            // We can call everything by reference as we will instantiate directly afterwards anyways
            auto nd = this->get_node()->make_child(t_name, [&](const std::shared_ptr<nodes::misa_work_node> &t_node) {
                auto task = std::make_shared<Instance>(t_node, std::forward<Args>(args)...);
                return task;
            });

            return dynamic_cast<Instance&>(*nd->get_or_create_instance());
        }
    };
}
