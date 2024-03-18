// bignumber_config.rs

#[cfg(target_pointer_width = "32")]
pub mod consts {
    pub const ONE: u32 = 0x1;
    pub const SIZE: usize = 8;
    pub type Word = u32;
    pub type Field = [Word; SIZE];

    pub const P256: Field = [
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000,
        0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF,
    ];
    
    pub const P256_INVERSE: Field = [
        0x00000001, 0x00000000, 0x00000000, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE, 0x00000000,
    ];
}

#[cfg(target_pointer_width = "64")]
pub mod consts {
    pub const ONE: u64 = 0x1;
    pub const SIZE: usize = 4;
    pub type Word = u64;
    pub type Field = [Word; SIZE];

    pub const P256: Field = [
        0xFFFFFFFFFFFFFFFF, 0x00000000FFFFFFFF,
        0x0000000000000000, 0xFFFFFFFF00000001,
    ];
    pub const P256_INVERSE: Field = [
        0x0000000000000001, 0xFFFFFFFF00000000,
        0xFFFFFFFFFFFFFFFF, 0x00000000FFFFFFFE,
    ];
}