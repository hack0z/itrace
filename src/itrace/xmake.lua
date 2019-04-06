
-- add target
target("itrace")

    -- make as a shared library
    set_kind("shared")

    -- add defines
    add_defines("__tb_prefix__=\"itrace\"")

    -- add includes directory
    add_includedirs("$(buildir)")
    add_includedirs("$(buildir)/itrace")

    -- add packages
    add_packages("tbox")

    -- add the common source files
    add_files("*.m")

