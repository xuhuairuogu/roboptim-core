// Copyright (C) 2009 by Thomas Moulard, AIST, CNRS, INRIA.
//
// This file is part of the roboptim.
//
// roboptim is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// roboptim is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with roboptim.  If not, see <http://www.gnu.org/licenses/>.

#include "shared-tests/common.hh"

#include <iostream>

#include <roboptim/core/io.hh>
#include <roboptim/core/quadratic-function.hh>

using namespace roboptim;

struct Null : public QuadraticFunction
{
  Null () : QuadraticFunction (1, 1, "null function")
  {}

  void impl_compute (result_t res, argument_t) const throw ()
  {
    res.block (0, 0, res.rows (), res.cols ()).setZero ();
  }

  void impl_gradient (gradient_t grad, argument_t,
		      size_type) const throw ()
  {
    grad.block (0, 0, grad.rows (), grad.cols ()).setZero ();
  }

  void impl_hessian (hessian_t h, argument_t,
		     size_type) const throw ()
  {
    h.block (0, 0, h.rows (), h.cols ()).setZero ();
  }
};

struct NoTitle : public QuadraticFunction
{
  NoTitle () : QuadraticFunction (1, 1)
  {}

  void impl_compute (result_t res, argument_t) const throw ()
  {
    res.block (0, 0, res.rows (), res.cols ()).setZero ();
  }

  void impl_gradient (gradient_t grad, argument_t,
		      size_type) const throw ()
  {
    grad.block (0, 0, grad.rows (), grad.cols ()).setZero ();
  }

  void impl_hessian (hessian_t h, argument_t, size_type)
    const throw ()
  {
    h.block (0, 0, h.rows (), h.cols ()).setZero ();
  }
};

BOOST_AUTO_TEST_CASE (quadratic_function)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("quadratic-function");

  Null null;
  NoTitle notitle;

  (*output) << null << std::endl
	    << notitle << std::endl;

  Null::vector_t x (1);
  x[0] = 42.;

  (*output) << null.gradient (x) << std::endl
	    << notitle.gradient (x) << std::endl;

  (*output) << null.hessian (x) << std::endl
	    << notitle.hessian (x) << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}
