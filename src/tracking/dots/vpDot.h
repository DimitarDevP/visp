/****************************************************************************
 *
 * $Id: vpDot.h,v 1.22 2007-05-31 12:15:04 asaunier Exp $
 *
 * Copyright (C) 1998-2006 Inria. All rights reserved.
 *
 * This software was developed at:
 * IRISA/INRIA Rennes
 * Projet Lagadic
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * http://www.irisa.fr/lagadic
 *
 * This file is part of the ViSP toolkit.
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech AS of Norway and appearing in the file
 * LICENSE included in the packaging of this file.
 *
 * Licensees holding valid ViSP Professional Edition licenses may
 * use this file in accordance with the ViSP Commercial License
 * Agreement provided with the Software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Contact visp@irisa.fr if any conditions of this licensing are
 * not clear to you.
 *
 * Description:
 * Track a white dot.
 *
 * Authors:
 * Eric Marchand
 * Fabien Spindler
 *
 *****************************************************************************/

/*
  \file vpDot.h
  \brief Track a white dot
*/

#ifndef vpDot_hh
#define vpDot_hh

#include <math.h>
#include <fstream>

#include <visp/vpConfig.h>
#include <visp/vpList.h>

#include <visp/vpImage.h>
#include <visp/vpDisplay.h>

#include <visp/vpTracker.h>
#include <visp/vpRect.h>


/*
  \class vpDot
  \brief Track a white dot.

  \sa vpDotExample.cpp
*/
class VISP_EXPORT vpDot : public vpTracker
{
public:
  /*! \enum ConnexityEnum
  Type of connexity 4, or 8.
  */
  typedef enum {
    CONNEXITY_4, /*!< For a given pixel 4 neighbors are considered (left,
		   right, up, down) */
    CONNEXITY_8 /*!< For a given pixel 8 neighbors are considered (left,
		  right, up, down, and the 4 pixels located on the diagonal) */
  } ConnexityEnum;

  static const unsigned int SPIRAL_SEARCH_SIZE; /*!< Spiral size for the dot
						  search. */
private:

  //! internal use only
  vpList<unsigned int> Lu, Lv ;

  //! Type of connexity
  ConnexityEnum connexity;

  //! coordinates of the point center of gravity
  unsigned int cog_u, cog_v ;
  //! coordinates (float) of the point center of gravity
  double cog_ufloat, cog_vfloat ;

  // Bounding box
  int u_min, u_max, v_min, v_max;

  // Flag used to allow display
  bool graphics ;


public :
  double m00; /*!< Considering the general distribution moments for \f$ N \f$
		points defined by the relation \f$ m_{ij} = \sum_{h=0}^{N}
		u_h^i v_h^j \f$, \f$ m_{00} \f$ is a zero order moment obtained
		with \f$i = j = 0 \f$.

		\sa setComputeMoments()
	      */
  double m01; /*!< Considering the general distribution moments for \f$ N \f$
		points defined by the relation \f$ m_{ij} = \sum_{h=0}^{N}
		u_h^i v_h^j \f$, \f$ m_{01} \f$ is a first order moment
		obtained with \f$i = 0 \f$ and \f$ j = 1 \f$.

		\sa setComputeMoments()
	      */
  double m10; /*!< Considering the general distribution moments for \f$ N \f$
		points defined by the relation \f$ m_{ij} = \sum_{h=0}^{N}
		u_h^i v_h^j \f$, \f$ m_{10} \f$ is a first order moment
		obtained with \f$i = 1 \f$ and \f$ j = 0 \f$.

		\sa setComputeMoments()
	      */
  double m11; /*!< Considering the general distribution moments for \f$ N \f$
		points defined by the relation \f$ m_{ij} = \sum_{h=0}^{N}
		u_h^i v_h^j \f$, \f$ m_{11} \f$ is a first order moment
		obtained with \f$i = 1 \f$ and \f$ j = 1 \f$.

		\warning This moment is not computed for the moment.

		\sa setComputeMoments()
	      */
  double m20; /*!< Considering the general distribution moments for \f$ N \f$
		points defined by the relation \f$ m_{ij} = \sum_{h=0}^{N}
		u_h^i v_h^j \f$, \f$ m_{20} \f$ is a second order moment
		obtained with \f$i = 2 \f$ and \f$ j = 0 \f$.

		\warning This moment is not computed for the moment.

		\sa setComputeMoments()
	      */
  double m02; /*!< Considering the general distribution moments for \f$ N \f$
		points defined by the relation \f$ m_{ij} = \sum_{h=0}^{N}
		u_h^i v_h^j \f$, \f$ m_{02} \f$ is a second order moment
		obtained with \f$i = 0 \f$ and \f$ j = 2 \f$.

		\warning This moment is not computed for the moment.

		\sa setComputeMoments()
	      */
public:
  void init() ;
  vpDot() ;
  vpDot(const unsigned int u, const unsigned int v) ;
  vpDot(const double u, const double v) ;
  vpDot(const vpDot& c) ;
  virtual ~vpDot() ;

public:
  vpDot& operator =(const vpDot& f) ;
  bool operator ==(const vpDot& m);
  bool operator !=(const vpDot& m);


public:
  /*!

  Return the "u" (column) coordinate of the center of the dot within the image
  it comes from.
  */
  double get_u() const { return cog_ufloat ; }
  /*!

  Return the "v" (row) coordinate of the center of the dot within the image it
  comes from.
  */

  double get_v() const { return cog_vfloat ; }
  /*!

  Return the list of the "u" coordinates (row) of all the pixels on the dot
  border.

  \param u_list The "u" coordinate of the pixels on the dot border. This list
  is update after a call to track().


  */
  void   get_u(vpList<unsigned int> & u_list) { u_list = Lu; };
  /*!

  Return the list of the "v" coordinates (column) of all the pixels on the dot
  border.

  \param v_list The "v" coordinate of the pixels on the dot border. This list
  is update after a call to track().

  */
  void   get_v(vpList<unsigned int> & v_list) { v_list = Lv; };

  void set_u(double u) { cog_ufloat = u ; cog_u = (unsigned int)u ; }
  void set_v(double v) { cog_vfloat = v ; cog_v = (unsigned int)v ; }

  /*!
    Set the type of connexity: 4 or 8.
  */
  void setConnexity(ConnexityEnum connexity) {this->connexity = connexity; };

public:

  /*!
    Print the coordinates of the point center of gravity
    in the stream.
  */
  friend std::ostream& operator<< (std::ostream& os, vpDot& p) {
    return (os <<"("<<p.cog_ufloat<<","<<p.cog_vfloat<<")" ) ;
  } ;
  void print(std::ostream& os) { os << *this << std::endl ; }

private:
  enum pixelInDot
    {
      in,
      out
    } ;
  double nbMaxPoint ;
  int connexe(vpImage<unsigned char>& I, int u, int v,
	      unsigned int gray_level_min, unsigned int gray_level_max,
	      double &mean_value, double &u_cog, double &v_cog, double &n);
  void COG(vpImage<unsigned char> &I,double& u, double& v) ;

  unsigned int gray_level_min; // left threshold for binarisation
  unsigned int gray_level_max; // right threshold for binarisation
  double grayLevelPrecision;  //precision of the gray level of the dot.
  //It is a double precision float witch value is in ]0,1].
  //1 means full precision, whereas values close to 0 show a very bad precision

  //! flag : true moment are computed
  bool compute_moment ;

public:
  /*!

    Activates the dot's moments computation.

    \param activate true, if you want to compute the moments. If false, moments
    are not computed.

    Computed moment are vpDot::m00, vpDot::m10, vpDot::m01, vpDot::m11,
    vpDot::m20, vpDot::m02.

    The coordinates of the region's centroid (u, v) can be computed from the
    moments by \f$u=\frac{m10}{m00}\f$ and  \f$v=\frac{m01}{m00}\f$.

  */
  void setComputeMoments(const bool activate) { compute_moment = activate; }
  void initTracking(vpImage<unsigned char> &I) ;
  void initTracking(vpImage<unsigned char> &I, unsigned int u, unsigned int v);
  void initTracking(vpImage<unsigned char> &I, unsigned int u, unsigned int v,
		    unsigned int gray_level_min,
		    unsigned int gray_level_max);
  void track(vpImage<unsigned char> & I) ;
  void track(vpImage<unsigned char> & I, double &u, double &v) ;
  //! the maximum in pixel of a dot (default 5000 that is a radius of 40pixels)
  void setNbMaxPoint(double nb) ;


  /*!
    Activates the display of all the pixels of the dot during the tracking.

    \warning To effectively display the dot graphics a call to
    vpDisplay::flush() is needed.

    \param activate true to activate the display of dot pixels, false to turn
    off the display
  */
  void setGraphics(const bool activate) { graphics = activate ; }

  /*!

    Return the width of the dot.

    \sa getHeight()

  */
  inline unsigned int getWidth() const {
    return (this->u_max - this->u_min + 1);
  };

  /*!

    Return the width of the dot.

    \sa getHeight()

  */
  inline unsigned int getHeight() const {
    return (this->v_max - this->v_min + 1);
  };

  /*!

    Return the dot bounding box.

    \sa getWidth(), getHeight()

  */
  inline vpRect getBBox() {
    vpRect bbox;

    bbox.setRect(this->u_min,
		 this->v_min,
		 this->u_max - this->u_min + 1,
		 this->v_max - this->v_min + 1);

    return (bbox);
  };
} ;


#endif

/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */


