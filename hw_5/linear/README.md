# Linear Regression (hw_5/linear)

Linear regression using the Least Squares method: fit a line y = mx + b and evaluate with R², MSE, and MAE.

## Files

- `linear.h` – `DataPoint` struct and `LinearRegression` class declarations
- `regression.cpp` – implementation and `main()` (synthetic data, fit, metrics, PPM visualization)

## Build

```bash
cd hw_5/linear
make
```

## Run

```bash
./regression
```

Or:

```bash
make run
```

This generates synthetic data, fits the model, prints the equation and metrics (R², MSE, MAE), and saves `regression.ppm`.

## Clean

```bash
make clean
```

Removes the `regression` binary and `regression.ppm`.

## Output

- **Console**: Generated data info, fitted equation `y = mx + b`, R², MSE, MAE.
- **regression.ppm**: PPM image with data points (black) and regression line (gray).

View the PPM with an image viewer or convert to PNG, e.g. `convert regression.ppm regression.png`.
