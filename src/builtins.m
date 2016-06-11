%-----------------------------------------------------------------------%
% vim: ts=4 sw=4 et
%-----------------------------------------------------------------------%
:- module builtins.
%
% Copyright (C) 2015-2016 Plasma Team
% Distributed under the terms of the MIT License see ../LICENSE.code
%
% Plasma builtins
%
%-----------------------------------------------------------------------%

:- interface.

:- import_module core.

:- pred setup_builtins(core::in, core::out) is det.

%-----------------------------------------------------------------------%
%-----------------------------------------------------------------------%

:- implementation.

:- import_module list.
:- import_module require.
:- import_module set.
:- import_module string.

:- import_module context.
:- import_module common_types.
:- import_module core.types.
:- import_module q_name.

%-----------------------------------------------------------------------%

setup_builtins(!Core) :-
    foldl(register_builtin, builtins, !Core).

%-----------------------------------------------------------------------%

:- type builtin
    --->    builtin(
                b_name          :: string,
                b_function      :: function
            ).

:- func builtins = list(builtin).

builtins = [
        builtin("print",
            func_init(nil_context, s_private, [builtin_type(string)],
                [], set([r_io]), init)),
        builtin("int_to_string",
            func_init(nil_context, s_private, [builtin_type(int)],
                [builtin_type(string)], init, init)),
        builtin("free",
            func_init(nil_context, s_private, [builtin_type(string)],
                [], set([r_io]), init))
    ].

:- pred register_builtin(builtin::in, core::in, core::out) is det.

register_builtin(Builtin, !Core) :-
    Builtin = builtin(Name, Func),
    ( if
        core_register_function(q_name_append(q_name("builtin"), Name),
            FuncId, !Core)
    then
        core_set_function(FuncId, Func, !Core)
    else
        unexpected($file, $pred, "Duplicate builtin")
    ).

