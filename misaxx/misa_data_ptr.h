//
// Created by rgerst on 17.08.18.
//


#pragma once

namespace misaxx {
    /**
     * Pointer to module data
     * @tparam Data
     */
    template<class Data> class misa_data_ptr {
    public:
        misa_data_ptr(Data &src) : m_data(&src) {

        }

        Data &operator *() {
            return *m_data;
        }

        Data *operator ->() {
            return m_data;
        }
    private:
        Data *m_data = nullptr;
    };
}
