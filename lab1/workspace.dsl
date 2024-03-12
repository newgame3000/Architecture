workspace {
    name "Лабораторная работа 1"

    model {
        user = person "Пассажир"
        main_user = person "Водитель"

        search_system = softwareSystem "Сервис поиска попутчиков" {

            auto_system = container "Система авторизации" {
               description "Авторизация и управление пользователями"
            }

            route_system = container "Сервис маршрутов" {
                description "Создание/хранение информации о маршрутах"
            }
            travel_system = container "Сервис поездок" {
                description "Создание/хранение информации о поездках"
            }

            db = container "База данных" {
                description "Хранение данных о пользователях, поездках и маршрутах"

                user_db = component  "База данных пользователей" {
                    tags "database"
                }

                travel_db = component  "База данных поездок"{
                    tags "database"
                }

                route_db = component  "База данных маршрутов"{
                    tags "database"
                }

                tags "database"
            }
        }      

        user -> auto_system "Поиск поездки"
        main_user -> auto_system "Регистрация маршрута"
        auto_system -> route_system "Регистрация маршрута"
        auto_system -> travel_system "Регистрация поездки"
        travel_system -> route_system "Запрос данных о маршруте"
        route_system -> route_db "Запрос/запись данных о маршруте"
        travel_system -> travel_db "Запрос/запись данных о поездке"
        auto_system -> user_db "Запрос/запись данных о пользователе"

        production = deploymentEnvironment "Результат" {
            deploymentNode "Сервер системы авторизации" {
                containerInstance auto_system
            }

            deploymentNode "Сервер системы маршрутов" {
                containerInstance route_system
            }

            deploymentNode "Сервер системы поездок" {
                containerInstance travel_system
            }

            deploymentNode "Базы данных" {
                containerInstance db
                tags "database"
            }
        }
    }

    views {
        themes default

        systemContext search_system "Context" {
            include *
            autoLayout 
        }

        container search_system "Container" {
            include *
            autoLayout
        }

        component db "Compomemt" {
            include *
            autoLayout
        }

        deployment search_system production {
            include *
            autoLayout
        }

        dynamic search_system "auto_user" "Создание нового пользователя" {
            autoLayout
            user -> auto_system "Регистрация (POST)"
            main_user -> auto_system "Регистрация (POST)"
            auto_system -> db "Сохранить данные о пользователе" 
        }

        dynamic search_system "search_login" "Поиск пользователя по логину" {
            autoLayout
            user -> auto_system "Поиск пользователя по логину (GET)"
            main_user -> auto_system "Поиск пользователя по логину (GET)"
            auto_system -> db "Поиск пользователя по логину" 
        }

        dynamic search_system "search_name" "Поиск пользователя по маске имя и фамилии" {
            autoLayout
            user -> auto_system "Поиск пользователя по маске имя и фамилии (GET)"
            main_user -> auto_system "Поиск пользователя по маске имя и фамилии (GET)"
            auto_system -> db "Поиск пользователя по маске имя и фамилии" 
        }

        dynamic search_system "create_route" "Создание маршрута" {
            autoLayout
            main_user -> auto_system "Создание маршрута (POST)"
            auto_system -> db "Поиск пользователя, Сохранение пользователя"
            auto_system -> route_system "Создание маршрута (POST)"
            route_system -> db "Создание маршрута"
        }

        dynamic search_system "search_route" "Получение маршрутов пользователя" {
            autoLayout
            user -> auto_system "Получение маршрутов пользователя (GET)"
            auto_system -> route_system "Получение маршрутов пользователя (GET)"
            route_system -> db "Получение маршрутов пользователя"
        }

        dynamic search_system "create_travel" "Создание поездки" {
            autoLayout
            main_user -> auto_system "Создание поездки (POST)"
            auto_system -> db "Поиск пользователя, Сохранение пользователя"
            auto_system -> travel_system "Создание поездки (POST)"
            travel_system -> route_system "Поиск маршрута (GET)"
            route_system -> db "Поиск маршрута"
            travel_system -> db "Запись поездки"
        }

        dynamic search_system "add_to_travel" "Подключение пользователей к поездке" {
            autoLayout
            user -> auto_system "Подключение пользователей к поездке (POST)"
            auto_system -> db "Поиск пользователя, Сохранение пользователя"
            auto_system -> travel_system "Подключение пользователей к поездке (POST)"
            travel_system -> db "Добавление пользователя к поездке"
        }

         dynamic search_system "info_travel" "Получение информации о поездке" {
            autoLayout
            main_user -> auto_system "Получение информации о поездке (POST)"
            user -> auto_system "Получение информации о поездке (POST)"
            auto_system -> travel_system "Получение информации о поездке (POST)"
            travel_system -> db "Получение информации о поездке"
        }

        styles {
            element "database" {
                shape cylinder
            }
        }
    }
}