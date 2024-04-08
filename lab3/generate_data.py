from faker import Faker
import random
import json

fake = Faker()

count = 1000

res = []

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

    res.append(data)

with open('travels.json', 'w') as f:
    json.dump(res, f)

res = []

for i in range(count):
    data = {}

    id = random.randint(0, 9999999999999)
    id = format(id, 'x')
    data['_id'] = f'{id:0<23}'

    data['start'] = fake.address()

    data['end'] = fake.address()

    res.append(data)

with open('routes.json', 'w') as f:
    json.dump(res, f)