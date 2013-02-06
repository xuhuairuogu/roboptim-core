
// Copyright (c) 2011 CNRS
// Authors: Florent Lamiraux
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

#include <roboptim/core/debug.hh>
#include "roboptim/core/sum-of-c1-squares.hh"

namespace roboptim {

  SumOfC1Squares::SumOfC1Squares
  (const boost::shared_ptr<DifferentiableFunction>& function,
   const std::string& name) throw ()
    : DifferentiableFunction(function->inputSize(), 1, name),
      baseFunction_ (function),
      x_ (),
      value_ (),
      gradient_ ()
  {
    value_.resize (function->outputSize());
    gradient_.resize (function->inputSize());
    x_.resize (function->inputSize());
    x_.setZero ();
    (*baseFunction_) (value_, x_);
  }
    
  SumOfC1Squares::SumOfC1Squares (const SumOfC1Squares& src) throw ()
    : DifferentiableFunction(src.inputSize(), 1, src.getName()),
      baseFunction_ (src.baseFunction_),
      x_ (src.x_),
      value_ (src.value_),
      gradient_ (src.gradient_)
  {
  }

  SumOfC1Squares::~SumOfC1Squares () throw ()
  {
  }

  const boost::shared_ptr<const DifferentiableFunction>& SumOfC1Squares::
  baseFunction () const
  {
    return baseFunction_;
  }

  void SumOfC1Squares::
  impl_compute(result_t result, argument_t x) const throw ()
  {
    computeFunction (x);
    value_t sumSquares = 0;
    for (size_t i = 0; i < value_.size(); i++) {
      value_t y = value_[i];
      sumSquares += y*y;
    }
    result[0] = sumSquares;
  }

  void SumOfC1Squares::
  impl_gradient(gradient_t gradient, argument_t x,
		size_type ROBOPTIM_DEBUG_ONLY (row)) const throw ()
  {
    assert (row == 0);
    computeFunction (x);
    gradient.block (0, 0, gradient.rows (), gradient.cols ()).setZero ();
    for (size_t i = 0; i < value_.size(); i++) {
      value_t y = value_[i];
      baseFunction_->gradient(gradient_, x, i);
      gradient += 2*y*gradient_;
    }
  }

  void SumOfC1Squares::computeFunction (const argument_t x) const
  {
    if (x != x_) {
      x_ = x;
      (*baseFunction_) (value_, x_);
    }
  }
} // namespace roboptim
