#ifndef LAP_UTILS_OPENMP_H
#define LAP_UTILS_OPENMP_H

namespace laplace {
namespace openmp {
  /*! \brief
   * Returns an integer equal to or greater than the number of threads
   * that would be available if a parallel region without num_threads were
   * defined at that point in the code.
   *
   * Acts as a wrapper for omp_get_max_threads().
   */
  int get_max_threads(void);

  /*! \brief
   * Returns the number of processors available when the function is called.
   *
   * Acts as a wrapper around omp_get_num_procs().
   */
  int get_num_procs(void);

  /*! \brief
   * Returns the thread number of the thread executing within its thread team.
   *
   * Acts as a wrapper for omp_get_thread_num().
   */
  int get_thread_num(void);

  /*! \brief
   * Sets the number of threads in subsequent parallel regions, unless overridden
   * by a num_threads clause.
   *
   * Acts as a wrapper for omp_set_num_threads().
   */
  void set_num_threads(int num_threads);
}}

#endif
