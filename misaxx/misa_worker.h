//
// Created by rgerst on 10.08.18.
//


#pragma once

namespace misaxx {
    /**
     * Base class for all MISA++ workers
     * @tparam ModuleDeclaration
     */
    template<class ModuleDeclaration> struct misa_worker {
        using module_type = ModuleDeclaration;
        virtual ModuleDeclaration &module() = 0;

        template<class Metadata, class DataPtr> Metadata &access_metadata(DataPtr &ptr) {
            return ptr->user_metadata.template access<Metadata>();
        }
    };
}
