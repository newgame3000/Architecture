import pandas as pd
from sqlalchemy import text
from sqlalchemy import create_engine
import sqlalchemy
import os;

engine = create_engine(f"postgresql://{os.environ['DB_LOGIN']}:{os.environ['DB_PASSWORD']}@{os.environ['DB_HOST']}:{os.environ['DB_PORT']}/{os.environ['DB_DATABASE']}", echo = True)



with engine.connect() as con:
    con.execute(text('DROP TABLE IF EXISTS users'))
    con.execute(text('''CREATE TABLE IF NOT EXISTS "users" (id SERIAL PRIMARY KEY,
    name VARCHAR(256) NOT NULL,
    surname VARCHAR(256) NOT NULL,
    login VARCHAR(256) NOT NULL,
    pass VARCHAR(256) NOT NULL,
    phone VARCHAR(256) NOT NULL)'''))
    con.execute(text('CREATE INDEX indexLogin ON users(login)'))
    con.execute(text('CREATE INDEX indexName ON users(name)'))
    con.execute(text('CREATE INDEX indexSurname ON users(surname)'))
    con.commit()

df = pd.read_json("ExportJson.json")
df.to_sql("users", con = engine, if_exists = 'append', index = False)