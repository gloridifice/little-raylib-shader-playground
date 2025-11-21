use crate::cpp_parser::CppStruct;

pub mod cmake_compile_commands;
pub mod cpp_parser;

pub fn generate_cpp(s: &CppStruct) -> String {
    let mut out = String::new();

    let class_name = s.get_name_with_namespace();

    out.push_str("RTTR_REGISTRATION\n{\n");
    out.push_str("    using namespace rttr;\n\n");
    out.push_str(&format!(
        "    registration::class_<{}>(\"{}\")\n",
        &class_name, &class_name
    ));

    for field in &s.fields {
        out.push_str(&format!(
            "        .property(\"{}\", &{}::{})\n",
            &field.name, &class_name, &field.name
        ));
    }

    out.push_str("    ;\n}\n");

    out
}
