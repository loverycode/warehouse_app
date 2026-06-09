# Система управления складом

Приложение для автоматизации складского учёта: приход, расход, заказы, остатки, отчёты.  

## Основные функции

- Авторизация с ролями: **сотрудник склада** и **администратор**
- Управление товарами (CRUD)
- Оформление прихода/расхода с автоматическим обновлением остатков
- Создание и отгрузка заказов
- Отчёты по остаткам и движению товаров (экспорт JSON/PDF)
- Unit-тесты (GoogleTest) – 56 тестов
- Контейнеризация (Docker)

## Данные для входа

| Роль                | Логин   | Пароль |
 Сотрудник склада      `user`     `user` 
  Администратор       `admin`    `admin` 

## Сборка и запуск (локально)

```bash
# Клонирование и переход в проект
git clone <repo-url>
cd WareHouseProject

# Создание базы данных
createdb warehouse_db
psql -d warehouse_db -f docker/init.sql

# Сборка
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH=$(brew --prefix qt6) ..
make -j$(nproc)

# Запуск
./warehouse_app
```

## Запуск тестов

```bash
cd build/tests
./run_tests
```

## Docker

### Сборка образов

```bash
# Только тесты
docker build --target test -t warehouse-tests .

# Полный образ (с GUI)
docker build --target runtime -t warehouse-app .
```

### Запуск тестов в контейнере

```bash
docker run --rm warehouse-tests
```

### Запуск приложения с GUI (через X11)

**macOS (требуется XQuartz):**
```bash
xhost +localhost
docker run --rm -e DISPLAY=host.docker.internal:0 -v /tmp/.X11-unix:/tmp/.X11-unix warehouse-app
```

### Запуск через docker-compose (с PostgreSQL)

```bash
docker-compose up -d postgres   # только БД
docker-compose up app           # БД + приложение
docker-compose run --rm tests   # тесты
```
