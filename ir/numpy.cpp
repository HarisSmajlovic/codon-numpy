#include "numpy.h"

namespace numpy {

void NumPy::addIRPasses(codon::ir::transform::PassManager *pm, bool debug) {
  // nothing
}

} // namespace numpy

extern "C" std::unique_ptr<codon::DSL> load() {
  return std::make_unique<numpy::NumPy>();
}
