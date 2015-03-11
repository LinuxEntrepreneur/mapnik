/*****************************************************************************
 *
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2015 Artem Pavlenko
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

#ifndef MAPNIK_GEOMETRY_CORRECT_HPP
#define MAPNIK_GEOMETRY_CORRECT_HPP

#include <mapnik/geometry_impl.hpp>
#include <mapnik/geometry_adapters.hpp>
#include <boost/geometry/algorithms/correct.hpp>

namespace mapnik { namespace new_geometry {

namespace detail {

struct geometry_correct
{
    using result_type = void;

    result_type operator() (geometry_collection & collection) const
    {
        for (auto & geom : collection)
        {
            (*this)(geom);
        }
    }

    result_type operator() (polygon & poly) const
    {
        return boost::geometry::correct(poly);
    }

    result_type operator() (multi_polygon & multi_poly) const
    {
        return boost::geometry::correct(multi_poly);
    }

    template <typename T>
    result_type operator() (T & geom) const
    {
        //no-op
    }
};

}

inline void correct(mapnik::new_geometry::geometry & geom)
{
    return mapnik::util::apply_visitor(detail::geometry_correct(), geom);
}

}}

#endif // MAPNIK_GEOMETRY_CORRECT_HPP