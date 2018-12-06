//
// Created by rgerst on 31.07.18.
//


#pragma once

namespace misaxx::nodes {

    /**
     * Status of the node's subtree
     */
    enum misa_work_subtree_status : int {
        /**
         * Not all children are instantiated.
         * The runtime must keep track of this node and update it constantly until the node reports completion
         */
                incomplete,
        /**
         * All children are instantiated.
         * The runtime is not required anymore to check for changes in children
         */
                complete
    };

}