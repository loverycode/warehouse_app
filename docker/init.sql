CREATE TABLE IF NOT EXISTS users (
    id SERIAL PRIMARY KEY,
    login VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    name VARCHAR(100) NOT NULL,
    role VARCHAR(20) NOT NULL CHECK (role IN ('admin', 'employee')),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

INSERT INTO users (login, password, name, role) VALUES 
('admin', 'admin', 'Администратор', 'admin'),
('user', 'user', 'Иван Иванов', 'employee')
ON CONFLICT (login) DO NOTHING;

CREATE TABLE IF NOT EXISTS products (
    id SERIAL PRIMARY KEY,
    sku VARCHAR(50) UNIQUE NOT NULL,
    name VARCHAR(200) NOT NULL,
    price DECIMAL(10,2) NOT NULL,
    description TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS stocks (
    product_id INTEGER PRIMARY KEY REFERENCES products(id) ON DELETE CASCADE,
    quantity INTEGER NOT NULL DEFAULT 0,
    min_threshold INTEGER DEFAULT 5,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS orders (
    id SERIAL PRIMARY KEY,
    order_number VARCHAR(50) UNIQUE NOT NULL,
    date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    status VARCHAR(20) DEFAULT 'Создан' CHECK (status IN ('Создан', 'Закрыт', 'Отменён')),
    employee_id INTEGER REFERENCES users(id),
    total DECIMAL(10,2) DEFAULT 0
);

CREATE TABLE IF NOT EXISTS order_items (
    id SERIAL PRIMARY KEY,
    order_id INTEGER REFERENCES orders(id) ON DELETE CASCADE,
    product_id INTEGER REFERENCES products(id),
    quantity INTEGER NOT NULL,
    price DECIMAL(10,2) NOT NULL
);

CREATE TABLE IF NOT EXISTS receipts (
    id SERIAL PRIMARY KEY,
    receipt_number VARCHAR(50) UNIQUE NOT NULL,
    date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    supplier VARCHAR(200) NOT NULL,
    employee_id INTEGER REFERENCES users(id)
);

CREATE TABLE IF NOT EXISTS receipt_items (
    id SERIAL PRIMARY KEY,
    receipt_id INTEGER REFERENCES receipts(id) ON DELETE CASCADE,
    product_id INTEGER REFERENCES products(id),
    quantity INTEGER NOT NULL,
    buy_price DECIMAL(10,2) NOT NULL
);

CREATE TABLE IF NOT EXISTS shipments (
    id SERIAL PRIMARY KEY,
    shipment_number VARCHAR(50) UNIQUE NOT NULL,
    date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    order_id INTEGER REFERENCES orders(id),
    employee_id INTEGER REFERENCES users(id)
);

CREATE TABLE IF NOT EXISTS shipment_items (
    id SERIAL PRIMARY KEY,
    shipment_id INTEGER REFERENCES shipments(id) ON DELETE CASCADE,
    product_id INTEGER REFERENCES products(id),
    quantity INTEGER NOT NULL,
    sell_price DECIMAL(10,2) NOT NULL
);

CREATE TABLE IF NOT EXISTS audit_log (
    id SERIAL PRIMARY KEY,
    user_id INTEGER REFERENCES users(id),
    action VARCHAR(100) NOT NULL,
    details TEXT,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

DO $$
BEGIN
    FOR i IN 1..100 LOOP
        INSERT INTO products (sku, name, price, description) VALUES (
            'SKU-' || LPAD(i::TEXT, 5, '0'),
            CASE (i % 10)
                WHEN 0 THEN 'Смартфон Model ' || i
                WHEN 1 THEN 'Ноутбук Model ' || i
                WHEN 2 THEN 'Планшет Model ' || i
                WHEN 3 THEN 'Наушники Model ' || i
                WHEN 4 THEN 'Клавиатура Model ' || i
                WHEN 5 THEN 'Мышь Model ' || i
                WHEN 6 THEN 'Монитор Model ' || i
                WHEN 7 THEN 'Системный блок Model ' || i
                WHEN 8 THEN 'Принтер Model ' || i
                WHEN 9 THEN 'Сканер Model ' || i
            END,
            ROUND((50 + RANDOM() * 1000)::NUMERIC, 2),
            CASE (i % 5)
                WHEN 0 THEN 'Отличное качество, гарантия 2 года'
                WHEN 1 THEN 'Новинка 2024 года'
                WHEN 2 THEN 'Хит продаж'
                WHEN 3 THEN 'Рекомендуем'
                ELSE 'В наличии'
            END
        );
    END LOOP;
END $$;

INSERT INTO stocks (product_id, quantity, min_threshold)
SELECT id, (10 + RANDOM() * 100)::INT, 5
FROM products
ON CONFLICT (product_id) DO NOTHING;

INSERT INTO orders (order_number, employee_id, status, total) VALUES
('ORD-2024001', 2, 'Создан', 0),
('ORD-2024002', 2, 'Закрыт', 0),
('ORD-2024003', 2, 'Отменён', 0);

INSERT INTO receipts (receipt_number, supplier, employee_id) VALUES
('RCP-2024001', 'ООО "ТехноПоставка"', 2),
('RCP-2024002', 'ИП "Электроника"', 2),
('RCP-2024003', 'ЗАО "Компьютеры"', 2);