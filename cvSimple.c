#include <stdio.h>
#include <cvode/cvode.h>                  // prototypes for CVODE
#include <nvector/nvector_serial.h>       // serial N_Vector
#include <sundials/sundials_types.h>      // sunrealtype
#include <sunmatrix/sunmatrix_dense.h>    // dense SUNMatrix
#include <sunlinsol/sunlinsol_dense.h>    // dense SUNLinearSolver
#include <sundials/sundials_context.h>    // SUNContext

/* ODE: dy/dt = f(t, y) */
int f(sunrealtype t, N_Vector y, N_Vector ydot, void *user_data) {
    NV_Ith_S(ydot,0) = -0.04 * NV_Ith_S(y,0) + 1e4 * NV_Ith_S(y,1) * NV_Ith_S(y,2);
    NV_Ith_S(ydot,1) =  0.04 * NV_Ith_S(y,0) - 1e4 * NV_Ith_S(y,1) * NV_Ith_S(y,2)
                       - 3e7 * NV_Ith_S(y,1) * NV_Ith_S(y,1);
    NV_Ith_S(ydot,2) =  3e7 * NV_Ith_S(y,1) * NV_Ith_S(y,1);
    return 0;
}

int main() {
    /* Context */
    SUNContext sunctx;
    SUNContext_Create(0, &sunctx);

    /* Initial conditions */
    N_Vector y = N_VNew_Serial(3, sunctx);
    NV_Ith_S(y,0) = 1.0;
    NV_Ith_S(y,1) = 0.0;
    NV_Ith_S(y,2) = 0.0;

    /* CVODE memory */
    void *cvode_mem = CVodeCreate(CV_BDF, sunctx);
    CVodeInit(cvode_mem, f, 0.0, y);

    /* Tolerances */
    sunrealtype reltol = 1e-6;
    sunrealtype abstol = 1e-8;
    CVodeSStolerances(cvode_mem, reltol, abstol);

    /* Create dense matrix and linear solver */
    SUNMatrix A = SUNDenseMatrix(3, 3, sunctx);
    SUNLinearSolver LS = SUNLinSol_Dense(y, A, sunctx);
    CVodeSetLinearSolver(cvode_mem, LS, A);

    /* Integrate */
    sunrealtype t = 0.0;
    for (int i = 1; i <= 10; i++) {
        CVode(cvode_mem, i * 1.0, y, &t, CV_NORMAL);
        printf("t = %.2f, y0 = %.6f\n", t, NV_Ith_S(y,0));
    }

    /* Free memory */
    SUNLinSolFree(LS);
    SUNMatDestroy(A);
    CVodeFree(&cvode_mem);
    N_VDestroy(y);
    SUNContext_Free(&sunctx);

    return 0;
}
