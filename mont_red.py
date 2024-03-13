def montgomery_reduction(a, m, m_prime, r):
    """
    Perform Montgomery reduction.
    :param a: The number to be reduced.
    :param m: The modulus.
    :param m_prime: -m^(-1) mod R. Precomputed value for the modulus.
    :param r: The R value, should be a power of 2 larger than m.
    :return: The Montgomery reduction of a mod m.
    """
    # Calculate t = (a * m_prime) % r
    t = (a * m_prime) % r
    # Calculate u = (a + t * m) / r
    u = (a + t * m) // r
    if u >= m:
        u -= m
    return u

# Example usage
m = 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff  # Example modulus
R = 0x10000000000000000000000000000000000000000000000000000000000000000  # R is a power of 2 larger than m, choose appropriately
m_prime = -pow(m, -1, R)  # Compute m' such that m * m' ≡ -1 (mod R)
print(hex(m_prime + R))
# Let's say we want to reduce a = 'val' mod 'm'
# First, convert a to Montgomery form: 'aR' mod 'm'
val = 0xEAC8C20CE80D90B3BF6DC08AFD4C8B1D6274839279D9F1469BE87CCD85283FE5 * 0xF54ECAE2F71FC58022DF670B74F41B07195BEEC269F85DDC1368137BC3DD50B3
aR = (val * R) % m

# Perform Montgomery reduction
# reduced = montgomery_reduction(aR, m, m_prime, R)
precomputed_m_prime = 0xffffffff00000002000000000000000000000001000000000000000000000001
reduced = montgomery_reduction(aR, m, precomputed_m_prime, R)

# Convert back from Montgomery form
result = (reduced * 1) % m  # Here, 1 is the Montgomery representation of 1, i.e., R mod m
print("Reduced result:", hex(result))