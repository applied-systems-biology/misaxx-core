//
// Created by rgerst on 05.12.18.
//

#pragma once

#include "misa_range.h"

namespace misaxx {
    /**
    * A voxel defined by its extents in X, Y and Z directions
    * @tparam T
    */
    template<typename T> struct misa_voxel : public misa_serializeable {
        misa_range<T> range_x;
        misa_range<T> range_y;
        misa_range<T> range_z;

        misa_voxel() = default;

        explicit misa_voxel(misa_range<T> x, misa_range<T> y, misa_range<T> z) :
                range_x(std::move(x)), range_y(std::move(y)), range_z(std::move(z)) {

        }

        /**
         * Initializes the voxel with X = [0, x); Y = [0, y); Z = [0, z)
         * @param x
         * @param y
         * @param z
         */
        explicit misa_voxel(T x, T y, T z) :
        range_x(misa_range<T>(T(), x)), range_y(misa_range<T>(T(), y)), range_z(misa_range<T>(T(), z)) {

        }

        void from_json(const nlohmann::json &t_json) override {
            range_x.from_json(t_json["x"]);
            range_y.from_json(t_json["y"]);
            range_z.from_json(t_json["z"]);
        }

        void to_json(nlohmann::json &t_json) const override {
            misa_serializeable::to_json(t_json);
            range_x.to_json(t_json["x"]);
            range_y.to_json(t_json["y"]);
            range_z.to_json(t_json["z"]);
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            range_x.to_json_schema(t_schema.resolve("x"));
            range_y.to_json_schema(t_schema.resolve("y"));
            range_z.to_json_schema(t_schema.resolve("z"));
        }

        misa_serialization_id get_serialization_id() const override {
            return misa_serialization_id("misa_ome", "attachments/cube");
        }

        std::vector<misa_serialization_id> get_serialization_id_hierarchy() const override {
            return {get_serialization_id()};
        }

        /**
         * Calculates the volume of the voxel
         * @return
         */
        T volume() const {
            return range_x.size() * range_y.size() * range_z.size();
        }

        /**
         * Returns true if the voxel contains the coordinate
         * @param x
         * @param y
         * @param z
         * @return
         */
        bool contains(const T &x, const T &y, const T &z) const {
            return range_x.contains(x) && range_y.contains(y) && range_z.contains(z);
        }

        /**
         * Updates the voxel, so it includes the provided point
         * @param x
         * @param y
         * @param z
         */
        void include(const T &x, const T &y, const T &z) {
            range_x.include(x);
            range_y.include(y);
            range_z.include(z);
        }

        T size_x() const {
            return range_x.size();
        }

        T size_y() const {
            return range_y.size();
        }

        T size_z() const {
            return range_z.size();
        }

        /**
         * Returns true if size_x() and size_y() are equal
         * @return
         */
        bool is_pixel() const {
            return size_x() == size_y();
        }

        /**
         * Returns true if size_x(), size_y() and size_z() are equal
         * @return
         */
        bool is_cube() const {
            return is_pixel() && size_x() == size_z();
        }

        /**
         * Returns size_x() if size_x() and size_y() are equal
         * Otherwise throws an exception
         * @return
         */
        T pixel_size() const {
            if(!is_pixel())
                throw std::runtime_error("The voxel is no pixel!");
            return range_x.size();
        }

        /**
        * Returns size_x() if size_x(), size_y() and size_z() are equal
        * Otherwise throws an error
        * @return
        */
        T cube_size() const {
            if(!is_cube())
                throw std::runtime_error("The voxel is no cube!");
            return range_x.size();
        }
    };
}



