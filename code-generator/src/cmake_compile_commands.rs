use regex::Regex;

pub struct Element {
    command: String,
    file: String,
}

pub fn get_include_args_of_parser(input_json: &str) -> anyhow::Result<Vec<String>> {
    get_main_include_args(parse(input_json)?)
}

fn parse(input: &str) -> anyhow::Result<Vec<Element>> {
    let json = json::parse(input)?;
    let elements = json
        .members()
        .map(|it| Element {
            command: it["command"].as_str().unwrap().to_string(),
            file: it["file"].as_str().unwrap().to_string(),
        })
        .collect::<Vec<_>>();

    Ok(elements)
}

fn get_main_include_args(elements: Vec<Element>) -> anyhow::Result<Vec<String>> {
    let re = Regex::new(r" -I(\S+)").unwrap();
    if let Some(main_ele) = elements.iter().find(|it| it.file.ends_with("main.cpp")) {
        let include_paths: Vec<String> = re
            .captures_iter(&main_ele.command)
            .filter_map(|cap| cap.get(1).map(|m| m.as_str().to_string()))
            .collect();
        Ok((include_paths))
    } else {
        Err(anyhow::anyhow!(
            "Not find main.cpp's command in compile_commands.json!"
        ))
    }
}
