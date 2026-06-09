include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

include_directories(${CMAKE_SOURCE_DIR}/include)
set(SOURCES_UNDER_TEST
    ${CMAKE_SOURCE_DIR}/src/core/User.cpp
    ${CMAKE_SOURCE_DIR}/src/core/Employee.cpp
    ${CMAKE_SOURCE_DIR}/src/core/Admin.cpp
    ${CMAKE_SOURCE_DIR}/src/core/Product.cpp
    ${CMAKE_SOURCE_DIR}/src/core/Stock.cpp
    ${CMAKE_SOURCE_DIR}/src/documents/Order.cpp
    ${CMAKE_SOURCE_DIR}/src/documents/OrderItem.cpp
    ${CMAKE_SOURCE_DIR}/src/documents/Receipt.cpp
    ${CMAKE_SOURCE_DIR}/src/documents/ReceiptItem.cpp
    ${CMAKE_SOURCE_DIR}/src/documents/Shipment.cpp
    ${CMAKE_SOURCE_DIR}/src/documents/ShipmentItem.cpp
    ${CMAKE_SOURCE_DIR}/src/database/Database.cpp
    ${CMAKE_SOURCE_DIR}/src/facade/WareHouseFacade.cpp
    ${CMAKE_SOURCE_DIR}/src/factory/DocumentFactory.cpp
    ${CMAKE_SOURCE_DIR}/src/factory/EmployeeDocumentFactory.cpp
    ${CMAKE_SOURCE_DIR}/src/builder/OrderBuilder.cpp
    ${CMAKE_SOURCE_DIR}/src/state/OrderState.cpp
    ${CMAKE_SOURCE_DIR}/src/state/CreatedState.cpp
    ${CMAKE_SOURCE_DIR}/src/state/ClosedState.cpp
    ${CMAKE_SOURCE_DIR}/src/state/CancelledState.cpp
    ${CMAKE_SOURCE_DIR}/src/observer/Observer.cpp
    ${CMAKE_SOURCE_DIR}/src/observer/Subject.cpp
)

set(TEST_SOURCES
    main.cpp
    test_user.cpp
    test_employee.cpp
    test_admin.cpp
    test_product.cpp
    test_stock.cpp
    test_order.cpp
    test_orderitem.cpp
    test_receipt.cpp
    test_shipment.cpp
    test_database.cpp
    test_facade.cpp
    scenarios.cpp
)

add_executable(run_tests ${TEST_SOURCES} ${SOURCES_UNDER_TEST})

target_link_libraries(run_tests 
    gtest_main 
    gtest
    Qt6::Core
    Qt6::Widgets
    ${PostgreSQL_LIBRARIES}
)

target_include_directories(run_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${Qt6_INCLUDE_DIRS}
    ${PostgreSQL_INCLUDE_DIRS}
)