// use secp256r1_rust::arithmetic;

// use secp256r1_rust::arithmetic::bignumber;

// use arithmetic::bignumber_config::P256;

use secp256r1_rust::arithmetic;

fn main() {
    println!("{:?}", arithmetic::bignumber_config::consts::P256);
}
