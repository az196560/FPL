(* Abstract Syntax Tree and functions for printing it *)

type op = Add | Sub | Mult | Div | Equal | Neq | Less | Leq | Greater | Geq |
          And | Or

type uop = Neg | Not


type typ = Int | Bool | Void | Float | Char | String | Wall | Bed | Desk | Door | Window | Rectangle | Circle

type bind = typ * string

type expr =
    Literal of int
  | FLiteral of float
  | CharLit of char
  | StringLit of string
  | ArrayAccess of string * expr 
  | BoolLit of bool
  | Id of string
  | Binop of expr * op * expr
  | Unop of uop * expr
  | WallConstruct of string * expr list
  | BedConstruct of string * expr list
  | DeskConstruct of string * expr list
  | DoorConstruct of string * expr list
  | WindowConstruct of string * expr list
  | RectangleConstruct of string * expr list
  | CircleConstruct of string * expr list
  | Assign of string * expr
  | Call of string * expr list
  | Noexpr

type stmt =
    Block of stmt list
  | Expr of expr
  | Return of expr
  | If of expr * stmt * stmt
  | For of expr * expr * expr * stmt
  | While of expr * stmt

type func_decl = {
    typ : typ;
    fname : string;
    formals : bind list;
    locals : bind list;
    body : stmt list;
  }

type program = bind list * func_decl list

(* Pretty-printing functions *)

let string_of_op = function
    Add -> "+"
  | Sub -> "-"
  | Mult -> "*"
  | Div -> "/"
  | Equal -> "=="
  | Neq -> "!="
  | Less -> "<"
  | Leq -> "<="
  | Greater -> ">"
  | Geq -> ">="
  | And -> "&&"
  | Or -> "||"

let string_of_uop = function
    Neg -> "-"
  | Not -> "!"

let rec string_of_expr = function
    Literal(l) -> string_of_int l
  | FLiteral(f) -> string_of_float f
  | CharLit(c) -> String.make 1 c
  | StringLit(s) -> s
  | BoolLit(true) -> "true"
  | BoolLit(false) -> "false"
  | ArrayAccess(e1,e2) -> e1 ^ "[" ^ string_of_expr e2^ "]"
  | Id(s) -> s
  | Binop(e1, o, e2) ->
      string_of_expr e1 ^ " " ^ string_of_op o ^ " " ^ string_of_expr e2
  | Unop(o, e) -> string_of_uop o ^ string_of_expr e
  | WallConstruct(n, el) -> n ^ " = wall(" ^ String.concat ", " (List.map string_of_expr el) ^ ")"
  | BedConstruct(n, el) -> n ^ " = bed(" ^ String.concat ", " (List.map string_of_expr el) ^ ")"
  | DeskConstruct(n, el) -> n ^ " = desk(" ^ String.concat ", " (List.map string_of_expr el) ^ ")"
  | DoorConstruct(n, el) -> n ^ " = door(" ^ String.concat ", " (List.map string_of_expr el) ^ ")"
  | WindowConstruct(n, el) -> n ^ " = window(" ^ String.concat ", " (List.map string_of_expr el) ^ ")"
  | RectangleConstruct(n, el) -> n ^ " = rectangle(" ^ String.concat ", " (List.map string_of_expr el) ^ ")"
  | CircleConstruct(n, el) -> n ^ " = circle(" ^ String.concat ", " (List.map string_of_expr el) ^ ")"
  | Assign(v, e) -> v ^ " = " ^ string_of_expr e
  | Call(f, el) ->
      f ^ "(" ^ String.concat ", " (List.map string_of_expr el) ^ ")"
  | Noexpr -> ""

let rec string_of_stmt = function
    Block(stmts) ->
      "{\n" ^ String.concat "" (List.map string_of_stmt stmts) ^ "}\n"
  | Expr(expr) -> string_of_expr expr ^ ";\n";
  | Return(expr) -> "return " ^ string_of_expr expr ^ ";\n";
  | If(e, s, Block([])) -> "if (" ^ string_of_expr e ^ ")\n" ^ string_of_stmt s
  | If(e, s1, s2) ->  "if (" ^ string_of_expr e ^ ")\n" ^
      string_of_stmt s1 ^ "else\n" ^ string_of_stmt s2
  | For(e1, e2, e3, s) ->
      "for (" ^ string_of_expr e1  ^ " ; " ^ string_of_expr e2 ^ " ; " ^
      string_of_expr e3  ^ ") " ^ string_of_stmt s
  | While(e, s) -> "while (" ^ string_of_expr e ^ ") " ^ string_of_stmt s

let string_of_typ = function
    Int -> "int"
  | Float -> "float"
  | Bool -> "bool"
  | Void -> "void"
  | Char -> "char"
  | String -> "string"
  | Wall -> "wall"
  | Bed -> "bed"
  | Desk -> "desk"
  | Door -> "door"
  | Window -> "window"
  | Rectangle -> "rectangle"
  | Circle -> "circle"

let string_of_vdecl (t, id) = string_of_typ t ^ " " ^ id ^ ";\n"

let string_of_fdecl fdecl =
  string_of_typ fdecl.typ ^ " " ^
  fdecl.fname ^ "(" ^ String.concat ", " (List.map snd fdecl.formals) ^
  ")\n{\n" ^
  String.concat "" (List.map string_of_vdecl fdecl.locals) ^
  String.concat "" (List.map string_of_stmt fdecl.body) ^
  "}\n"

let string_of_program (vars, funcs) =
  String.concat "" (List.map string_of_vdecl vars) ^ "\n" ^
  String.concat "\n" (List.map string_of_fdecl funcs)
