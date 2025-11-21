use clang_sys::*;
use std::ffi::{CStr, CString};
use std::path::Path;

#[derive(Debug)]
pub struct CppField {
    pub name: String,
    pub ty: String,
    pub ty_name_space: String,
}

#[derive(Debug)]
pub struct CppStruct {
    pub name: String,
    pub name_space: String,
    pub fields: Vec<CppField>,
}

impl CppField {
    pub fn get_ty_with_namespace(&self) -> String {
        format!("{}::{}", &self.ty_name_space, &self.ty)
    }
}

impl CppStruct {
    pub fn get_name_with_namespace(&self) -> String {
        format!("{}::{}", &self.name_space, &self.name)
    }
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

fn get_type_namespace(ty: CXType) -> String {
    unsafe {
        // 获取类型的声明 Cursor
        let decl_cursor = clang_getTypeDeclaration(ty);

        if clang_Cursor_isNull(decl_cursor) != 0 {
            return "".to_string();
        }

        get_namespace(decl_cursor)
    }
}

fn get_namespace(cursor: CXCursor) -> String {
    let mut names: Vec<String> = Vec::new();
    let mut cur = cursor;

    unsafe {
        loop {
            let parent = clang_getCursorSemanticParent(cur);
            let kind = clang_getCursorKind(parent);

            if kind == CXCursor_Namespace {
                let ns = cursor_spelling(parent);
                if !ns.is_empty() {
                    names.push(ns);
                }
            }

            // 到顶层（TU）就停止
            if kind == CXCursor_TranslationUnit {
                break;
            }

            cur = parent;
        }
    }

    names.reverse();
    names.join("::")
}

pub fn parse_header(path: &Path, args: &Vec<CString>) -> Vec<CppStruct> {
    let mut structs: Vec<CppStruct> = Vec::new();

    unsafe {
        // index
        let index = clang_createIndex(0, 0);

        let c_path = CString::new(path.to_str().unwrap()).unwrap();

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

            let name_space = get_namespace(cursor);

            let mut s = CppStruct {
                name,
                name_space,
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
            let field_ty = clang_getCursorType(cursor);
            let field_type = type_spelling(field_ty);
            let field_namespace = get_type_namespace(field_ty);

            s.fields.push(CppField {
                name: field_name,
                ty: field_type,
                ty_name_space: field_namespace,
            });
        }
    }

    CXChildVisit_Continue
}
