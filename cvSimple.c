# ============================================================================
# Copyright 2025 Richard Spencer Landry Jr, Ph.D.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at:
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================

#include <stdio.h>
#include <cvode/cvode.h>
#include <nvector/nvector_serial.h>

int f(realtype t, N_Vector y, N_Vector ydot, void *user_data) {
    NV_Ith_S(ydot,0) = -0.04 * NV_Ith_S(y,0) + 1e4 * NV_Ith_S(y,1) * NV_Ith_S(y,2);
    return 0;
}

int main() {
    N_Vector y = N_VNew_Serial(3);
    NV_Ith_S(y,0) = 1.0; NV_Ith_S(y,1) = 0.0; NV_Ith_S(y,2) = 0.0;

    void *cvode_mem = CVodeCreate(CV_BDF);
    CVodeInit(cvode_mem, f, 0.0, y);
    CVodeSStolerances(cvode_mem, 1e-4, 1e-8);
    CVodeSetUserData(cvode_mem, NULL);

    realtype t = 0.0;
    for (int i = 1; i <= 10; i++) {
        CVode(cvode_mem, i * 1.0, y, &t, CV_NORMAL);
        printf("t = %g, y0 = %.6f\n", t, NV_Ith_S(y,0));
    }

    N_VDestroy(y);
    CVodeFree(&cvode_mem);
    return 0;
}
