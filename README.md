# Elliptic_Curve_Crypto

**- High-Performance Implementation for ECC -**

---

## NIST P-256 (secp256r1)

### Environment
- AMD Ryzen 5 5625U with Radeon Graphics @ 2.3 GHz

### My Performance
- Number of Samples:
  - Addition: 10,000,000,000 (10B)
  - Subtraction: 10,000,000,000 (10B)
  - Multiplication (reduction): 10,000,000,000 (10B)
  - Multiplication I : 100,000,000 (100M)
  - Multiplication II: 100,000,000 (100M)

| Operation \ Platform | 32-bit (cycle) | 64-bit (cycle) |
| --- | --- | --- |
| P-256 Addition $\mathbb{F}_2^{256}\times\mathbb{F}_2^{256}\to\mathbb{F}_2^{256}$ | 22.15 | 14.38 |
| P-256 Subtraction $\mathbb{F}_2^{256}\times\mathbb{F}_2^{256}\to\mathbb{F}_2^{256}$ | 23.86 | 16.00 |
| Multiplication I $\mathbb{F}_2^{256}\times\mathbb{F}_2^{256}\to\mathbb{F}_2^{512}$ | 4878.74 | 915.33 |
| Multiplication II $\mathbb{F}_2^{256}\times\mathbb{F}_2^{256}\to\mathbb{F}_2^{512}$ | 1091.18 | 304.60 |
| P-256 Multiplication $\mathbb{F}_2^{256}\times\mathbb{F}_2^{256}\to\mathbb{F}_2^{512}\to\mathbb{F}_2^{256}$ | 1393.11 | 521.08 |