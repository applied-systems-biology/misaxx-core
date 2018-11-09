//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <misaxx/workers/misa_dispatcher.h>
#include <misaxx/workers/dependency_management/misa_work_dependency_chain.h>
#include <misaxx/workers/dependency_management/misa_work_dependency_group.h>
#include "misaxx/misa_module_declaration_base.h"
#include "misa_worker.h"
#include "misaxx/misa_module_base.h"
#include "misaxx/workers/paths/algorithm_node_path.h"
#include "misaxx/workers/paths/object_node_path.h"
#include "misaxx/misa_future_dispatch.h"
#include <misaxx/workers/misa_functional_task.h>
#include <misaxx/workers/task_tree/misa_work_node.h>

namespace misaxx {

    /**
     * MISA++ dispatcher base class.
     * @tparam ModuleDeclaration
     */
    template<class ModuleDeclaration>
    struct misa_dispatcher : public misa_worker<ModuleDeclaration> {
        static_assert(std::is_base_of<misa_module_declaration_base, ModuleDeclaration>::value, "Template argument must be a module definition!");

        using chain = misaxx::dependencies::misa_work_dependency_chain;
        using group = misaxx::dependencies::misa_work_dependency_group;

        /**
         * A future dispatch instance
         */
        template<class Instance> using dispatched = misa_future_dispatch<misa_dispatcher<ModuleDeclaration>, Instance>;

        using misa_worker<ModuleDeclaration>::misa_worker;

    protected:

        template<class FutureDispatch, class Instance = typename FutureDispatch::result_type>
        Instance &misa_dispatch(FutureDispatch &t_dispatch) {
            return t_dispatch.dispatch_specific(*this);
        }

        template<class Instance>
        dispatched<Instance> future_dispatch(const std::string &t_name) {
            dispatched<Instance> result;
            result.name = t_name;
            result.function = [t_name](misa_dispatcher<ModuleDeclaration> &t_worker) -> Instance & {
                return t_worker.template dispatch<Instance>(t_name, t_worker.module());
            };
            if(misa_runtime_base::instance().is_simulating())
                m_future_dispatched.push_back(result);
            return result;
        }

        template<class Submodule, class Module = typename Submodule::module_type>
        dispatched<Module> future_dispatch(Submodule &t_submodule) {
            dispatched<Module> result;
            result.name = t_submodule.name;
            result.function = [&t_submodule](misa_dispatcher<ModuleDeclaration> &t_worker) -> Module & {
                if (t_submodule.has_instance())
                    throw std::runtime_error("The submodule already has been instantiated!");

                // Dispatch the module and tell the submodule holder
                auto &instance = t_worker.template dispatch<Module>(t_submodule.name, std::move(t_submodule.definition()));
                t_submodule.m_module = &instance;
                return instance;
            };
            if(misa_runtime_base::instance().is_simulating())
                m_future_dispatched.push_back(result);
            return result;
        }

        template<class InstanceBase>
        dispatched<InstanceBase> select(const std::string &t_name, const std::vector<dispatched<InstanceBase>> &t_values) {
            for (const auto &v : t_values) {
                if (v.name == t_name) {
                    return v;
                }
            }
            throw std::runtime_error("Unknown instance with name " + t_name);
        }

        template<class InstanceBase>
        dispatched<InstanceBase>
        select_from_algorithm_json(const std::string &t_param_name, const std::vector<dispatched<InstanceBase>> &t_values) {
            misa_json_property <std::string> m;
            for (const auto &v : t_values) {
                m.allowed_values.push_back(v.name);
            }
            std::string n = this->template from_algorithm_json<std::string>(t_param_name, std::move(m));
            return select<InstanceBase>(n, t_values);
        }

        template<class InstanceBase>
        dispatched<InstanceBase>
        select_from_algorithm_json_or(const std::string &t_param_name, const std::string &t_default, const std::vector<dispatched<InstanceBase>> &t_values) {
            misa_json_property <std::string> m;
            for (const auto &v : t_values) {
                m.allowed_values.push_back(v.name);
            }
            std::string n = this->template from_algorithm_json_or<std::string>(t_param_name, t_default, std::move(m));
            return select<InstanceBase>(n, t_values);
        }

    public:

        virtual void misa_init() = 0;

        virtual void misa_simulate() {
            for(const auto &f : m_future_dispatched) {
                f.dispatch(*this);
            }
        }

        void work() override {
            if (misa_runtime_base::instance().is_simulating())
                this->misa_simulate();
            else
                this->misa_init();
        }

        bool is_parallelizeable() const override {
            return false;
        }

    private:

        /**
         * Lists all future dispatched entries for later use in misa_simulate().
         * We can do without a pointer because misa_future_dispatch can convert internally
         */
        std::vector<dispatched<misa_worker_base>> m_future_dispatched;

        /**
        * Creates a pattxx::functional_task that runs a function
        * @param t_function
        * @param t_parallelized If true, the tasks might be called in a parallel context. Defaults to false.
        * @param t_name Name of the task
        * @return
        */
        misa_functional_task<ModuleDeclaration> &run_function(typename misa_functional_task<ModuleDeclaration>::function_type t_function, bool t_parallelized = false, const std::string &t_name = "functional_task") {
            // We can call everything by reference as we will instantiate directly afterwards anyways
            auto nd = this->get_node()->make_child(t_name, [&](const std::shared_ptr<nodes::misa_work_node> &t_node) {
                auto task = std::make_shared<misa_functional_task>(t_node, this->module());
                task->m_function = std::move(t_function);
                task->is_parallelizeable = t_parallelized;
                return task;
            });

            return dynamic_cast<misa_functional_task<ModuleDeclaration>&>(*nd->get_or_create_instance());
        }

        /**
         * Basic dispatch function that instantiates a task or dispatcher.
         * Although already instantiated, the runtime will ensure that dependencies are satisfied before work() is called.
         * @tparam Instance
         * @param t_name
         * @return
         */
        template<class Instance, typename... Args>
        Instance &dispatch(const std::string &t_name, Args &&... args) {
            // We can call everything by reference as we will instantiate directly afterwards anyways
            auto nd = this->get_node()->make_child(t_name, [&](const std::shared_ptr<nodes::misa_work_node> &t_node) {
                auto task = std::make_shared<Instance>(t_node, std::forward<Args>(args)...);
                return task;
            });

            return dynamic_cast<Instance&>(*nd->get_or_create_instance());
        }
    };
}
