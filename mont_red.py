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
# print(hex(m_prime + R))
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
# print("Reduced result:", hex(result))

def mont_debugging(u):
    N = 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff
    R = 0x10000000000000000000000000000000000000000000000000000000000000000
    t = u
    for i in range(8):
        s = u % (2**32)
        _s = s * N * ((2**32) ** i)
        _s_formatted = f"{_s:0128x}"
        t += _s
        print(f"i : {i} | {s:#x} | 0x{_s_formatted} | {t:#x}")
        u = t >> (32 * (i+1))
    print("out of loop:")
    t  >>= 256
    print(f"{t:#x}")
    if t >= N:
        t -= N
    print(f"{t:#x}")
    res = (t * R) % N
    print(f"{res:#X}")
        
# u = 0xe0fa64f40b0204c9a245a2e1ba35254bf342087cc4600be85ab506e36fa5f190ab84085811050b0f4372273fa5522a106ee1785d607cda60df85cbc246cd3d1f
# mont_debugging(u)

# u7 = 0xEAC8C20C
# u6 = 0xE80D90B3
# u5 = 0xBF6DC08A
# u4 = 0xFD4C8B1D
# u3 = 0x62748392
# u2 = 0x79D9F146
# u1 = 0x9BE87CCD
# u0 = 0x85283FE5 
# v7 = 0xF54ECAE2
# v6 = 0xF71FC580
# v5 = 0x22DF670B
# v4 = 0x74F41B07
# v3 = 0x195BEEC2
# v2 = 0x69F85DDC
# v1 = 0x1368137B
# v0 = 0xC3DD50B3

'''
def mont_test():
    u = 0xEAC8C20CE80D90B3BF6DC08AFD4C8B1D6274839279D9F1469BE87CCD85283FE5
    v = 0xF54ECAE2F71FC58022DF670B74F41B07195BEEC269F85DDC1368137BC3DD50B3
    # u = [
    #     0x85283FE5,  # u0
    #     0x9BE87CCD,  # u1
    #     0x79D9F146,  # u2
    #     0x62748392,  # u3
    #     0xFD4C8B1D,  # u4
    #     0xBF6DC08A,  # u5
    #     0xE80D90B3,  # u6
    #     0xEAC8C20C   # u7
    # ]
    # v = [
    #     0xC3DD50B3,  # v0
    #     0x1368137B,  # v1
    #     0x69F85DDC,  # v2
    #     0x195BEEC2,  # v3
    #     0x74F41B07,  # v4
    #     0x22DF670B,  # v5
    #     0xF71FC580,  # v6
    #     0xF54ECAE2   # v7
    # ]
    # t = [
    #     0x00000000,
    #     0x00000000,
    #     0x00000000,
    #     0x00000000,
    #     0x00000000,
    #     0x00000000,
    #     0x00000000,
    #     0x00000000
    # ]
    t = 0
    for i in range(1):
        N = 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff  # Example modulus
        R = 0x10000000000000000000000000000000000000000000000000000000000000000  # R is a power of 2 larger than m, choose appropriately
        print(f"{(u >> (32 * i)) & 0xFFFFFFFF:#x}")
        print(f"{(v & 0xFFFFFFFF):#x}")
        # m = (((t & 0xFFFFFFFF) + ((u >> (32 * i)) & 0xFFFFFFFF)*(v & 0xFFFFFFFF)) * 0xffffffff00000002000000000000000000000001000000000000000000000001) % (2 ** 32)
        m = (((t & 0xFFFFFFFF) + (((u >> (32 * i)) & 0xFFFFFFFF)*(v & 0xFFFFFFFF) % (2**32)) ) * 1)
        # print(f"m: {m:#x}")
        s = m * 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff
        print(f"s: {s:#x}")
        print(f"s: {((u >> (32 * i)) & 0xFFFFFFFF)*v:#x}")
        t += ((u >> (32 * i)) & 0xFFFFFFFF)*v + s
        print(f"t: {t:#x}")
        t >>= 32
        print(f"t: {t:#x}")
        if t >= 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff:
            t -= 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff
        print(f"{t % N:X}")
        # print(f"{(t*0x10000000000000000000000000000000000000000000000000000000000000000) % 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff:#X}")
        # print(f"{(0x10000000000000000000000000000000000000000000000000000000000000000) % 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff:#x}")
        # print(f"{t % N:#x}")
        # print(f"{t % (2**256):#x}")
        
# mont_test()
'''

# 24-03-26
def mont_test():
    N = 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff      # p_256
    R = 0x10000000000000000000000000000000000000000000000000000000000000000     # R = 2 ** 256    
    u = 0xEAC8C20CE80D90B3BF6DC08AFD4C8B1D6274839279D9F1469BE87CCD85283FE5      # Operand 1
    v = 0xF54ECAE2F71FC58022DF670B74F41B07195BEEC269F85DDC1368137BC3DD50B3      # Operand 2
    t = 0
    for i in range(8):
        print(f"\n==== {i} ====\n")
        print(f"u_{i} = {(u >> (32 * i)) & 0xFFFFFFFF:#x}") # u_i for u= u7 || ... || u_0
        print(f"v_0 = {(v & 0xFFFFFFFF):#x}")               # v_0
        
        # m = (((t & 0xFFFFFFFF) + ((u >> (32 * i)) & 0xFFFFFFFF)*(v & 0xFFFFFFFF)) * 0xffffffff00000002000000000000000000000001000000000000000000000001) % (2 ** 32)
        # m = (((t & 0xFFFFFFFF) + (((u >> (32 * i)) & 0xFFFFFFFF)*(v & 0xFFFFFFFF) % (2**32)) ) * 1)
        m = (((t & 0xFFFFFFFF) + (((u >> (32 * i)) & 0xFFFFFFFF)*(v & 0xFFFFFFFF) % (2**32)) ))
        print(f"((t_0 + u_iv_0) x N') mod (2 ** 32):\n m_{i} = {m:#X}")
        
        s = m * N 
        # s = m * 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff
        print(f"m_{i} x N = {m:#X} x N \n = {s:#X}")
        
        t += ((u >> (32 * i)) & 0xFFFFFFFF)*v + s
        print(f"u_{i} x v:\n {((u >> (32 * i)) & 0xFFFFFFFF)*v:#x}")
        print(f"u_{i} x v + (m_{i} x N):\n {((u >> (32 * i)) & 0xFFFFFFFF)*v + s:#x}")
        print(f"Check:\n {t:#X}")
        
        t >>= 32
        print(f"% (2 ** 32): {t:#X}")
        
    if t >= N:
        t -= N
    print(f"{t*R % N:#X}")
        
mont_test()
    