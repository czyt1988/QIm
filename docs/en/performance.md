# Qt High-Performance Plotting Library Performance Comparison

In the Qt ecosystem, there are few plotting libraries. The mainstream ones are `QCustomPlot`, `Qwt`, `Qt Charts`, and `KDChart`.

!!! info "Info"
    Comparison source code located at `benchmark/performance`

## Test Method

### Core Principles

Tests designed around **fairness, reproducibility, realism**:
- **Fairness**: All libraries use identical test datasets, render sizes, test procedures
- **Reproducibility**: Fixed random seed, unified warmup frames, test frame counts
- **Realism**: Simulate real business scenarios with dynamic data updates

### Test Configuration

- Render canvas: 800×600 pixels (all libraries)
- Warmup frames: 10 frames default
- Test frames: 100 frames default
- Data points: 10000/50000/100000/500000/1000000

## Test Results

### Test 1: Downsampling OFF + OpenGL OFF

| Library | Render Time (ms) | FPS | Memory (MB) |
|:--------:|:----------------:|:---:|:-----------:|
| QIm | 92.45 | 10.82 | 459.98 |
| Qwt | 144.1 | 6.94 | 20.94 |
| QCustomPlot | 249.3 | 4.01 | 20.80 |

### Test 2: Downsampling ON + OpenGL OFF

| Library | Render Time (ms) | FPS | Memory (MB) |
|:--------:|:----------------:|:---:|:-----------:|
| QIm | 42.39 | 23.59 | 339.84 |
| Qwt | 43.17 | 23.16 | 21.20 |
| QCustomPlot | 41.33 | 24.20 | 20.84 |

### Test 3: Downsampling ON + OpenGL ON

| Library | Render Time (ms) | FPS | Memory (MB) |
|:--------:|:----------------:|:---:|:-----------:|
| QIm | 46.99 | 21.28 | 449.75 |
| Qwt | 46.55 | 21.48 | 32.66 |
| QCustomPlot | 45.46 | 22.00 | 0 |

## Conclusions

| Scenario | Performance Ranking | Key Feature |
|----------|---------------------|-------------|
| **No downsampling + No OpenGL** | **QIm >> Qwt > QCustomPlot** | QIm leads 1.5-2x at million-level data |
| **With downsampling (LTTB)** | **Similar performance** | Difference within 10% |
| **Memory usage** | **Qwt ≈ QCustomPlot << QIm** | QIm uses 5-15x more memory |

### Key Findings

1. **Downsampling is decisive for large data rendering** (>100K points)
2. **OpenGL acceleration has limited actual benefit** when downsampling is enabled
3. **Memory usage shows significant differences**

See [Chinese version](../zh/performance.md) for detailed test methodology and full results.