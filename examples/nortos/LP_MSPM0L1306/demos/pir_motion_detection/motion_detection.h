
#ifndef INCLUDE_MOTION_DETECTION_H_
#define INCLUDE_MOTION_DETECTION_H_

#include <stdint.h>



//*******************Datalogging *********************
//Enable for datalogging
#define DATALOGGING_ENABLE

//Max size currently (due to Memory protection settings only) is 2048
#define PIR_DATALOGGING_LENGTH  120

extern uint32_t PIR_motion_detection_logging[];
extern uint32_t PIR_Inst_Detn_th_val[];
extern uint32_t PIR_abs_inst_dev_value[];
extern unsigned int PIR_Data_pointer;

//*****************************************************

//*******************Noise/Motion Filtering *********************
//Theshold of how many counts abs_inst_dev_value is higher than Inst_detn_th_val - length of motion detection
//#define PIR_MOTION_COUNT_THRESHOLD 200




/**** Local Function Prototypes ***********************************************/

void signal_analysis(uint32_t);

#endif /* INCLUDE_MOTION_DETECTION_H_ */

/*** end of file ***/
