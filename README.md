# Random Walk Simulator (C++)

## Overview
This project implements a configurable random walk simulator in C++,
supporting 1D, 2D, and 3D walks with statistical analysis.

## Features
- Arbitrary number of steps
- Dimensionality selection
- Reproducible randomness
- CSV export
- Statistical analysis
- CMake build system

### Theory

A **random walk** is a discrete stochastic process where a particle takes successive random steps.

---

#### 1D random walk

The position after $n$ steps is

$$
\mathbf{X}_n=\sum_{i=1}^{n}S_i,
$$

where each step $S_i\in\{-1,+1\}$ is independent and identically distributed.  
**Key results:**  
- **Mean:** $\mathbb{E}[X_n]=0$.  
- **Variance:** $\mathrm{Var}(X_n)=n$.  
- **Scaling:** $\sqrt{\mathbb{E}[X_n^2]}\propto\sqrt{n}$.

---

#### 2D random walk

The position vector after $n$ steps is

$$
\mathbf{X}_n=\sum_{i=1}^{n}\mathbf{S}_i,\qquad \mathbf{X}_n\in\mathbb{Z}^2,
$$

with each step $\mathbf{S}_i$ chosen from $\{\pm\hat{x},\pm\hat{y}\}$ or from an isotropic angular distribution.  
**Key results:**  
- **Mean vector:** $\mathbb{E}[\mathbf{X}_n]=\mathbf{0}$.  
- **Mean squared displacement:** $\mathbb{E}\big[\|\mathbf{X}_n\|^2\big]=n$ (unit step length).  
- **Recurrence:** Simple symmetric walks on $\mathbb{Z}^2$ are **recurrent** (return to the origin with probability 1).

---

#### 3D random walk

For three dimensions,

$$
\mathbf{X}_n=\sum_{i=1}^{n}\mathbf{S}_i,\qquad \mathbf{X}_n\in\mathbb{Z}^3,
$$

with $\mathbf{S}_i$ chosen from $\{\pm\hat{x},\pm\hat{y},\pm\hat{z}\}$ or isotropically on the unit sphere.  
**Key results:**  
- **Mean:** $\mathbb{E}[\mathbf{X}_n]=\mathbf{0}$.  
- **Mean squared displacement:** $\mathbb{E}\big[\|\mathbf{X}_n\|^2\big]=n$ (unit steps).  
- **Transience:** Simple symmetric walks on $\mathbb{Z}^3$ are **transient** (nonzero probability of never returning to the origin).

---

#### Important properties

- **RMS scaling:** Root mean squared displacement grows as $\sqrt{n}$ in all dimensions:

$$
\sqrt{\mathbb{E}\big[\|\mathbf{X}_n\|^2\big]}\propto\sqrt{n}.
$$
  
- **Central Limit Theorem:** For large $n$, each coordinate (properly normalized) converges in distribution to a normal law:  

$$
\frac{X_n}{\sqrt{n}}\xrightarrow{d}\mathcal{N}(0,1).
$$
  
- **Recurrence versus transience:** Affects long‑term return probabilities and tail behavior: recurrent for $d=1,2$; transient for $d\ge 3$.





