add_library(${ProjectName} OBJECT)
target_import_src(${ProjectName})
target_install_obj_dir(${ProjectName})

target_import_base(${ProjectName} PUBLIC)
target_import_bsp_interface(${ProjectName} PUBLIC)
target_import_task(${ProjectName} PUBLIC)
target_import_stm32h743iit6_hal(${ProjectName} PRIVATE)
