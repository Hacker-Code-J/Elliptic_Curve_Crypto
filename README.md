# Elliptic_Curve_Crypto

**- High-Performance Implementation for ECC -**

---

## NIST P-256 (secp256r1)

### Environment
- AMD Ryzen 5 5625U with Radeon Graphics @ 2.3 GHz

### My Performance
- Number of Samples:
  - Addition: 1,000,000,000 (1B)
  - Subtraction: 1,000,000,000 (1B)
  - Multiplication (reduction): 100,000,000 (100M)

| Operation \ Platform | 32-bit | 64-bit |
| --- | --- | --- |
| P-256 Addition | 22.23 cycle | 14.52 cycle |
| P-256 Subtraction | 22.23 cycle | 14.52 cycle |
| P-256 Multiplication | - cycle | - cycle |