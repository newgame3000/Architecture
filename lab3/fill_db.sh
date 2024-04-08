#!/bin/bash
mongoimport --jsonArray --db archdb --collection travels --file travels.json
mongoimport --jsonArray --db archdb --collection routes --file routes.json