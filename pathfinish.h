/*  Copyright 2010 Anders Wallin (anders.e.e.wallin "at" gmail.com)
 *  
 *  This file is part of OpenCAMlib.
 *
 *  OpenCAMlib is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  OpenCAMlib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with OpenCAMlib.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PATH_FINISH_H
#define PATH_FINISH_H
#include <boost/foreach.hpp>
#include <boost/python.hpp>
#include <iostream>
#include <string>
#include <list>
#include "path.h"
#include "triangle.h"
#include "stlsurf.h"
#include "cutter.h"
#include "kdtree.h"

///
/// \brief path drop cutter finish Path generation
      
class PathDropCutterFinish {
    public:
        /// the path to follow
		const Path *path;
        /// the cutter used for this operation
		const MillingCutter *cutter;
        /// the surface for this operation
        const STLSurf *surf;
        /// root of a kd-tree that holds the triangles of the surface
        KDNode *root;
		/// the lowest z height, used when no triangles are touched
		double minimumZ;

        /// list of CL-points
        std::list<Point> clpoints;

		PathDropCutterFinish();
        /// create a PathDropCutterFinish operation with surface surf.
		PathDropCutterFinish(const STLSurf *surf);
        /// run drop-cutter on the whole Path
		void run();
        /// run drop-cutter on Span
		void run(const Span* span);
		void refinePointList(std::list<Point> &point_list);
        /// set the cutter
		void setCutter(const MillingCutter *cutter);
        /// set the path
		void setPath(const Path *path);
        /// return CL-points to Python
        boost::python::list getCLPoints();
};

#endif
