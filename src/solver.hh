// Copyright (C) 2009 by Thomas Moulard, FIXME.
//
// This file is part of the liboptimization.
//
// liboptimization is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// liboptimization is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with liboptimization.  If not, see <http://www.gnu.org/licenses/>.

/**
 * \file src/solver.hh
 *
 * \brief Declaration of the Solver class.
 */

#ifndef OPTIMIZATION_SOLVER_HH
# define OPTIMIZATION_SOLVER_HH
# include <stdexcept>
# include <boost/utility.hpp>
# include <boost/variant/variant.hpp>

# include <fwd.hh>
# include <problem.hh>

namespace optimization
{
  /// \brief Base error.
  /// All other errors inherits this class.
  struct SolverError : public std::runtime_error
  {
    SolverError (const std::string& arg) throw ()
      : std::runtime_error (arg)
    {}
  };

  /// \brief Returned by Solver::getMinimum if no solution can be
  /// found (but no error has been encountered during the process).
  struct NoSolution {};

  /// Abstract interface satisfied by all solvers.
  class GenericSolver : public boost::noncopyable
  {
  public:
    /// Define the kind of solution which has been found.
    enum solutions {
      /// Solution has yet to be found.
      SOLVER_NO_SOLUTION,
      /// Solution has been found.
      SOLVER_VALUE,
      /// The solver failed to found a solution.
      SOLVER_ERROR
    };

    /// Vector type.
    typedef Function::vector_t vector_t;

    /// Result type.
    typedef boost::variant<NoSolution, vector_t, SolverError> result_t;

     /// \defgroup ctor Constructors and destructors.
    /// \{
    /// Main constructor.
    explicit GenericSolver () throw ();

    /// Destructor.
    virtual ~GenericSolver () throw ();
    /// \}

    /// Reset the internal mechanism to force the solution to be
    /// re-computed next time getMinimum is called.
    void reset () throw ();

    /// Solve the problem.
    /// Called automatically by getMinimum if required.
    virtual void solve () throw () = 0;

    /// Returns the function minimum (and solve the problem, if
    /// it has not yet been solved).
    const result_t& getMinimum () throw ();
  protected:
    /// Result of minimization.
    result_t result_;
  };

  template <typename F, typename C>
  class Solver : public GenericSolver
  {
  public:
    typedef Problem<F, C> problem_t;

    explicit Solver (const problem_t&) throw ();
    ~Solver () throw ();

    const problem_t& getProblem () const throw ();
  protected:
    const problem_t& problem_;
  };



} // end of namespace optimization

# include <solver.hxx>
#endif //! OPTIMIZATION_SOLVER_HH
