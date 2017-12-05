(* Code generation: translate takes a semantically checked AST and
produces LLVM IR

LLVM tutorial: Make sure to read the OCaml version of the tutorial

http://llvm.org/docs/tutorial/index.html

Detailed documentation on the OCaml LLVM library:

http://llvm.moe/
http://llvm.moe/ocaml/

*)

open Ast
module L = Llvm
module A = Ast

module StringMap = Map.Make(String)

(* Error message *)
exception FPL_err of string;;

(* global variable *)
let fplObjectValueMap = ref StringMap.empty;;
let structMemberMap = ref StringMap.empty;;

let translate program =
  let context = L.global_context () in
  let the_module = L.create_module context "Fpl"
  and i32_t  = L.i32_type  context
  and i8_t   = L.i8_type   context
  and i1_t   = L.i1_type   context
  and flt_t  = L.float_type context
  and str_t  = L.pointer_type (L.i8_type context)
  and void_t = L.void_type context
  and fplObject_t = L.i16_type context
  and struct_t n = L.i16_type context in

  let ltype_of_typ = function
      A.Int -> i32_t
    | A.Bool -> i1_t
    | A.Float -> flt_t
    | A.Char -> i8_t
    | A.String -> str_t
    | A.Void -> void_t
    | A.Wall -> fplObject_t
    | A.Bed -> fplObject_t
    | A.Desk -> fplObject_t
    | A.Door -> fplObject_t
    | A.Window -> fplObject_t
    | A.Rectangle -> fplObject_t
    | A.Circle -> fplObject_t 
    | A.Struct n -> struct_t n (* unitialized struct type *)
  in

  (* debug helper *)
  let rec getMap map = function
    [] -> map
    | pair::pairs -> getMap (StringMap.add (snd pair) (fst pair) map) pairs in
 
  let printList l =
      List.iter (fun n -> Printf.printf "%s, " (A.string_of_expr n)) l;  Printf.printf "\n" in

  let printObjectValueMap m =
     StringMap.iter (fun key value -> Printf.printf "%s: " key;  printList value) m in
  
  let printStringList l =
      List.iter (fun n -> Printf.printf "%s, " n) l;  Printf.printf "\n" in
    
  let printStructMemberMap m =
     StringMap.iter (fun key value -> Printf.printf "%s: " key;  printStringList value) m in

    (* Declare ensureInt and ensureFloat function *)
  let ensureInt c = 
      if L.type_of c = flt_t then (L.const_fptosi c i32_t) else c in
    
  let ensureFloat c =
      if L.type_of c = flt_t then c else (L.const_sitofp c flt_t) in


  (* Declare each global variable; remember its value in a map *)
  let global_vars =
    let global_var m (t, n) =
      let init = L.const_int (ltype_of_typ t) 0
      in StringMap.add n (L.define_global n init the_module) m in
    List.fold_left global_var StringMap.empty program.globals in

  (* Declare printf(), which the print built-in function will call *)
  let printf_t = L.var_arg_function_type i32_t [| L.pointer_type i8_t |] in
  let printf_func = L.declare_function "printf" printf_t the_module in

  (* Declare the built-in printbig() function *)
  let printbig_t = L.function_type i32_t [| i32_t |] in
  let printbig_func = L.declare_function "printbig" printbig_t the_module in

  (* Declare the built-in drawLine() function *)
  let drawLine_t = L.function_type i32_t [| i32_t |] in
  let drawLine_func = L.declare_function "drawLine" drawLine_t the_module in
  
  (* Declare the built-in drawRec() function *)
  let drawRec_t = L.function_type i32_t [| i32_t |] in
  let drawRec_func = L.declare_function "drawRec" drawRec_t the_module in
  
  (* Declare the built-in put() function *)
  let put_t = L.function_type i32_t [|i32_t; i32_t; flt_t; flt_t; flt_t; flt_t; flt_t|] in
  let put_func = L.declare_function "put" put_t the_module in
  
  (* Declare the built-in render() function *)
  let render_t = L.function_type i32_t [||] in
  let render_func = L.declare_function "render" render_t the_module in
  
  (* Define each function (arguments and return type) so we can call it *)
  let function_decls =
    let function_decl m fdecl =
      let name = fdecl.A.fname
      and formal_types =
	Array.of_list (List.map (fun (t,_) -> ltype_of_typ t) fdecl.A.formals)
      in let ftype = L.function_type (ltype_of_typ fdecl.A.typ) formal_types in
      StringMap.add name (L.define_function name ftype the_module, fdecl) m in
    List.fold_left function_decl StringMap.empty program.functions in
  
  (* Fill in the body of the given function *)
  let build_function_body fdecl =
    let (the_function, _) = StringMap.find fdecl.A.fname function_decls in
    let builder = L.builder_at_end context (L.entry_block the_function) in

    let int_format_str = L.build_global_stringptr "%d\n" "fmt" builder in
    let char_format_str = L.build_global_stringptr "%c\n" "fmt" builder in
    let float_format_str = L.build_global_stringptr "%f\n" "fmt" builder in
    let str_format_str = L.build_global_stringptr "%s\n" "fmt" builder in

    (* Construct the function's "locals": formal arguments and locally
       declared variables.  Allocate each on the stack, initialize their
       value, if appropriate, and remember their values in the "locals" map *)
    let local_vars =
      let add_formal m (t, n) p = L.set_value_name n p;
	let local = L.build_alloca (ltype_of_typ t) n builder in
	ignore (L.build_store p local builder);
	StringMap.add n local m in
    

    let add_local m (t, n) =
        let local_var = L.build_alloca (ltype_of_typ t) n builder;
    in StringMap.add n local_var m in

      let formals = List.fold_left2 add_formal StringMap.empty fdecl.A.formals
          (Array.to_list (L.params the_function)) in
      List.fold_left add_local formals fdecl.A.locals in

    (* Return the value for a variable or formal argument *)
    let lookup n = try StringMap.find n local_vars
                   with Not_found -> StringMap.find n global_vars
    in
    List.iter (fun pair -> if not(StringMap.mem (snd pair) !structMemberMap) then 
        (structMemberMap := StringMap.add (snd pair) ["Dummy"] !structMemberMap)) fdecl.A.locals;
    (*printStructMemberMap !structMemberMap;*)

    (* Construct code for an expression; return its value *)
    let rec expr builder = function
	A.Literal i -> L.const_int i32_t i
      | A.BoolLit b -> L.const_int i1_t (if b then 1 else 0)
      | A.FLiteral f -> L.const_float flt_t f
      | A.CharLit c -> L.const_int i8_t (Char.code c)
      | A.StringLit s -> L.build_global_stringptr (s^"\x00") "strptr" builder
      | A.Noexpr -> L.const_int i32_t 0
      | A.Id s -> L.build_load (lookup s) s builder
      | A.WallStructConstruct (structName, memberName, act) ->
              let fullName = structName ^ "." ^ memberName in
              let members = StringMap.find structName !structMemberMap in
              let membersWithotDummy = List.filter (fun member -> (compare member "Dummy") != 0) members in
              let newMembers = membersWithotDummy @ [fullName] in
              structMemberMap := StringMap.add structName newMembers !structMemberMap;
              (*printStructMemberMap !structMemberMap;*)
              (* append type wall:0 and degree:0 *)
              fplObjectValueMap := StringMap.add fullName ([A.Literal(0); A.Literal(0)] @ act @ [A.FLiteral(0.0)]) !fplObjectValueMap;
              (*printObjectValueMap !fplObjectValueMap;*)
              L.const_int i32_t 0
      | A.BedStructConstruct (structName, memberName, act) ->
              let fullName = structName ^ "." ^ memberName in
              let members = StringMap.find structName !structMemberMap in
              let membersWithotDummy = List.filter (fun member -> (compare member "Dummy") != 0) members in
              let newMembers = membersWithotDummy @ [fullName] in
              structMemberMap := StringMap.add structName newMembers !structMemberMap;
              (* append type bed:1 and degree:0 *)
              fplObjectValueMap := StringMap.add fullName ([A.Literal(0); A.Literal(1)] @ act @ [A.FLiteral(0.0)]) !fplObjectValueMap;
              L.const_int i32_t 0
      | A.DeskStructConstruct (structName, memberName, act) ->
              let fullName = structName ^ "." ^ memberName in
              let members = StringMap.find structName !structMemberMap in
              let membersWithotDummy = List.filter (fun member -> (compare member "Dummy") != 0) members in
              let newMembers = membersWithotDummy @ [fullName] in
              structMemberMap := StringMap.add structName newMembers !structMemberMap;
              (* append type desk:2 and degree:0 *)
              fplObjectValueMap := StringMap.add fullName ([A.Literal(0); A.Literal(2)] @ act @ [A.FLiteral(0.0)]) !fplObjectValueMap;
              L.const_int i32_t 0
      | A.DoorStructConstruct (structName, memberName, act) ->
              let fullName = structName ^ "." ^ memberName in
              let members = StringMap.find structName !structMemberMap in
              let membersWithotDummy = List.filter (fun member -> (compare member "Dummy") != 0) members in
              let newMembers = membersWithotDummy @ [fullName] in
              structMemberMap := StringMap.add structName newMembers !structMemberMap;
              (* append type door:3 and degree:0 *)
              fplObjectValueMap := StringMap.add fullName ([A.Literal(0); A.Literal(3)] @ act @ [A.FLiteral(0.0)]) !fplObjectValueMap;
              L.const_int i32_t 0
      | A.WindowStructConstruct (structName, memberName, act) ->
              let fullName = structName ^ "." ^ memberName in
              let members = StringMap.find structName !structMemberMap in
              let membersWithotDummy = List.filter (fun member -> (compare member "Dummy") != 0) members in
              let newMembers = membersWithotDummy @ [fullName] in
              structMemberMap := StringMap.add structName newMembers !structMemberMap;
              (* append type window:4 and degree:0 *)
              fplObjectValueMap := StringMap.add fullName ([A.Literal(0); A.Literal(4)] @ act @ [A.FLiteral(0.0)]) !fplObjectValueMap;
              L.const_int i32_t 0
      | A.RectangleStructConstruct (structName, memberName, act) ->
              let fullName = structName ^ "." ^ memberName in
              let members = StringMap.find structName !structMemberMap in
              let membersWithotDummy = List.filter (fun member -> (compare member "Dummy") != 0) members in
              let newMembers = membersWithotDummy @ [fullName] in
              structMemberMap := StringMap.add structName newMembers !structMemberMap;
              (* append type rectangle:5 and degree:0 *)
              fplObjectValueMap := StringMap.add fullName ([A.Literal(0); A.Literal(5)] @ act @ [A.FLiteral(0.0)]) !fplObjectValueMap;
              L.const_int i32_t 0
      | A.CircleStructConstruct (structName, memberName, act) ->
              let fullName = structName ^ "." ^ memberName in
              let members = StringMap.find structName !structMemberMap in
              let membersWithotDummy = List.filter (fun member -> (compare member "Dummy") != 0) members in
              let newMembers = membersWithotDummy @ [fullName] in
              structMemberMap := StringMap.add structName newMembers !structMemberMap;
              (* append type circle:6 and degree:0 *)
              fplObjectValueMap := StringMap.add fullName ([A.Literal(0); A.Literal(6)] @ act) !fplObjectValueMap;
              L.const_int i32_t 0
      | A.WallConstruct (variableName, act) ->
              let member = StringMap.find variableName !structMemberMap in
              let memberWithotDummy = List.filter (fun member -> (compare member "Dummy") != 0) member in
              let newMember = memberWithotDummy @ [variableName] in
              structMemberMap := StringMap.add variableName newMember !structMemberMap;
              (* append type wall:0 and degree:0 *)
              fplObjectValueMap := StringMap.add variableName ([A.Literal(0); A.Literal(0)] @ act @ [A.FLiteral(0.0)]) !fplObjectValueMap;
              (*printObjectValueMap !fplObjectValueMap;*)
              L.const_int i32_t 0
      | A.BedConstruct (variableName, act) ->
              let member = StringMap.find variableName !structMemberMap in
              let memberWithotDummy = List.filter (fun member -> (compare member "Dummy") != 0) member in
              let newMember = memberWithotDummy @ [variableName] in
              structMemberMap := StringMap.add variableName newMember !structMemberMap;
              (* append type bed:1 and degree:0 *)
              fplObjectValueMap := StringMap.add variableName ([A.Literal(0); A.Literal(1)] @ act @ [A.FLiteral(0.0)]) !fplObjectValueMap;
              L.const_int i32_t 0
      | A.DeskConstruct (variableName, act) ->
              let member = StringMap.find variableName !structMemberMap in
              let memberWithotDummy = List.filter (fun member -> (compare member "Dummy") != 0) member in
              let newMember = memberWithotDummy @ [variableName] in
              structMemberMap := StringMap.add variableName newMember !structMemberMap;
              (* append type desk:2 and degree:0 *)
              fplObjectValueMap := StringMap.add variableName ([A.Literal(0); A.Literal(2)] @ act @ [A.FLiteral(0.0)]) !fplObjectValueMap;
              L.const_int i32_t 0
      | A.DoorConstruct (variableName, act) ->
              let member = StringMap.find variableName !structMemberMap in
              let memberWithotDummy = List.filter (fun member -> (compare member "Dummy") != 0) member in
              let newMember = memberWithotDummy @ [variableName] in
              structMemberMap := StringMap.add variableName newMember !structMemberMap;
              (* append type door:3 and degree:0 *)
              fplObjectValueMap := StringMap.add variableName ([A.Literal(0); A.Literal(3)] @ act @ [A.FLiteral(0.0)]) !fplObjectValueMap;
              L.const_int i32_t 0
      | A.WindowConstruct (variableName, act) ->
              let member = StringMap.find variableName !structMemberMap in
              let memberWithotDummy = List.filter (fun member -> (compare member "Dummy") != 0) member in
              let newMember = memberWithotDummy @ [variableName] in
              structMemberMap := StringMap.add variableName newMember !structMemberMap;
              (* append type window:4 and degree:0 *)
              fplObjectValueMap := StringMap.add variableName ([A.Literal(0); A.Literal(4)] @ act @ [A.FLiteral(0.0)]) !fplObjectValueMap;
              L.const_int i32_t 0
      | A.RectangleConstruct (variableName, act) ->
              let member = StringMap.find variableName !structMemberMap in
              let memberWithotDummy = List.filter (fun member -> (compare member "Dummy") != 0) member in
              let newMember = memberWithotDummy @ [variableName] in
              structMemberMap := StringMap.add variableName newMember !structMemberMap;
              (* append type rectangle:5 and degree:0 *)
              fplObjectValueMap := StringMap.add variableName ([A.Literal(0); A.Literal(5)] @ act @ [A.FLiteral(0.0)]) !fplObjectValueMap;
              L.const_int i32_t 0
      | A.CircleConstruct (variableName, act) ->
              let member = StringMap.find variableName !structMemberMap in
              let memberWithotDummy = List.filter (fun member -> (compare member "Dummy") != 0) member in
              let newMember = memberWithotDummy @ [variableName] in
              structMemberMap := StringMap.add variableName newMember !structMemberMap;
              (* append type circle:6 and degree:0 *)
              fplObjectValueMap := StringMap.add variableName ([A.Literal(0); A.Literal(6)] @ act) !fplObjectValueMap;
              L.const_int i32_t 0
      | A.Binop (e1, op, e2) ->
	  let e1' = expr builder e1
    and e2' = expr builder e2 in

    (* Check whether e1 and e2 are float or not.
       If one of them is float, do float operation.
       If not, do int operation *)
    if (L.type_of e1' = flt_t || L.type_of e2' = flt_t) then
    (match op with
      A.Add     -> L.build_fadd
    | A.Sub     -> L.build_fsub
    | A.Mult    -> L.build_fmul
    | A.Div     -> L.build_fdiv
    | A.Equal   -> L.build_fcmp L.Fcmp.Oeq
    | A.Neq     -> L.build_fcmp L.Fcmp.One
    | A.Less    -> L.build_fcmp L.Fcmp.Olt
    | A.Leq     -> L.build_fcmp L.Fcmp.Ole
    | A.Greater -> L.build_fcmp L.Fcmp.Ogt
    | A.Geq     -> L.build_fcmp L.Fcmp.Oge
    | _         -> raise (FPL_err "Invalid operands for operator")
    ) (ensureFloat e1') (ensureFloat e2') "tmp" builder
    else
	  (match op with
	    A.Add     -> L.build_add
	  | A.Sub     -> L.build_sub
	  | A.Mult    -> L.build_mul
      | A.Div     -> L.build_sdiv
	  | A.And     -> L.build_and
	  | A.Or      -> L.build_or
	  | A.Equal   -> L.build_icmp L.Icmp.Eq
	  | A.Neq     -> L.build_icmp L.Icmp.Ne
	  | A.Less    -> L.build_icmp L.Icmp.Slt
	  | A.Leq     -> L.build_icmp L.Icmp.Sle
	  | A.Greater -> L.build_icmp L.Icmp.Sgt
	  | A.Geq     -> L.build_icmp L.Icmp.Sge
    ) (ensureInt e1') (ensureInt e2') "tmp" builder
      | A.Unop(op, e) ->
      let e' = expr builder e in
      (match op with
	      A.Neg     -> L.build_neg
      | A.Not     -> L.build_not) e' "tmp" builder
      | A.Assign (s, e) -> let e' = expr builder e in
	                   ignore (L.build_store e' (lookup s) builder); e'
      | A.Call ("print", [e]) | A.Call ("printb", [e]) ->
	  L.build_call printf_func [| int_format_str ; (expr builder e) |] "printf" builder
      | A.Call ("printChar", [e])->
    L.build_call printf_func [| char_format_str ; (expr builder e) |] "printf" builder
      | A.Call ("printS", [e])->
    L.build_call printf_func [| str_format_str ; (expr builder e) |] "printf" builder
      | A.Call ("printFloat", [e])->
    L.build_call printf_func [| float_format_str ; (expr builder e) |] "printf" builder
      | A.Call ("putc", [e])->
	  L.build_call printf_func [| char_format_str ; (expr builder e) |] "printf" builder
      | A.Call ("drawRec", [e]) ->
	  L.build_call drawRec_func [| (expr builder e) |] "drawRec" builder
      | A.Call ("drawLine", [e]) ->
	  L.build_call drawLine_func [| (expr builder e) |] "drawLine" builder
      | A.Call ("printbig", [e]) ->
	  L.build_call printbig_func [| (expr builder e) |] "printbig" builder
      | A.Call ("put", act) ->
     let fplStruct = A.string_of_expr (List.hd act) in 
     let structMembers = StringMap.find fplStruct !structMemberMap in
     let invokePut offset structMember = 
       let attributes = StringMap.find structMember !fplObjectValueMap in
	   let parameters = List.map (expr builder) (attributes @ offset) in
       (*printList attributes;*)
	   ignore (L.build_call put_func (Array.of_list parameters) "put" builder) in
     let offset = List.tl act in
        List.iter (fun structMember -> invokePut offset structMember) structMembers;
        L.const_int i32_t 0
      | A.Call ("rotate", act) ->
     let fplStruct = A.string_of_expr (List.hd act) in 
     let structMembers = StringMap.find fplStruct !structMemberMap in
     let rotate degree structMember = 
       let attributes = StringMap.find structMember !fplObjectValueMap in
       let newAttributes = [degree] @ (List.tl attributes) in
       (*printList attributes;*)
       fplObjectValueMap := StringMap.remove structMember !fplObjectValueMap;
       fplObjectValueMap := StringMap.add structMember newAttributes !fplObjectValueMap in
     let degree = List.nth act 1 in 
        List.iter (fun structMember -> rotate degree structMember) structMembers;
        L.const_int i32_t 0
      | A.Call ("render", act) ->
	 let parameters = List.map (expr builder) (act) in
         L.build_call render_func (Array.of_list parameters) "render" builder
      | A.Call (f, act) ->
         let (fdef, fdecl) = StringMap.find f function_decls in
	 let actuals = List.rev (List.map (expr builder) (List.rev act)) in
	 let result = (match fdecl.A.typ with A.Void -> ""
                                            | _ -> f ^ "_result") in
         L.build_call fdef (Array.of_list actuals) result builder
    in

    (* Invoke "f builder" if the current block doesn't already
       have a terminal (e.g., a branch). *)
    let add_terminal builder f =
      match L.block_terminator (L.insertion_block builder) with
	Some _ -> ()
      | None -> ignore (f builder) in
	
    (* Build the code for the given statement; return the builder for
       the statement's successor *)
    let rec stmt builder = function
	A.Block sl -> List.fold_left stmt builder sl
      | A.Expr e -> ignore (expr builder e); builder
      | A.Return e -> ignore (match fdecl.A.typ with
	  A.Void -> L.build_ret_void builder
	| _ -> L.build_ret (expr builder e) builder); builder
      | A.If (predicate, then_stmt, else_stmt) ->
         let bool_val = expr builder predicate in
	 let merge_bb = L.append_block context "merge" the_function in

	 let then_bb = L.append_block context "then" the_function in
	 add_terminal (stmt (L.builder_at_end context then_bb) then_stmt)
	   (L.build_br merge_bb);

	 let else_bb = L.append_block context "else" the_function in
	 add_terminal (stmt (L.builder_at_end context else_bb) else_stmt)
	   (L.build_br merge_bb);

	 ignore (L.build_cond_br bool_val then_bb else_bb builder);
	 L.builder_at_end context merge_bb

      | A.While (predicate, body) ->
	  let pred_bb = L.append_block context "while" the_function in
	  ignore (L.build_br pred_bb builder);

	  let body_bb = L.append_block context "while_body" the_function in
	  add_terminal (stmt (L.builder_at_end context body_bb) body)
	    (L.build_br pred_bb);

	  let pred_builder = L.builder_at_end context pred_bb in
	  let bool_val = expr pred_builder predicate in

	  let merge_bb = L.append_block context "merge" the_function in
	  ignore (L.build_cond_br bool_val body_bb merge_bb pred_builder);
	  L.builder_at_end context merge_bb

      | A.For (e1, e2, e3, body) -> stmt builder
	    ( A.Block [A.Expr e1 ; A.While (e2, A.Block [body ; A.Expr e3]) ] )
    in

    (* Build the code for each statement in the function *)
    let builder = stmt builder (A.Block fdecl.A.body) in

    (* Add a return if the last block falls off the end *)
    add_terminal builder (match fdecl.A.typ with
        A.Void -> L.build_ret_void
      | t -> L.build_ret (L.const_int (ltype_of_typ t) 0))
  in

  List.iter build_function_body program.functions;
  the_module
