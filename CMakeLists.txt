cmake_minimum_required(VERSION 3.16)

project(BusinessManagementSystem
  VERSION 1.0
  LANGUAGES CXX
)

# ---- C++ Standard ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Detect layout (root vs src/include) ----
if (EXISTS "${CMAKE_SOURCE_DIR}/src/main.cpp")
  set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
else()
  set(SRC_DIR "${CMAKE_SOURCE_DIR}")
endif()

if (EXISTS "${CMAKE_SOURCE_DIR}/include")
  set(INC_DIR "${CMAKE_SOURCE_DIR}/include")
else()
  set(INC_DIR "${CMAKE_SOURCE_DIR}")
endif()

# ---- Target ----
add_executable(BusinessManagementSystem
  "${SRC_DIR}/main.cpp"
  "${SRC_DIR}/Product.cpp"
  "${SRC_DIR}/Customer.cpp"
  "${SRC_DIR}/RegularCustomer.cpp"
  "${SRC_DIR}/PremiumCustomer.cpp"
  "${SRC_DIR}/Order.cpp"
  "${SRC_DIR}/Finance.cpp"
)

target_include_directories(BusinessManagementSystem PRIVATE "${INC_DIR}")

# ---- Warnings (nice for school projects) ----
if (MSVC)
  target_compile_options(BusinessManagementSystem PRIVATE /W4)
else()
  target_compile_options(BusinessManagementSystem PRIVATE -Wall -Wextra -Wpedantic)
endif()