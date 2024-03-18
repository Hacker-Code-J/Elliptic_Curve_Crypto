// In utils.rs
use std::fmt;

pub const SIZE: usize = 4;
pub type Word = u64; // Assuming Word is your type alias for elements in the field.

pub struct HexField(pub [Word; SIZE]);

impl fmt::Debug for HexField {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        // Collect formatted strings into a vector
        let hex_strings: Vec<String> = self.0.iter()
                                             .map(|&num| format!("{:#x}", num))
                                             .collect();

        // Use the vector of owned strings for output
        f.debug_list().entries(hex_strings.iter()).finish()
    }
}
