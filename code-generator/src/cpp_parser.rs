use clang_sys::*;
use std::ffi::{CStr, CString};
use std::path::Path;

#[derive(Debug)]
pub struct CppStruct {
    pub name: String,
    pub fields: Vec<(String, String)>,
}

fn cursor_spelling(cursor: CXCursor) -> String {
    unsafe {
        CStr::from_ptr(clang_getCString(clang_getCursorSpelling(cursor)))
            .to_string_lossy()
            .into_owned()
    }
}

fn type_spelling(ty: CXType) -> String {
    unsafe {
        CStr::from_ptr(clang_getCString(clang_getTypeSpelling(ty)))
            .to_string_lossy()
            .into_owned()
    }
}

pub fn parse_header(path: &Path) -> Vec<CppStruct> {
    let mut structs: Vec<CppStruct> = Vec::new();

    unsafe {
        // index
        let index = clang_createIndex(0, 0);

        let c_path = CString::new(path.to_str().unwrap()).unwrap();

        let args: Vec<CString> = vec![
            CString::new("-x").unwrap(),
            CString::new("c++").unwrap(),
            CString::new("-std=c++20").unwrap(),
        ];
        let c_args: Vec<*const i8> = args.iter().map(|a| a.as_ptr()).collect();

        let tu = clang_parseTranslationUnit(
            index,
            c_path.as_ptr(),
            c_args.as_ptr(),
            c_args.len() as i32,
            std::ptr::null_mut(),
            0,
            CXTranslationUnit_None,
        );

        if tu.is_null() {
            eprintln!("Failed to parse header: {}", path.display());
            return structs;
        }

        // cursor
        let cursor = clang_getTranslationUnitCursor(tu);

        clang_visitChildren(cursor, visitor, &mut structs as *mut _ as CXClientData);

        clang_disposeTranslationUnit(tu);
        clang_disposeIndex(index);
    }

    structs
}

extern "C" fn visitor(
    cursor: CXCursor,
    _parent: CXCursor,
    data: CXClientData,
) -> CXChildVisitResult {
    unsafe {
        let structs = &mut *(data as *mut Vec<CppStruct>);

        let kind = clang_getCursorKind(cursor);

        if kind == CXCursor_StructDecl || kind == CXCursor_ClassDecl {
            if clang_isCursorDefinition(cursor) == 0 {
                return CXChildVisit_Continue;
            }

            let name = cursor_spelling(cursor);
            if name.is_empty() {
                return CXChildVisit_Continue;
            }

            let mut s = CppStruct {
                name,
                fields: Vec::new(),
            };

            clang_visitChildren(cursor, field_visitor, &mut s as *mut _ as CXClientData);

            structs.push(s);
        }
    }

    CXChildVisit_Continue
}

extern "C" fn field_visitor(
    cursor: CXCursor,
    _parent: CXCursor,
    data: CXClientData,
) -> CXChildVisitResult {
    unsafe {
        if clang_getCursorKind(cursor) == CXCursor_FieldDecl {
            let s = &mut *(data as *mut CppStruct);

            let field_name = cursor_spelling(cursor);
            let field_type = type_spelling(clang_getCursorType(cursor));

            s.fields.push((field_type, field_name));
        }
    }

    CXChildVisit_Continue
}
