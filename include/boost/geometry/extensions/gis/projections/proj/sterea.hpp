#ifndef BOOST_GEOMETRY_PROJECTIONS_STEREA_HPP
#define BOOST_GEOMETRY_PROJECTIONS_STEREA_HPP

// Boost.Geometry - extensions-gis-projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright (c) 2008-2015 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels

// Last updated version of proj: 4.9.1

// Original copyright notice:

// Copyright (c) 2003   Gerald I. Evenden

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <boost/math/special_functions/hypot.hpp>

#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>
#include <boost/geometry/extensions/gis/projections/impl/pj_gauss.hpp>

#include <boost/geometry/extensions/gis/projections/epsg_traits.hpp>

namespace boost { namespace geometry { namespace projections
{
    struct sterea {};

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {
        namespace sterea
        {

            static const double DEL_TOL = 1.e-14;
            static const int MAX_ITER = 10;

            struct par_sterea
            {
                double phic0;
                double cosc0, sinc0;
                double R2;
                gauss::GAUSS en;
            };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_sterea_ellipsoid : public base_t_fi<base_sterea_ellipsoid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_sterea m_proj_parm;

                inline base_sterea_ellipsoid(const Parameters& par)
                    : base_t_fi<base_sterea_ellipsoid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                // FORWARD(e_forward)  ellipsoid
                // Project coordinates from geographic (lon, lat) to cartesian (x, y)
                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double cosc, sinc, cosl_, k;

                    detail::gauss::gauss(m_proj_parm.en, lp_lon, lp_lat);
                    sinc = sin(lp_lat);
                    cosc = cos(lp_lat);
                    cosl_ = cos(lp_lon);
                    k = this->m_par.k0 * this->m_proj_parm.R2 / (1. + this->m_proj_parm.sinc0 * sinc + this->m_proj_parm.cosc0 * cosc * cosl_);
                    xy_x = k * cosc * sin(lp_lon);
                    xy_y = k * (this->m_proj_parm.cosc0 * sinc - this->m_proj_parm.sinc0 * cosc * cosl_);
                }

                // INVERSE(e_inverse)  ellipsoid
                // Project coordinates from cartesian (x, y) to geographic (lon, lat)
                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    double rho, c, sinc, cosc;

                    xy_x /= this->m_par.k0;
                    xy_y /= this->m_par.k0;
                    if((rho = boost::math::hypot(xy_x, xy_y))) {
                        c = 2. * atan2(rho, this->m_proj_parm.R2);
                        sinc = sin(c);
                        cosc = cos(c);
                        lp_lat = asin(cosc * this->m_proj_parm.sinc0 + xy_y * sinc * this->m_proj_parm.cosc0 / rho);
                        lp_lon = atan2(xy_x * sinc, rho * this->m_proj_parm.cosc0 * cosc -
                            xy_y * this->m_proj_parm.sinc0 * sinc);
                    } else {
                        lp_lat = this->m_proj_parm.phic0;
                        lp_lon = 0.;
                    }
                    detail::gauss::inv_gauss(m_proj_parm.en, lp_lon, lp_lat);
                }

                static inline std::string get_name()
                {
                    return "sterea_ellipsoid";
                }

            };

            // Oblique Stereographic Alternative
            template <typename Parameters>
            void setup_sterea(Parameters& par, par_sterea& proj_parm)
            {
                double R;

                proj_parm.en = detail::gauss::gauss_ini(par.e, par.phi0, proj_parm.phic0, R);
                proj_parm.sinc0 = sin(proj_parm.phic0);
                proj_parm.cosc0 = cos(proj_parm.phic0);
                proj_parm.R2 = 2. * R;
            }

        } // namespace sterea

        /*!
            \brief Oblique Stereographic Alternative projection
            \ingroup projections
            \tparam Geographic latlong point type
            \tparam Cartesian xy point type
            \tparam Parameters parameter type
            \par Projection characteristics
             - Azimuthal
             - Spheroid
             - Ellipsoid
            \par Example
            \image html ex_sterea.gif
        */
        template <typename Geographic, typename Cartesian, typename Parameters = parameters>
        struct sterea_ellipsoid : public detail::sterea::base_sterea_ellipsoid<Geographic, Cartesian, Parameters>
        {
            inline sterea_ellipsoid(const Parameters& par) : detail::sterea::base_sterea_ellipsoid<Geographic, Cartesian, Parameters>(par)
            {
                detail::sterea::setup_sterea(this->m_par, this->m_proj_parm);
            }
        };

        // Static projection
        BOOST_GEOMETRY_PROJECTIONS_DETAIL_STATIC_PROJECTION(projections::sterea, sterea_ellipsoid, sterea_ellipsoid)

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class sterea_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual base_v<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<sterea_ellipsoid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void sterea_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("sterea", new sterea_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail
    #endif // doxygen

}}} // namespace boost::geometry::projections

#endif // BOOST_GEOMETRY_PROJECTIONS_STEREA_HPP

