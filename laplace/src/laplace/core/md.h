#ifndef LAP_CORE_MD_H
#define LAP_CORE_MD_H

#include "laplace/laplacepre.h"
#include "laplace/core/mm_system.h"
#include "laplace/config/simulation_config.h"
#include <H5Cpp.h>

namespace laplace {
  void run_md(H5::H5File &outfile,
              MmSystem &system,
              const config::SimulationConfig &config);
}

#endif
