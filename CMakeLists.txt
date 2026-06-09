cmake_minimum_required(VERSION 3.16)
project(WarehouseSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC OFF)

set(CMAKE_PREFIX_PATH /Users/arinazyranova/Qt/6.9.0/macos)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets PrintSupport)   

find_package(PostgreSQL REQUIRED)

set(SOURCES
    src/core/User.cpp
    src/core/Employee.cpp
    src/core/Admin.cpp
    src/core/Product.cpp
    src/core/Stock.cpp
    src/documents/Order.cpp
    src/documents/OrderItem.cpp
    src/documents/Receipt.cpp
    src/documents/ReceiptItem.cpp
    src/documents/Shipment.cpp
    src/documents/ShipmentItem.cpp
    src/database/Database.cpp
    src/facade/WareHouseFacade.cpp
    src/factory/DocumentFactory.cpp
    src/factory/EmployeeDocumentFactory.cpp
    src/builder/OrderBuilder.cpp
    src/state/OrderState.cpp
    src/state/CreatedState.cpp
    src/state/ClosedState.cpp
    src/state/CancelledState.cpp
    src/observer/Observer.cpp
    src/observer/Subject.cpp
    src/ui/main.cpp
    src/ui/MainWindow.cpp
    src/ui/LoginWidget.cpp
    src/ui/EmployeeWidget.cpp
    src/ui/AdminWidget.cpp
    src/ui/StockWidget.cpp
    src/ui/OrderWidget.cpp
    src/ui/ReceiptWidget.cpp
    src/ui/ProductWidget.cpp
    src/ui/ReportWidget.cpp
)

add_executable(warehouse_app ${SOURCES})

target_include_directories(warehouse_app PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${Qt6_INCLUDE_DIRS}
    ${PostgreSQL_INCLUDE_DIRS}
)

target_link_libraries(warehouse_app 
    Qt6::Core 
    Qt6::Widgets
    Qt6::PrintSupport      
    ${PostgreSQL_LIBRARIES}
)

add_subdirectory(tests)