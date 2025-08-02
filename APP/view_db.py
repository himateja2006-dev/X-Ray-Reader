from pymongo import MongoClient
from pprint import pprint

client = MongoClient("mongodb://localhost:27017/xray_db")
db = client["xray_db"]

print("Users:")
for user in db.users.find():
    pprint(user)

print("\nImages:")
for img in db.images.find():
    pprint(img) 