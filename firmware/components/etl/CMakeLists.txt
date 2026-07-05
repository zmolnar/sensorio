idf_component_register(
    INCLUDE_DIRS
        "../../lib/etl/include"
        "$ENV{IDF_PATH}/components/freertos/include/freertos"
)

target_compile_definitions(${COMPONENT_LIB} INTERFACE
    ETL_NO_STL
    ETL_TARGET_OS_FREERTOS
)
