use crate::cpp_parser::CppStruct;

pub mod cpp_parser;

pub fn generate_cpp(s: CppStruct) -> String {
    let mut out = String::new();

    out.push_str("RTTR_REGISTRATION\n{\n");
    out.push_str("    using namespace rttr;\n\n");
    out.push_str(&format!(
        "    registration::class_<{}>(\"{}\")\n",
        s.name, s.name
    ));

    for (_ty, name) in &s.fields {
        out.push_str(&format!(
            "        .property(\"{}\", &{}::{})\n",
            name, s.name, name
        ));
    }

    out.push_str("    ;\n}\n");

    out
}
