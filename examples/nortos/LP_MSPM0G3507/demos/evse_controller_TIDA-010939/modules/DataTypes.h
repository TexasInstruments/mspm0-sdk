/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * 	Modified: Texas Instruments Incorporated, 2025
 */

#ifndef SRC_EVDRIVERS_CPSTATE_H_
#define SRC_EVDRIVERS_CPSTATE_H_

#include "pb/protobuf/tida010939.pb.h"

enum class InternalCPState { Disabled, A, B, C, D, E, F, DF };

inline CpState to_CpState(InternalCPState input_state)
{
    CpState output_state;

    switch (input_state) {
        case InternalCPState::A:
            output_state = CpState::CpState_STATE_A;
            break;
        case InternalCPState::B:
            output_state = CpState::CpState_STATE_B;
            break;
        case InternalCPState::C:
            output_state = CpState::CpState_STATE_C;
            break;
        case InternalCPState::D:
            output_state = CpState::CpState_STATE_D;
            break;

        case InternalCPState::E:
            output_state = CpState::CpState_STATE_E;
            break;
        case InternalCPState::F:
            output_state = CpState::CpState_STATE_F;
            break;
        default:
            output_state = CpState::CpState_STATE_F;
            break;
    }

    return output_state;
}
#endif  // SRC_EVDRIVERS_CPSTATE_H_
