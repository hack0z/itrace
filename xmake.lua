
-- version
set_version("1.0.3")
set_xmakever("2.1.6")

-- set warning all as error
set_warnings("all", "error")

-- set language: c99, c++11
set_languages("c99", "cxx11")

-- disable some compiler errors
add_cxflags("-Wno-error=deprecated-declarations")
add_mxflags("-Wno-error=deprecated-declarations")
add_cxflags("-Wno-error=nullability-completeness")
add_mxflags("-Wno-error=nullability-completeness")

-- the debug mode
if is_arch("debug") then
    
    -- enable the debug symbols
    set_symbols("debug")

    -- disable optimization
    set_optimize("none")

    -- add defines for debug
    add_defines("__tb_debug__")

    -- attempt to enable some checkers for pc
    if is_arch("i386", "x86_64") then
        add_cxflags("-fsanitize=address", "-ftrapv")
        add_mxflags("-fsanitize=address", "-ftrapv")
        add_ldflags("-fsanitize=address")
    end

-- the release is_arch
elseif is_arch("release") then

    -- set the symbols visibility: hidden
    set_symbols("hidden")

    -- strip all symbols
    set_strip("all")

    -- fomit the frame pointer
    add_cxflags("-fomit-frame-pointer")
    add_mxflags("-fomit-frame-pointer")

    -- for pc
    if is_arch("i386", "x86_64") then
 
        -- enable fastest optimization
        set_optimize("fastest")

    -- for embed
    else
        -- enable smallest optimization
        set_optimize("smallest")
    end
end

-- for embed
if not is_arch("i386", "x86_64") then

    -- add defines for small
    add_defines("__tb_small__")

end

-- add option: demo
option("demo")
    set_default(true)
    set_showmenu(true)
    set_category("option")
    set_description("Enable or disable the demo module")

-- add packages
add_packagedirs("pkg") 

-- add projects
add_subdirs("src/itrace", "src/demo") 
