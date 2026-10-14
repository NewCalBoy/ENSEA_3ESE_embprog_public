cmake_minimum_required(VERSION 3.16)
project(td6_sdl C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)

# app/ n'est pas duplique : ce sont litteralement les memes fichiers que
# ceux compiles pour la cible STM32 dans ../td6_complete (une seule source
# de verite), juste recompiles ici avec la BSP simulateur.
set(TD6_APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../td6_complete/app)

add_executable(td6_sdl
    main.c
    bsp/bsp.c
    bsp/led.c
    bsp/buttons.c
    bsp/encoder.c
    bsp/neopixel.c
    bsp/display.c
    ${TD6_APP_DIR}/app.c
    ${TD6_APP_DIR}/events.c
    ${TD6_APP_DIR}/menu.c
    ${TD6_APP_DIR}/timers.c
    ${TD6_APP_DIR}/time_bomb.c
    ${TD6_APP_DIR}/menu_entry/menu_entry.c
    ${TD6_APP_DIR}/menu_entry/led_toggle_entry.c
    ${TD6_APP_DIR}/menu_entry/screen_switch_entry.c
    ${TD6_APP_DIR}/menu_entry/hsv_component_entry.c
    ${TD6_APP_DIR}/menu_entry/time_bomb_launch_entry.c
)

target_include_directories(td6_sdl PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/bsp
    ${TD6_APP_DIR}
    ${TD6_APP_DIR}/menu_entry
)
target_link_libraries(td6_sdl PRIVATE SDL3::SDL3)
