#ifndef TVMGEN_DEFAULT_H_
#define TVMGEN_DEFAULT_H_
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Symbol defined when running model on the host processor */
#define TVMGEN_DEFAULT_TI_NPU_SOFT
#ifdef TVMGEN_DEFAULT_TI_NPU
	#error Conflicting definition for where model should run.
#endif

/* The generated model library expects the following inputs/outputs:
 * Inputs:
 *    Tensor[(1, 1, 512, 1), float32]
 * Outputs:
 *    Tensor[(1, 3), float32]
 */


/*!
 * \brief Input tensor pointers for TVM module "default" 
 */
struct tvmgen_default_inputs {
  void* onnx__Add_0;
};

/*!
 * \brief Output tensor pointers for TVM module "default" 
 */
struct tvmgen_default_outputs {
  void* output;
};

/*!
 * \brief entrypoint function for TVM module "default"
 * \param inputs Input tensors for the module 
 * \param outputs Output tensors for the module 
 */
int32_t tvmgen_default_run(
  struct tvmgen_default_inputs* inputs,
  struct tvmgen_default_outputs* outputs
);

#ifdef __cplusplus
}
#endif

#endif // TVMGEN_DEFAULT_H_
