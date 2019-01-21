//
// Created by rgerst on 31.07.18.
//


#pragma once

#include <misaxx/core/workers/misa_work_node.h>
#include <unordered_set>

namespace misaxx {

    /**
     * Dependencies that are passed around
     */
    using depencencies_t = std::unordered_set<std::shared_ptr<misa_work_node>>;

    /**
     * Represents an item in a misaxx::depencencies::chain
     */
    struct misa_work_dependency_segment {

        /**
        * Dependencies of this specific segment
        * @return
        */
        virtual depencencies_t dependencies() const = 0;

       /**
        * If another segment should depend from this one, the dependencies in the return value must be statisfied.
        * This is used for interaction between segments
        * @return
        */
        virtual depencencies_t to_dependencies() = 0;
    };

}