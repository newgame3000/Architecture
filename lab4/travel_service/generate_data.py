from faker import Faker
import random
import json
import os
from sqlalchemy import create_engine, text
from pymongo import MongoClient

try:
    client = MongoClient(os.getenv('MONGO_DB_HOST'), int(os.getenv('MONGO_DB_PORT')))
    db = client.archdb
    routes = db.routes
    travels = db.travels
    routes.drop()
    travels.drop()
except:
    raise Exception('Can`t establish connection to database')

fake = Faker()
count = 1000

for i in range(count):
    data = {}

    id = random.randint(0, 99999999999)
    id = format(id, 'x')
    data['_id'] = f'{id:0<23}'

    users = []
    for j in range (random.randint(1, 5)):
        users.append(random.randint(0, 1000))
    data['users'] = users

    data['route_id'] = random.randint(0, count)

    data['date'] = fake.date('%d-%m-%Y')

    routes.insert_one(data)

for i in range(count):
    data = {}

    id = random.randint(0, 9999999999999)
    id = format(id, 'x')
    data['_id'] = f'{id:0<23}'

    data['start'] = fake.address()

    data['end'] = fake.address()

    travels.insert_one(data)
