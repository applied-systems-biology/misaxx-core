//
// Created by rgerst on 14.01.19.
//

#pragma once

#include <string>
#include <unordered_map>
#include <typeindex>
#include <misaxx/misa_dispatch_blueprint.h>
#include <memory>

namespace misaxx {

    class misa_dispatcher;

    /**
     * Describes how the maximum subtree of a misa_dispatcher looks like
     */
    class misa_dispatcher_blueprint_list {
    public:

        using blueprint = std::shared_ptr<misa_dispatch_blueprint_base>;

        explicit misa_dispatcher_blueprint_list(misa_dispatcher &t_worker);

        /**
         * Adds an entry to this blueprint list
         * @param t_dispatched
         */
        void add(blueprint t_blueprint);

        /**
         * Adds multiple entries to the blueprint list
         * @param t_blueprints
         */
        void add(std::vector<blueprint> t_blueprints);

        /**
         * Returns a blueprint by its name
         * @param t_key
         * @return
         */
        blueprint get(const std::string &t_key) const;

        /**
         * Returns the stored blueprints
         * @return
         */
        const std::unordered_map<std::string, blueprint> &get_entries() const;

        /**
         * Returns the worker this blueprint list is attached to
         * @return
         */
        misa_dispatcher &get_worker() const;

    private:

        std::weak_ptr<misa_dispatcher> m_worker;

        /**
         * Allows checking if the the code initializes the correct types for child nodes
         */
        std::unordered_map<std::string, blueprint> m_blueprints;

    };
}




