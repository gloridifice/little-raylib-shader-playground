use anyhow::anyhow;
use clap::Parser;
use std::{ffi::CString, fs, path::Path, str::FromStr};
use walkdir::WalkDir;

use code_generator::{cmake_compile_commands, cpp_parser::parse_header, generate_cpp};

#[derive(Parser)]
struct Args {
    input: String,
    #[arg(short, long)]
    output: String,
    #[arg(short, long)]
    cmake_compile_commands_path: String,
}

fn is_valid_dir<'a>(input: &'a String) -> anyhow::Result<()> {
    let input_path = Path::new(input);
    if !input_path.exists() {
        return Err(anyhow!(""));
    }
    if !input_path.is_dir() {
        return Err(anyhow!(""));
    }
    Ok(())
}

fn main() -> anyhow::Result<()> {
    color_backtrace::install();
    let Args {
        input,
        output,
        cmake_compile_commands_path,
    } = Args::parse();

    is_valid_dir(&input).unwrap();
    is_valid_dir(&output).unwrap();

    let json_string = fs::read_to_string(&cmake_compile_commands_path)?;

    let include_args = cmake_compile_commands::get_include_args_of_parser(&json_string)?;

    let mut include_args = include_args
        .iter()
        .map(|it| CString::from_str(&format!("-I{}", it)).unwrap())
        .collect::<Vec<_>>();

    let mut args = vec![
        CString::new("-x").unwrap(),
        CString::new("c++").unwrap(),
        CString::new("-std=c++20").unwrap(),
    ];
    args.append(&mut include_args);

    for entry in WalkDir::new(&input) {
        if let Ok(entry) = entry {
            let a = parse_header(entry.path(), &args);

            for s in a.iter() {
                let output = generate_cpp(s);
                println!("{}", output);
            }
        }
    }

    Ok(())
}
