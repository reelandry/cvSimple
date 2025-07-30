# Minimal CVODE Example

## Requirements
- SUNDIALS installed at `$HOME/sundials/install`
- GCC

## Build
```bash
gcc -o cvSimple cvSimple.c \
  -I$HOME/sundials/install/include \
  -L$HOME/sundials/install/lib -lsundials_cvode -lsundials_nvecserial -lsundials_core -lm
