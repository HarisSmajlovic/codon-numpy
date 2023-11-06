#pragma once

#include "codon/dsl/dsl.h"

namespace numpy {

class NumPy : public codon::DSL {
public:
  void addIRPasses(codon::ir::transform::PassManager *pm, bool debug) override;
};

} // namespace numpy
