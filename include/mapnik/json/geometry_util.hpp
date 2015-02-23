 /*****************************************************************************
 *
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2014 Artem Pavlenko
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/

#ifndef MAPNIK_JSON_GEOMETRY_UTIL_HPP
#define MAPNIK_JSON_GEOMETRY_UTIL_HPP

#include <mapnik/geometry_impl.hpp>

namespace mapnik { namespace json {

// geometries
template <typename Geometry>
struct create_point
{
    explicit create_point(Geometry & geom)
        : geom_(geom) {}

    void operator() (position const& pos) const
    {
        mapnik::new_geometry::point point(pos.x, pos.y);
        geom_ = std::move(point);
    }

    template <typename T>
    void operator()(T const&) const {} // no-op - shouldn't get here
    Geometry & geom_;
};

template <typename Geometry>
struct create_linestring
{
    explicit create_linestring(Geometry & geom)
        : geom_(geom) {}

    void operator() (positions const& ring) const
    {
        std::size_t size = ring.size();
        if (size > 1)
        {
            mapnik::new_geometry::line_string line;
            line.reserve(size);
            for (auto && pt : ring)
            {
                line.emplace_back(std::move(pt));
            }
            geom_ = std::move(line);
        }
    }

    template <typename T>
    void operator()(T const&) const {}  // no-op - shouldn't get here

    Geometry & geom_;
};

template <typename Geometry>
struct create_polygon
{
    explicit create_polygon(Geometry & geom)
        : geom_(geom) {}

    void operator() (std::vector<positions> const& rings) const
    {
        mapnik::new_geometry::polygon3 poly;
        std::size_t num_rings = rings.size();
        if (num_rings > 1)
            poly.interior_rings.reserve(num_rings - 1);

        for ( std::size_t i = 0; i < num_rings; ++i)
        {
            std::size_t size = rings[i].size();
            mapnik::new_geometry::linear_ring ring;
            ring.reserve(size);
            for ( auto && pt : rings[i])
            {
                ring.emplace_back(std::move(pt));
            }
            if (i == 0) poly.set_exterior_ring(std::move(ring));
            else poly.add_hole(std::move(ring));
        }

        geom_ = std::move(poly);
    }

    template <typename T>
    void operator()(T const&) const {}  // no-op - shouldn't get here

    Geometry & geom_;
};

// multi-geometries
template <typename Geometry>
struct create_multipoint
{
    explicit create_multipoint(Geometry & geom)
        : geom_(geom) {}

    void operator() (positions const& points) const
    {
        mapnik::new_geometry::multi_point multi_point;
        multi_point.reserve(points.size());
        for (auto && pos : points)
        {
            multi_point.emplace_back(std::move(pos));
        }
        geom_ = std::move(multi_point);
    }

    template <typename T>
    void operator()(T const&) const {}  // no-op - shouldn't get here

    Geometry & geom_;
};

template <typename Geometry>
struct create_multilinestring
{
    explicit create_multilinestring(Geometry & geom)
        : geom_(geom) {}

    void operator() (std::vector<positions> const& rings) const
    {
        mapnik::new_geometry::multi_line_string multi_line;
        multi_line.reserve(rings.size());

        for (auto const& ring : rings)
        {
            mapnik::new_geometry::line_string line;
            line.reserve(ring.size());
            for (auto && pt : ring)
            {
                line.emplace_back(std::move(pt));
            }
            multi_line.emplace_back(std::move(line));
        }
        geom_ = std::move(multi_line);
    }

    template <typename T>
    void operator()(T const&) const {}  // no-op - shouldn't get here

    Geometry & geom_;
};

template <typename Geometry>
struct create_multipolygon
{
    explicit create_multipolygon(Geometry & geom)
        : geom_(geom) {}

    void operator()(std::vector<std::vector<positions> > const& rings_array) const
    {
        mapnik::new_geometry::multi_polygon multi_poly;
        multi_poly.reserve(rings_array.size());
        for (auto const& rings : rings_array)
        {
            mapnik::new_geometry::polygon3 poly;
            std::size_t num_rings = rings.size();
            if ( num_rings > 1)
                poly.interior_rings.reserve(num_rings - 1);

            for ( std::size_t i = 0; i < num_rings; ++i)
            {
                std::size_t size = rings[i].size();
                mapnik::new_geometry::linear_ring ring;
                ring.reserve(size);
                for ( auto && pt : rings[i])
                {
                    ring.emplace_back(std::move(pt));
                }
                if (i == 0) poly.set_exterior_ring(std::move(ring));
                else poly.add_hole(std::move(ring));
            }
        }
        geom_ = std::move(multi_poly);
    }

    template <typename T>
    void operator()(T const&) const {}  // no-op - shouldn't get here

    Geometry & geom_;
};

struct create_geometry_impl
{
    using result_type = void;
    template <typename Geometry>
    void operator() (Geometry & geom, int type, mapnik::json::coordinates const& coords) const
    {
        switch (type)
        {
        case 1 ://Point
            util::apply_visitor(create_point<Geometry>(geom), coords);
            break;
        case 2 ://LineString
           util::apply_visitor(create_linestring<Geometry>(geom), coords);
           break;
        case 3 ://Polygon
            util::apply_visitor(create_polygon<Geometry>(geom), coords);
            break;
        case 4 ://MultiPoint
            util::apply_visitor(create_multipoint<Geometry>(geom), coords);
            break;
        case 5 ://MultiLineString
            util::apply_visitor(create_multilinestring<Geometry>(geom), coords);
            break;
        case 6 ://MultiPolygon
            util::apply_visitor(create_multipolygon<Geometry>(geom), coords);
            break;
        default:
            break;
        }

    }
};

}}

#endif // MAPNIK_JSON_GEOMETRY_UTIL_HPP