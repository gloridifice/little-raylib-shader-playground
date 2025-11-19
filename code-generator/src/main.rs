use std::path::Path;

use code_generator::cpp_parser::parse_header;

fn main() {
    let a = parse_header(Path::new("header/hello.h"));
    for s in a.iter() {
        println!("{}", &s.name);
        for field in &s.fields {
            println!("{} {}", &field.0, &field.1);
        }
    }
}
