// Copyright (c) 2015  GeometryFactory (France).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// 
//
// Author(s)     : Andreas Fabri


#ifndef CGAL_PROPERTIES_SEAM_MESH_H
#define CGAL_PROPERTIES_SEAM_MESH_H

#include <CGAL/boost/graph/Seam_mesh.h>

#include <CGAL/boost/graph/properties.h>

namespace CGAL {

  template <class TM,class SEM, class SVM>
class Seam_mesh;


template<class TM,class SEM, class SVM>
class Seam_mesh_point_map
  : public boost::put_get_helper<typename boost::property_traits<typename boost::property_map<TM,vertex_point_t>::const_type>::value_type, Seam_mesh_point_map<TM,SEM,SVM> >
{
public:
  typedef boost::readable_property_map_tag category;
  typedef typename boost::property_traits<typename boost::property_map<TM,vertex_point_t>::const_type>::value_type value_type;
  typedef value_type                    reference;
  typedef typename boost::graph_traits<Seam_mesh<TM,SEM,SVM> >::vertex_descriptor key_type;

private:
  typedef typename boost::property_map<TM,vertex_point_t>::const_type Map;

public:

  Seam_mesh_point_map(const Seam_mesh<TM,SEM,SVM>& mesh, Map map)
    : mesh(mesh), map(map) 
  {}

  Seam_mesh_point_map(const Seam_mesh_point_map<TM,SEM,SVM>& other)
    : mesh(other.mesh), map(other.map) 
  {}

  reference operator[](const key_type& vd) const
  {
    typename boost::graph_traits<TM>::halfedge_descriptor hd = vd;
    return map[target(hd,mesh.mesh())]; 
  }

private:
  const Seam_mesh<TM,SEM,SVM>& mesh;
  Map map;
};


  template<class TM, class SEM, class SVM, class Map>
class Seam_mesh_uv_map {
public:
  typedef boost::read_write_property_map_tag category;
  typedef typename boost::property_traits<Map>::value_type value_type;
  typedef typename boost::property_traits<Map>::reference reference;
  typedef typename boost::graph_traits<Seam_mesh<TM,SEM,SVM> >::vertex_descriptor key_type;
  // assert that key_type equals boost::property_traits<Map>::key_type

    typedef Seam_mesh<TM,SEM,SVM> Mesh;
public:

  Seam_mesh_uv_map(const Seam_mesh<TM,SEM,SVM>& mesh, Map map)
    : mesh(mesh), map(map) 
  {}

    Seam_mesh_uv_map(const Seam_mesh_uv_map<TM,SEM,SVM,Map>& other)
    : mesh(other.mesh), map(other.map) 
  {}

    //reference operator[](const key_type& vd) const
    //{
    //  typename boost::graph_traits<TM,SEM,SVM>::halfedge_descriptor hd = vd;
    //  return map[target(hd,mesh.mesh())]; 
    //}

  
    inline friend reference get(const Seam_mesh_uv_map<TM,SEM,SVM,Map>& pm, key_type k)
  {
    return get(pm.map,k);
  }

  inline friend void put(const Seam_mesh_uv_map<TM,SEM,SVM,Map>& pm, key_type k, const value_type& v)
  {
    typedef typename boost::graph_traits<typename Seam_mesh_uv_map<TM,SEM,SVM,Map>::Mesh>::halfedge_descriptor halfedge_descriptor;
    BOOST_FOREACH(halfedge_descriptor hd, halfedges_around_target(halfedge(k,pm.mesh),pm.mesh)){
      if(! hd.seam){
        put(pm.map,target(hd,pm.mesh),v);
      } else {
      }
    }
  }


private:
  const Seam_mesh<TM,SEM,SVM>& mesh;
  Map map;
  };


} // namespace CGAL


namespace boost {

template<class TM, class SEM, class SVM>
struct property_map<CGAL::Seam_mesh<TM,SEM,SVM>, CGAL::vertex_point_t >
{
  typedef CGAL::Seam_mesh<TM,SEM,SVM> SM; 

  typedef CGAL::Seam_mesh_point_map<TM,SEM,SVM> type;
  
  typedef type const_type;
  
};

template<class TM, class SEM, class SVM>
typename property_map<CGAL::Seam_mesh<TM,SEM,SVM>, CGAL::vertex_point_t >::const_type
get(CGAL::vertex_point_t, const CGAL::Seam_mesh<TM,SEM,SVM>& sm) {
  return CGAL::Seam_mesh_point_map<TM,SEM,SVM>(sm, get(CGAL::vertex_point, const_cast<TM&>(sm.mesh())));
}


} // namespace boost 

#endif // CGAL_PROPERTIES_SEAM_MESH_H
