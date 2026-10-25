# C++ Equation Solver

A small educational console project exploring polymorphism, expression parsing and equation solving in C++.

Uses **Boost.Regex** to validate supported input formats and **muParser** to evaluate expressions. This is a learning project with deliberately limited grammars and numerical methods, not a general-purpose computer algebra system.

## Quick example

Running without arguments demonstrates all three implementations behind the `Root` interface:

```text
x^2 - 3*x + 2 = 0: x1 = 1, x2 = 2
(x^2 - 4) / (x + 2) = 0: x ~= 2 (grid search)
2.3*x - 5 = 7 - 0.1*x: x ~= 5 (grid search)
```

The fraction example excludes `x = -2` because its denominator is zero.

## Quick start with Docker

Start Docker Desktop in Linux-container mode. From this repository folder:

```bash
docker build -t equation-solver .
docker run --rm equation-solver
docker run --rm equation-solver quadratic "x^2 - 3*x + 2 = 0"
docker run --rm equation-solver fraction "(x^2 - 4) / (x + 2) = 0"
docker run --rm equation-solver numeric "3*(x - 5) = 18"
```

The single-stage image installs Ubuntu's build dependencies, compiles the project and runs the existing tests during the build. If a test fails, the image build fails. Running the image starts the program as an unprivileged user; no ports or volumes are needed. The first build requires internet access.

To run the tests again:

```bash
docker run --rm --entrypoint ctest equation-solver --test-dir /app/build --output-on-failure
```

The image includes the compiler and test executable. Ubuntu packages are not pinned to exact versions, so rebuilds can receive package updates.

## Build without Docker

Requirements: a C++17 compiler, CMake 3.16 or newer, Boost.Regex and muParser. Dependencies are not bundled as DLLs. CMake is the maintained build entry point; IDE project files can be generated from it.

### Windows (Visual Studio and vcpkg)

Install the Visual Studio **Desktop development with C++** workload and [vcpkg](https://learn.microsoft.com/vcpkg/get_started/get-started). Open Developer PowerShell, then run from the repository folder:

```powershell
$env:VCPKG_ROOT = 'C:/path/to/vcpkg'
cmake -S . -B build -A x64 "-DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
./build/Debug/equation-solver.exe
```

`vcpkg.json` declares both dependencies and pins the registry baseline. The toolchain must be supplied during the **first** configure step. To change toolchains, use a new build directory. Visual Studio can also open this folder as a CMake project.

### Ubuntu / WSL

```bash
sudo apt-get install build-essential cmake libboost-regex-dev libmuparser-dev
cmake -S . -B build-linux -DCMAKE_BUILD_TYPE=Debug
cmake --build build-linux --parallel
ctest --test-dir build-linux --output-on-failure
./build-linux/equation-solver
```

The Ubuntu commands use distribution packages rather than the pinned vcpkg registry. Tests cover 23 regression cases and three CLI checks, verified with MSVC on Windows and GCC 13 in the Ubuntu 24.04 container.

## Usage

Quote the equation so the shell passes it as one argument. On Windows, replace the executable path below with `./build/Debug/equation-solver.exe`.

```bash
./build-linux/equation-solver quadratic "x^2 - 3*x + 2 = 0"
./build-linux/equation-solver fraction "(x^2 - 4) / (x + 2) = 0"
./build-linux/equation-solver numeric "3*(x - 5) = 18"
./build-linux/equation-solver --help
```

Exit codes: `0` for a completed result, `1` for invalid input or a failed search, `2` for invalid command-line usage. A quadratic with no real roots is a valid completed result.

## Solvers and limitations

| Mode | Implementation | Supported input / output |
| --- | --- | --- |
| `quadratic` | `Quadratic_Equations` | `a*x^2 + b*x + c = 0`; all three terms must be present. Returns real roots, handles a repeated root and the `a = 0` cases. |
| `fraction` | `Linear_Equation` (historical class name) | `(numerator) / (denominator) = 0`; no nested parentheses in the two expressions. Returns the first valid grid candidate. |
| `numeric` | `Root_from_Root` | One `left = right` equation, with muParser expressions on each side. Returns the first grid candidate. |

- Quadratic coefficients may be decimal or scientific notation. Coefficients `1` and `-1` may be omitted before `x`. The old `+ -3*x` spelling is accepted. Missing terms must be written explicitly, e.g. `x^2 + 0*x - 4 = 0`.
- Quadratic arithmetic uses `double`, scales coefficients and uses a cancellation-resistant formula. Near-zero discriminants use a floating-point tolerance; extreme coefficient ranges are still limited by floating-point precision.
- Both grid searches sample **[-100, 100] with step 0.01**. A candidate needs an absolute expression residual at most `1e-7`; this is not a guarantee of that much accuracy in `x`.
- The fraction search checks the numerator residual and requires a finite denominator with magnitude above `1e-12`. It can therefore skip valid points with very small denominators.
- Grid searches do not find all roots or roots between sample points. For example, `3*x = 1` is not found. A failed search **does not prove that the equation has no solution**. Identities are not classified by the grid modes.
- Non-finite samples are skipped. Syntax errors are reported. Domain behaviour follows muParser; some expressions can raise an evaluation error instead of returning a non-finite sample.

## Code map

- `root.h`, `root.cpp`: common solver interface, result access and shared grid search.
- `Quadratic_Equations.*`: typed coefficients and quadratic/degenerate solving.
- `Linear_Equation.*`: fraction parsing and denominator validation.
- `Root_from_Root.*`: expression equality parsing.
- `main.cpp`: examples and command-line entry point, owning solvers through `std::unique_ptr<Root>`.
- `tests/solver_tests.cpp`: regression cases for valid results, invalid input, excluded roots, repeated calls and numeric limitations.

## Dependencies

- [Boost.Regex](https://www.boost.org/libs/regex/): input matching.
- [muParser](https://github.com/beltoforion/muparser): expression evaluation.

Dependency licenses are provided by their respective distributions.
